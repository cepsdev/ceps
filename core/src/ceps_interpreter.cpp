/*
Copyright 2021 Tomas Prerovsky (cepsdev@hotmail.com).

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.elete
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "ceps_interpreter.hh"
#include "symtab.hh"
#include <cmath>
#include "ceps_interpreter_loop.hh"
#include "ceps_interpreter_nodeset.hh"
#include "pugixml.hpp"

bool ceps::interpreter::DEBUG_OUTPUT = false;

int ceps::interpreter::Environment::lookup_kind(std::string const& k)
{
	auto it_id = kind_to_id_map_.find(k);

	if (it_id == kind_to_id_map_.end())
	 return kind_to_id_map_[k] = ++kind_to_id_map_ctr_;
	return it_id->second;
}


void ceps::interpreter::Environment::register_global_binop_overload( 	ceps::interpreter::Environment::Fn_binop_overload fn,
																char op,
																std::string const & lhs_kind,
																std::string const & rhs_kind )
{
	int id_lhs = lookup_kind(lhs_kind);
	int id_rhs = lookup_kind(rhs_kind);

	global_binop_overloads_[std::make_tuple(op,id_lhs,id_rhs)] = fn;
}


ceps::interpreter::Environment::Fn_binop_overload ceps::interpreter::Environment::get_glbl_binop_overload(  char op,
		                                    std::string const & lhs_kind,
											std::string const & rhs_kind)
{
 auto it = global_binop_overloads_.find(std::make_tuple(op,lookup_kind(lhs_kind),lookup_kind(rhs_kind)));
 if (it != global_binop_overloads_.end())
	 return it->second;
 return nullptr;
}



/**
 * evaluate ( A{B1;B2;B3;} ) = evaluate(B1);evaluate(B2);evaluate(B3);
 *
 */
void ceps::interpreter::evaluate(	 ceps::ast::Nodeset & universe,
									 ceps::ast::Nodebase_ptr root_,
  		 	 	 	 	 	 	 	 ceps::parser_env::Symboltable & sym_table,
  		 	 	 	 	 	 	 	 ceps::interpreter::Environment& env,
  		 	 	 	 	 	 	 	 std::vector<ceps::ast::Nodebase_ptr>* generated_nodes)
{

	if (root_ == nullptr) return;

	if (root_->kind() == ceps::ast::Ast_node_kind::int_literal ||
		root_->kind() == ceps::ast::Ast_node_kind::float_literal ||
		root_->kind() == ceps::ast::Ast_node_kind::string_literal)
	{
		if (generated_nodes != nullptr) generated_nodes->push_back(root_);
		return;
	}

	auto root = ceps::ast::nlf_ptr(root_);
	env.associated_universe() = &universe;
	ceps::ast::Nodebase_ptr predecessor = nullptr;
	for(auto p : root->children())
	{
		if (p->kind() == Kind::stmts)
		{
			ceps::ast::Nodebase_ptr predecessor = nullptr;
			for(auto pp: as_stmts_ptr(p)->children() )
			{
				bool symbols_found{false};
				auto ev = evaluate_generic(pp,sym_table,env,p,predecessor,nullptr,symbols_found,thoroughness_t::normal);predecessor = ev;
				if (ev  != nullptr && ev->kind() == Kind::nodeset)
				{
					universe.nodes().insert(universe.nodes().end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
					if (generated_nodes != nullptr)
						generated_nodes->insert(generated_nodes->end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());

				} else if (ev != nullptr && ev->kind() == Kind::stmts){
				 auto& stmts = as_stmts_ref(ev);
				 if (stmts.children().size()){
				  universe.nodes().insert(universe.nodes().end(),stmts.children().begin(),stmts.children().end());
				  if (generated_nodes != nullptr) generated_nodes->insert(generated_nodes->end(),stmts.children().begin(),stmts.children().end());
				 }
				}else if (ev != nullptr)
				{
					universe.nodes().push_back(ev);
					if (generated_nodes != nullptr) generated_nodes->push_back(ev);
				}
			}//for
			continue;
		}
		bool symbols_found{false};
		auto ev = evaluate_generic(p,sym_table,env,/*p??*/root_,predecessor,nullptr,symbols_found,thoroughness_t::normal);predecessor=p;

		if (ev != nullptr && ev->kind() == Kind::nodeset)
		{
			universe.nodes().insert(universe.nodes().end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
			if (generated_nodes != nullptr)
									generated_nodes->insert(generated_nodes->end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
		} else if (ev != nullptr && ev->kind() == Kind::stmts){
		  auto& stmts = as_stmts_ref(ev);
	      if (stmts.children().size()){
	       universe.nodes().insert(universe.nodes().end(),stmts.children().begin(),stmts.children().end());
	       if (generated_nodes != nullptr) generated_nodes->insert(generated_nodes->end(),stmts.children().begin(),stmts.children().end());
	      }
		}
		else if (ev != nullptr) {
			universe.nodes().push_back(ev);
			if (generated_nodes != nullptr) generated_nodes->push_back(ev);
		}
	}//for
}//ceps::interpreter::evaluate


void ceps::interpreter::evaluate_without_modifying_universe(ceps::ast::Nodeset & universe,
                                     ceps::ast::Nodebase_ptr root_,
                                     ceps::parser_env::Symboltable & sym_table,
                                     ceps::interpreter::Environment& env,
                                     std::vector<ceps::ast::Nodebase_ptr>* generated_nodes
                 )
{

    if (root_ == nullptr) return;

    if (root_->kind() == ceps::ast::Ast_node_kind::int_literal ||
        root_->kind() == ceps::ast::Ast_node_kind::float_literal ||
        root_->kind() == ceps::ast::Ast_node_kind::string_literal)
    {
        if (generated_nodes != nullptr) generated_nodes->push_back(root_);
        return;
    }


    auto root = ceps::ast::nlf_ptr(root_);
    env.associated_universe() = &universe;
    ceps::ast::Nodebase_ptr predecessor = nullptr;
    auto temp = universe.nodes();
    for(auto p : root->children())
    {
        if (p->kind() == Kind::stmts)
        {
            ceps::ast::Nodebase_ptr predecessor = nullptr;
            for(auto pp: as_stmts_ptr(p)->children() )
            {
				bool symbols_found{false};
                auto ev = evaluate_generic(pp,sym_table,env,p,predecessor,nullptr, symbols_found,thoroughness_t::normal);predecessor = ev;
                if (ev  != nullptr && ev->kind() == Kind::nodeset)
                {
                    universe.nodes().insert(universe.nodes().end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
                    if (generated_nodes != nullptr)
                        generated_nodes->insert(generated_nodes->end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());

                } else if (ev != nullptr && ev->kind() == Kind::stmts){
                 auto& stmts = as_stmts_ref(ev);
                 if (stmts.children().size()){
                  universe.nodes().insert(universe.nodes().end(),stmts.children().begin(),stmts.children().end());
                  if (generated_nodes != nullptr) generated_nodes->insert(generated_nodes->end(),stmts.children().begin(),stmts.children().end());
                 }
                }else if (ev != nullptr)
                {
                    universe.nodes().push_back(ev);
                    if (generated_nodes != nullptr) generated_nodes->push_back(ev);
                }
            }//for
            continue;
        }
		bool symbols_found{false};
        auto ev = evaluate_generic(p,sym_table,env,/*p??*/root_,predecessor,nullptr, symbols_found,thoroughness_t::normal);predecessor=p;

        if (ev != nullptr && ev->kind() == Kind::nodeset)
        {
            universe.nodes().insert(universe.nodes().end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
            if (generated_nodes != nullptr)
                                    generated_nodes->insert(generated_nodes->end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
        } else if (ev != nullptr && ev->kind() == Kind::stmts){
          auto& stmts = as_stmts_ref(ev);
          if (stmts.children().size()){
           universe.nodes().insert(universe.nodes().end(),stmts.children().begin(),stmts.children().end());
           if (generated_nodes != nullptr) generated_nodes->insert(generated_nodes->end(),stmts.children().begin(),stmts.children().end());
          }
        }
        else if (ev != nullptr) {
            universe.nodes().push_back(ev);
            if (generated_nodes != nullptr) generated_nodes->push_back(ev);
        }
    }//for
    universe.nodes() = temp;

}//ceps::interpreter::evaluate

ceps::ast::Nodebase_ptr ceps::interpreter::eval_kinddef(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness
		)
{
 using namespace ceps::parser_env;

 ceps::ast::Kinddef& kind_def_node = *dynamic_cast<ceps::ast::Kinddef*>(root_node);
 ceps::parser_env::Symbol* sym_kind_ptr =
	sym_table.lookup(ceps::ast::get<0>(kind_def_node),true);
 sym_kind_ptr->category = ceps::parser_env::Symbol::KIND;
 for (ceps::ast::Nodebase* pnbs : kind_def_node.children())
 {
	 ceps::ast::Identifier & v = ceps::ast::as_id_ref(pnbs);
	 ceps::parser_env::Symbol* sym_ptr =
			 				sym_table.lookup(ceps::ast::name(v),true);
	 sym_ptr->category = ceps::parser_env::Symbol::SYMBOL;
	 ceps::parser_env::Symbol* copy_of_sym_kind_ptr = new ceps::parser_env::Symbol(ceps::parser_env::Symbol::KIND);
	 copy_of_sym_kind_ptr->name = sym_kind_ptr->name;
	 copy_of_sym_kind_ptr->payload = nullptr;
	 sym_ptr->payload = copy_of_sym_kind_ptr;
 }
 return nullptr;
}

ceps::ast::Nodebase_ptr ceps::interpreter::eval_valdef(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness)
{
 using namespace ceps::parser_env;
 ceps::ast::Valdef& val_node = *dynamic_cast<ceps::ast::Valdef*>(root_node);
 ceps::parser_env::Symbol* sym_ptr;
 bool local_symbols_found{false};

 ceps::ast::Nodebase_ptr rhs = evaluate_generic(dynamic_cast<ceps::ast::Nonleafbase*>(root_node)->children()[0],sym_table,env,root_node,nullptr,nullptr,local_symbols_found,thoroughness);
 if (local_symbols_found){
	symbols_found = true;
  	return new ceps::ast::Valdef{name(val_node),rhs};
 }

 if ( (sym_ptr = sym_table.lookup(name(val_node),true,true,false)) == nullptr)
 {
	 throw semantic_exception{root_node,"Couldn't define Variable '" +name(val_node)+"' already defined."};
 }

 ceps::parser_env::Symbol& sym = *sym_ptr;
 sym.category = ceps::parser_env::Symbol::Category::VAR;
 sym.payload = rhs;//TODO: See comment in symtab.hh

 return nullptr;//Called because of side effect, no return value. Variable definitions end up in the symbol table and disappear from the tree.
}

ceps::ast::Nodebase_ptr ceps::interpreter::eval_unaryop(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness
		){

	ceps::ast::Unary_operator& unop = as_unary_op_ref(root_node);
	bool local_symbols_found{false};
	auto operand = evaluate_generic(unop.children()[0],sym_table,env,root_node,nullptr,nullptr,local_symbols_found,thoroughness);
	if (local_symbols_found){
		symbols_found = true;
		return new ceps::ast::Unary_operator{op(unop),operand};
	}
	bool operand_boolean{false};
	bool operand_bool_equiv{false};

	//Check for a node set
	if (is<Ast_node_kind::nodeset>(operand)){
		auto& set_of_nodes{as_ast_nodeset_ref(operand)};
		if (!set_of_nodes.children().size()) {operand_boolean=true;operand_bool_equiv = true;}
		else
			operand = set_of_nodes.children()[0];
	}

	if (op(unop) == '-')
	{
		if (operand->kind() == Kind::int_literal)
			value(as_int_ref(operand)) *= -1;
		else if (operand->kind() == Kind::float_literal)
			value(as_double_ref(operand)) *= -1.0;
		return operand;
	}
	else if (op(unop) == '!')
	{
		if (operand_bool_equiv){
			return operand_boolean ? mk_int_node(1) : mk_int_node(0);
		} else {
			if (operand->kind() == Kind::int_literal)
			{
				if ( value(as_int_ref(operand)) == 0)
					return mk_int_node(1);
				else 
					return mk_int_node(0);
			} else if (	is<Ast_node_kind::symbol> (operand) || is<Ast_node_kind::binary_operator> (operand) ||
						is<Ast_node_kind::unary_operator> (operand) || is<Ast_node_kind::func_call> (operand))
				return mk_unary_op_node(op(unop),operand);
		}
	}
	throw semantic_exception{root_node,"Illformed unary operator expression"};
}


ceps::ast::Nodebase_ptr ceps::interpreter::eval_binaryop(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		ceps::interpreter::thoroughness_t thoroughness
		)
{

	ceps::ast::Binary_operator& binop = *dynamic_cast<ceps::ast::Binary_operator*>(root_node);
	if (binop.children().size() != 2)
	{
		throw semantic_exception{root_node,"Expecting 2 arguments, given " +make_string(binop.children().size())};
	}
	ceps::ast::Nodebase_ptr result;


	if (op_val(binop) != "=")
	{
	 bool local_symbols_found_l{false};
	 bool local_symbols_found_r{false};
	 ceps::ast::Nodebase_ptr lhs = evaluate_generic(binop.children()[0],sym_table,env,root_node,predecessor,nullptr,local_symbols_found_l,thoroughness);
	 ceps::ast::Nodebase_ptr rhs = evaluate_generic(binop.children()[1],sym_table,env,root_node,predecessor, (op(binop) == '.' ? lhs : nullptr) ,local_symbols_found_r, thoroughness);
	 symbols_found = local_symbols_found_l || local_symbols_found_r;

	 //std::cout << "thoroughness="<< (int)thoroughness << " "<< op_val(binop) << ":\n"<< "\t" << *lhs << "\n\t"<< *rhs << std::endl;


	 //A single element nodeset, with no idx operand set, evaluates to its only element (execption: operator is '.')
	 if(op(binop) != '.'){
		if (is<Ast_node_kind::nodeset>(lhs)){
			auto& ns = as_ast_nodeset_ref(lhs);
			if (apply_idx_op_operand(ns) == "" && ns.children().size() == 1){				
				lhs = ns.children()[0];
			} 
		}
		if (is<Ast_node_kind::nodeset>(rhs)){
			auto& ns = as_ast_nodeset_ref(rhs);
			if (apply_idx_op_operand(ns) == "" && ns.children().size() == 1){				
					rhs = ns.children()[0];
			} 
		}
	 } else if (lhs->kind() == Kind::nodeset || lhs->kind() == Kind::structdef || lhs->kind() == Kind::scope) /*op == '.'*/{
		if (local_symbols_found_l || local_symbols_found_r)  return mk_bin_op(ceps::ast::op(binop),lhs,rhs,ceps::ast::op_str(binop));
		if(lhs->kind() == Kind::nodeset)
			return evaluate_nodeset_expr_dot(	lhs,
												rhs ,
												sym_table,
												env,
												root_node,
												thoroughness,
												symbols_found
											);
		else if (lhs->kind() == Kind::structdef){
			return evaluate_nodeset_expr_dot(	create_ast_nodeset("",{lhs}),
												rhs ,
												sym_table,
												env,
												root_node,
												thoroughness,
												symbols_found
											);
		} else if (lhs->kind() == Kind::scope)
			return evaluate_nodeset_expr_dot(	create_ast_nodeset("",as_scope_ref(lhs).children()),
												rhs ,
												sym_table,
												env,
												root_node,
												thoroughness,
												symbols_found
											);
	 }

		if ( (lhs->kind() == ceps::ast::Ast_node_kind::symbol
				|| rhs->kind() == ceps::ast::Ast_node_kind::symbol
				|| lhs->kind() == ceps::ast::Ast_node_kind::binary_operator
				|| rhs->kind() == ceps::ast::Ast_node_kind::binary_operator
				|| lhs->kind() == ceps::ast::Ast_node_kind::func_call
				|| rhs->kind() == ceps::ast::Ast_node_kind::func_call) )
			{
				auto override_value = env.call_binop_resolver(&binop,lhs,rhs,parent_node);
				if (override_value) {symbols_found = false; return override_value;}
				auto t = mk_bin_op(ceps::ast::op(binop),lhs,rhs,ceps::ast::op_str(binop));
				return t;
			}
		bool local_symbols_found_temp{false};
	 	result = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table,env,root_node,op(binop) == '.' ? lhs : nullptr,thoroughness,local_symbols_found_temp);
	}
	else /* operator is = */
	{
		bool local_symbols_found_l{false};
	 	bool local_symbols_found_r{false};
		ceps::ast::Nodebase_ptr lhs =  evaluate_generic(binop.children()[0],sym_table,env,root_node,nullptr, nullptr,local_symbols_found_l,thoroughness);
		ceps::ast::Nodebase_ptr unevaluated_lhs = binop.children()[0];
		ceps::ast::Nodebase_ptr rhs = evaluate_generic(binop.children()[1],sym_table,env,root_node,binop.children()[0],nullptr, local_symbols_found_r, thoroughness);

		 bool treat_lhs_as_symbol = false;
		 if (unevaluated_lhs->kind() == ceps::ast::Ast_node_kind::identifier){
			 	ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(unevaluated_lhs);
		 	 	ceps::parser_env::Symbol* sym_ptr;
	 		 	if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr) treat_lhs_as_symbol=true;
	 		 	if (sym_ptr && sym_ptr->category != ceps::parser_env::Symbol::Category::VAR) treat_lhs_as_symbol=true;
	 		 	else if (sym_ptr && sym_ptr->category == ceps::parser_env::Symbol::Category::VAR) lhs = unevaluated_lhs;
		 } else if (ceps::ast::is<ceps::ast::Ast_node_kind::symbol>(unevaluated_lhs)){
				auto override_value = env.call_binop_resolver(&binop,unevaluated_lhs,rhs,parent_node);
				if (override_value) {symbols_found = false; return override_value;}
		 }

		 if( treat_lhs_as_symbol ||
			 lhs->kind() == ceps::ast::Ast_node_kind::symbol ||
		     lhs->kind() == ceps::ast::Ast_node_kind::binary_operator ||
		     lhs->kind() == ceps::ast::Ast_node_kind::unary_operator ||
		     lhs->kind() == ceps::ast::Ast_node_kind::func_call )
		 {
			 auto t = mk_bin_op(op(binop),lhs,rhs,op_val(binop));
			 return t;
		 }
		 else {
			 
			 result  = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table,env,root_node,nullptr, thoroughness,symbols_found);
			 symbols_found = symbols_found || local_symbols_found_l || local_symbols_found_r;
		 }
	}
	return result;
}


ceps::ast::Nodebase_ptr ceps::interpreter::evaluate_nonleaf(ceps::ast::Nonleafbase& root,
		                                            ceps::parser_env::Symboltable & sym_table,
		                                            ceps::interpreter::Environment& env, 
													ceps::ast::Nodebase_ptr parent_node,
													ceps::ast::Nodebase_ptr predecessor,
										 			ceps::ast::Nodebase_ptr this_ptr,
													bool& symbols_found,
													thoroughness_t thoroughness
													)
{
	using namespace ceps::ast;
	Nonleafbase::Container_t v;

    auto old_scope = env.scope;
    env.scope = &v;
	Nodebase* root_ptr = dynamic_cast<Nodebase*>(&root); //The dynamic cast is REALLY necessary here
	for(Nodebase_ptr p : root.children())
	{
		bool symbols_found_local{false};
		Nodebase_ptr r = evaluate_generic(p,sym_table,env,root_ptr,predecessor,this_ptr,symbols_found_local, thoroughness);predecessor=r;
		symbols_found = symbols_found || symbols_found_local;
		if(r == nullptr)
			continue;
		if (r->kind() == Ast_node_kind::stmts || r->kind() == Ast_node_kind::nodeset)
		{
			for(Nodebase_ptr elem : nlf_ptr(r)->children())
				v.push_back(elem);
		}
		else v.push_back(r);
	}
	

	//rec_flatten_tree(v, root, sym_table, env, parent_node, predecessor, this_ptr, true);

	env.scope = old_scope;

	switch (root_ptr->kind()){
		case Ast_node_kind::root:
		{
			Root* result = mk_root();
			result->children() = v;
			return result;
		}
		case Ast_node_kind::stmts:
		{
			Stmts* result = mk_stmts();
			result->children() = v;
			return result;
		}
		case Ast_node_kind::stmt:
		{
			Stmt* result = mk_stmt();
			result->children() = v;
			return result;
		}
		case Ast_node_kind::structdef:
		{
			ceps::ast::Struct& s = as_struct_ref(root_ptr);
			Struct* result = make_struct(name(s));
			result->children() = v;
			return result;
		}
		case Ast_node_kind::scope:
		{
			Scope* result = mk_scope();
			result->children() = v;
			return result;
		}
		case Ast_node_kind::call_parameters:
		{
			Call_parameters* result = mk_callparameters();
			result->children() = v;
			return result;
		}
		default:
		CEPSERROR("Internal Error.")
	}
}

