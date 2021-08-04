/*
Copyright 2014,2015,2016,2017,2018,2019,2020,2021 Tomas Prerovsky (cepsdev@hotmail.com).

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
	for(auto p : root->children())
	{
		if (p->kind() == Kind::stmts)
		{
			ceps::ast::Nodebase_ptr predecessor = nullptr;
			for(auto pp: as_stmts_ptr(p)->children() )
			{
				auto ev = evaluate_generic(pp,sym_table,env,p,predecessor,nullptr);predecessor = ev;
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
		auto ev = evaluate_generic(p,sym_table,env,/*p??*/root_,predecessor,nullptr);predecessor=p;

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
                auto ev = evaluate_generic(pp,sym_table,env,p,predecessor,nullptr);predecessor = ev;
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
        auto ev = evaluate_generic(p,sym_table,env,/*p??*/root_,predecessor,nullptr);predecessor=p;

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



static void flatten_args(ceps::ast::Nodebase_ptr r, std::vector<ceps::ast::Nodebase_ptr>& v, char op_val = ',')
{
	using namespace ceps::ast;
	if (r == nullptr) return;
	if (r->kind() == ceps::ast::Ast_node_kind::binary_operator && op(as_binop_ref(r)) ==  op_val)
	{
		auto& t = as_binop_ref(r);
		flatten_args(t.left(),v,op_val);
		flatten_args(t.right(),v,op_val);
		return;
	}
	v.push_back(r);
}

ceps::ast::Nodebase_ptr ceps::interpreter::eval_kinddef(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor)
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
		ceps::ast::Nodebase_ptr predecessor)
{
 using namespace ceps::parser_env;
 ceps::ast::Valdef& val_node = *dynamic_cast<ceps::ast::Valdef*>(root_node);
 ceps::parser_env::Symbol* sym_ptr;

 ceps::ast::Nodebase_ptr rhs = evaluate_generic(dynamic_cast<ceps::ast::Nonleafbase*>(root_node)->children()[0],sym_table,env,root_node,nullptr,nullptr);

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
		ceps::ast::Nodebase_ptr predecessor){

	ceps::ast::Unary_operator& unop = as_unary_op_ref(root_node);
	auto operand = evaluate_generic(unop.children()[0],sym_table,env,root_node,nullptr,nullptr);
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

ceps::ast::Nodebase_ptr ceps::interpreter::eval_ifelse(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor)
{
 ceps::ast::Ifelse& ifelse = ceps::ast::as_ifelse_ref(root_node);
 ceps::ast::Nodebase_ptr cond = evaluate_generic(ifelse.children()[0],sym_table,env,root_node,nullptr,nullptr);
 ceps::ast::Nodebase_ptr left_branch = nullptr,right_branch=nullptr;
 if (cond->kind() == ceps::ast::Ast_node_kind::int_literal){
	 ceps::ast::Nodebase_ptr r = nullptr;
	 if (ceps::ast::value(ceps::ast::as_int_ref(cond)) != 0 ){
		 if (ifelse.children().size() > 1) r = evaluate_generic(ifelse.children()[1],sym_table,env,root_node,ifelse.children()[0],nullptr);
	 } else {
		 if (ifelse.children().size() > 2) r = evaluate_generic(ifelse.children()[2],sym_table,env,root_node,ifelse.children()[1],nullptr);
	 }
	 if (r == nullptr || r->kind() != ceps::ast::Ast_node_kind::scope) return r;
	 auto& scope = *ceps::ast::nlf_ptr(r);
	 if (scope.children().size() == 0) return nullptr;
	 if (scope.children().size() == 1) return scope.children()[0];
	 return create_ast_nodeset("", scope.children());
 }

 if (ifelse.children().size() > 1) left_branch = evaluate_generic(ifelse.children()[1],sym_table,env,root_node,ifelse.children()[0],nullptr);
 if (ifelse.children().size() > 2) right_branch = evaluate_generic(ifelse.children()[2],sym_table,env,root_node,ifelse.children()[1],nullptr);
 return new ceps::ast::Ifelse(cond,left_branch,right_branch);
}











ceps::ast::Nodebase_ptr ceps::interpreter::eval_binaryop(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor)
{
	ceps::ast::Binary_operator& binop = *dynamic_cast<ceps::ast::Binary_operator*>(root_node);
	if (binop.children().size() != 2)
	{
		throw semantic_exception{root_node,"Expecting 2 arguments, given " +make_string(binop.children().size())};
	}
	ceps::ast::Nodebase_ptr result;


	if (op(binop) != '=')
	{
	 ceps::ast::Nodebase_ptr lhs = evaluate_generic(binop.children()[0],sym_table,env,root_node,predecessor,nullptr);
	 ceps::ast::Nodebase_ptr rhs = evaluate_generic(binop.children()[1],sym_table,env,root_node,predecessor, op(binop) == '.' ? lhs : nullptr );

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
	 } else /*op == '.'*/{
		if(lhs->kind() == Kind::nodeset)
			return evaluate_nodeset_expr_dot(	lhs,
												rhs ,
												sym_table,
												env,root_node
											);
		else if (lhs->kind() == Kind::structdef)
			return evaluate_nodeset_expr_dot(	create_ast_nodeset("",as_struct_ref(lhs).children()),
												rhs ,
												sym_table,
												env,root_node
											);
		else if (lhs->kind() == Kind::scope)
			return evaluate_nodeset_expr_dot(	create_ast_nodeset("",as_scope_ref(lhs).children()),
												rhs ,
												sym_table,
												env,root_node
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
		 if (override_value) return override_value;
		 auto t = mk_bin_op(ceps::ast::op(binop),lhs,rhs,ceps::ast::op_str(binop));
		 return t;
	 }
	 result = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table,env,root_node,op(binop) == '.' ? lhs : nullptr);
	}
	else /* operator is = */
	{
		 ceps::ast::Nodebase_ptr lhs =  evaluate_generic(binop.children()[0],sym_table,env,root_node,nullptr,nullptr);
		 ceps::ast::Nodebase_ptr unevaluated_lhs = binop.children()[0];
		 ceps::ast::Nodebase_ptr rhs = evaluate_generic(binop.children()[1],sym_table,env,root_node,binop.children()[0],nullptr);

		 bool treat_lhs_as_symbol = false;

		 if (unevaluated_lhs->kind() == ceps::ast::Ast_node_kind::identifier){
			 ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(unevaluated_lhs);
		 	 ceps::parser_env::Symbol* sym_ptr;
	 		 if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr) treat_lhs_as_symbol=true;
	 		 if (sym_ptr && sym_ptr->category != ceps::parser_env::Symbol::Category::VAR) treat_lhs_as_symbol=true;
	 		 else if (sym_ptr && sym_ptr->category == ceps::parser_env::Symbol::Category::VAR) lhs = unevaluated_lhs;
		 }

		 if( treat_lhs_as_symbol ||
			 lhs->kind() == ceps::ast::Ast_node_kind::symbol ||
		     lhs->kind() == ceps::ast::Ast_node_kind::binary_operator ||
		     lhs->kind() == ceps::ast::Ast_node_kind::unary_operator ||
		     lhs->kind() == ceps::ast::Ast_node_kind::func_call )
		 {
			 auto t = mk_bin_op(ceps::ast::op(binop),lhs,rhs,ceps::ast::op_str(binop));
			 return t;
		 }
		 else result  = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table,env,root_node,nullptr);
	}
	return result;
}

















ceps::ast::Nodebase_ptr ceps::interpreter::eval_id(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor)
{
	using namespace ceps::parser_env;

	ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(root_node);

	if (name(id) == "m" || name(id) == "metre" || name(id) == "meter")
	{
		return new ceps::ast::Int(1, ceps::ast::m_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "s" || name(id) == "second")
	{
		return new ceps::ast::Int(1, ceps::ast::s_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "kg" || name(id) == "kilogram")
	{
		return new ceps::ast::Int(1, ceps::ast::kg_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "celsius" || name(id) == "kelvin")
	{
		return new ceps::ast::Int(1, ceps::ast::kelvin_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "ampere" )
	{
		return new ceps::ast::Int(1, ceps::ast::ampere_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "cd" || name(id) == "candela" )
	{
		return new ceps::ast::Int(1, ceps::ast::candela_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "mol" || name(id) == "mole" )
	{
		return new ceps::ast::Int(1, ceps::ast::mol_unit(), nullptr, nullptr, nullptr);
	}
	else if (name(id) == "scope"){
		if (env.scope)
		 return create_ast_nodeset("", *env.scope);
		return create_ast_nodeset("",std::vector<ceps::ast::Nodebase_ptr>{});
	}
	else if (name(id) == "root" && env.associated_universe() != nullptr)
		return create_ast_nodeset("", env.associated_universe()->nodes());
	else if (name(id) == "arglist"){
		ceps::parser_env::Symbol* sym_ptr;
		if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr)
			throw semantic_exception{root_node,"arglist undefined (not inside macro body)."};
		if (sym_ptr->category != ceps::parser_env::Symbol::Category::NODESET)
			throw semantic_exception{root_node,"arglist was redefined with wrong type (should be nodeset)."};
		if(nullptr == sym_ptr->payload)
		     throw semantic_exception{root_node,"arglist undefined."};

		ceps::ast::Ast_nodeset_ptr ndeset = (ceps::ast::Ast_nodeset_ptr)(sym_ptr->payload);
		return ndeset;
	}

	 ceps::parser_env::Symbol* sym_ptr;

	 if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr)
	 {
		 //throw semantic_exception{root_node,"Variable '" +name(id)+"' is not defined."};
		 std::string id_name = name(id);
		 return new ceps::ast::Identifier(id_name,nullptr,nullptr,nullptr);
	 }

	 ceps::parser_env::Symbol& sym = *sym_ptr;
	 if (sym_ptr->category == ceps::parser_env::Symbol::Category::SYMBOL )
	 {
		 return new ceps::ast::Symbol(name(id), ((ceps::parser_env::Symbol*)sym_ptr->payload)->name, nullptr, nullptr, nullptr);
	 }


	 if (sym_ptr->category == ceps::parser_env::Symbol::Category::MACRO)
	 {
		 return new ceps::ast::Identifier(name(id),nullptr,nullptr,nullptr);
	 }
	 else if (sym.category != ceps::parser_env::Symbol::Category::VAR)
		 throw semantic_exception{root_node,"Variable '" +name(id)+"' is not defined."};
	
	if (sym.payload == nullptr) return new ceps::ast::Undefined(nullptr,nullptr,nullptr);

	 ceps::ast::Nodebase_ptr node_ptr = reinterpret_cast<ceps::ast::Nodebase_ptr>(sym.payload);


	 if (node_ptr->kind() == ceps::ast::Ast_node_kind::float_literal)
	 {
		 ceps::ast::Double & v = *dynamic_cast<ceps::ast::Double*>(node_ptr);
		 return new ceps::ast::Double(value(v), unit(v), nullptr, nullptr, nullptr);
	 }
	 else if (node_ptr->kind() == ceps::ast::Ast_node_kind::int_literal)
	 {
		 ceps::ast::Int & v = *dynamic_cast<ceps::ast::Int*>(node_ptr);
		 return new ceps::ast::Int( value(v), unit(v), nullptr, nullptr, nullptr );
	 }
	 else if (node_ptr->kind() == ceps::ast::Ast_node_kind::string_literal)
	 {
		 ceps::ast::String & v = *dynamic_cast<ceps::ast::String*>(node_ptr);
		 return new ceps::ast::String(value(v), nullptr, nullptr, nullptr);
	 }
	 else if (node_ptr->kind() == ceps::ast::Ast_node_kind::symbol)
	 {
	 	 auto & v = as_symbol_ref(node_ptr);
	 	 //std::cerr << v << std::endl;
		 return new ceps::ast::Symbol(name(v), kind(v), nullptr, nullptr, nullptr);

	 }
	 else
		 return node_ptr;

}

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate_generic(ceps::ast::Nodebase_ptr root_node,
													ceps::parser_env::Symboltable & sym_table,
													ceps::interpreter::Environment& env,
													ceps::ast::Nodebase_ptr parent_node,
													ceps::ast::Nodebase_ptr predecessor,
													ceps::ast::Nodebase_ptr this_ptr)
 {
	 if(!root_node)
		 return nullptr;
	 if (ceps::interpreter::DEBUG_OUTPUT) std::cerr << "ceps::interpreter::evaluate_generic " << *root_node << std::endl;

	 switch(root_node->kind())
	 {
	 case Kind::kind_def:
	 {
		 return eval_kinddef(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
	 }
	 case Kind::valdef:
	 {
		 return eval_valdef(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
	 }
	 case Kind::let:
	 {
		ceps::ast::Let& let_node{ceps::ast::as_let_ref(root_node)};
 		ceps::parser_env::Symbol* sym_ptr;

 		ceps::ast::Nodebase_ptr rhs = evaluate_generic(let_node.children()[0],sym_table,env,root_node,nullptr,nullptr);

 		if ( (sym_ptr = sym_table.lookup(name(let_node),false,false,false)) == nullptr)
	 		throw semantic_exception{root_node,"Couldn't assign to Variable '" +name(let_node)+"': not defined."};

 		ceps::parser_env::Symbol& sym = *sym_ptr;
 		sym.payload = rhs;//TODO: See comment in symtab.hh
 		return nullptr;//Called because of side effect, no return value. Variable definitions end up in the symbol table and disappear from the tree.
	 }
	 case Kind::stmts:
	 case Kind::root:
	 case Kind::stmt:
	 case Kind::call_parameters:
		 return evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr);
	 case Kind::scope:
	 {
		 sym_table.push_scope();
	 	 auto r = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr);
	 	 sym_table.pop_scope();
	 	 return r;
	 }
	 case Kind::structdef:
	 {
		ceps::ast::Nodebase_ptr result = nullptr;
		sym_table.push_scope();
		std::string id = ceps::ast::name(ceps::ast::as_struct_ref(root_node));

		ceps::parser_env::Symbol* sym_ptr;
		if ( (sym_ptr = sym_table.lookup(id)) != nullptr && sym_ptr->category ==  ceps::parser_env::Symbol::Category::MACRO){
			result =  eval_macro(root_node,
					  sym_ptr,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
		} else if ( (sym_ptr = sym_table.lookup(id)) != nullptr && sym_ptr->category ==  ceps::parser_env::Symbol::Category::REWRITE){
			result =  eval_rewrite(root_node,
					  sym_ptr,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
		} else if (
				(sym_ptr = sym_table.lookup(id)) != nullptr &&
				sym_ptr->category ==  ceps::parser_env::Symbol::Category::VAR &&
				sym_ptr->payload != nullptr &&

				(
				 (((ceps::ast::Nodebase_ptr)sym_ptr->payload)->kind() == ceps::ast::Ast_node_kind::nodeset)
				)
		)
		{
			std::string id;
			auto& ns = ceps::ast::as_ast_nodeset_ref(((ceps::ast::Nodebase_ptr)sym_ptr->payload));
			if (ns.children().size() > 0)
			{
			 if (ns.children()[0]->kind() == ceps::ast::Ast_node_kind::identifier)
				id = ceps::ast::name(ceps::ast::as_id_ref( ns.children()[0] ));
			 else if (ns.children()[0]->kind() == ceps::ast::Ast_node_kind::symbol)
				id = ceps::ast::name(ceps::ast::as_symbol_ref( ns.children()[0] ));
			}
			result = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr);
			if (id.length()) ceps::ast::name(ceps::ast::as_struct_ref(result)) = id;

		} else result = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr);
		sym_table.pop_scope();
		if (id == "ignore_value") return nullptr;
		if (id == "comment_stmt") return nullptr;
		return result;
	 }
	 case Kind::expr:
	 {
		ceps::ast::Nonleafbase& nleaf = *dynamic_cast<ceps::ast::Nonleafbase*>(root_node);


		if ( nleaf.children().size() != 1)
			{
				CEPSERROR("Internal error: Expression invalid.")
			}

		ceps::ast::Nodebase_ptr result = evaluate_generic(nleaf.children()[0],sym_table,env,root_node,predecessor,this_ptr),this_ptr;
		//nleaf.children().clear();
		return result;
	 }
	 case Kind::unary_operator:
	 {
		 return eval_unaryop(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
	 }
	 case Kind::ifelse:
	 {
		 return eval_ifelse(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
	 }
	 case Kind::func_call:
	 {
		 if (ceps::interpreter::DEBUG_OUTPUT) std::cerr << "ceps::interpreter::evaluate_generic Kind::func_call:\n";
		 return eval_funccall(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,
					  this_ptr);
	 }
	 case Kind::binary_operator:
	 {
		 auto& bop{as_binop_ref(root_node)};
		 if (op_val(bop) != "#") return eval_binaryop(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
		return evaluate_generic(bop.left(),sym_table,env,root_node,predecessor,this_ptr);
	 }
	 case Kind::identifier:
	 {
		 return eval_id(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor);
	 }
	 case Kind::rawmap:
	 {
		 using namespace ceps::ast;
		 Rawmap_ptr map_p_src = as_rawmap_ptr(root_node);
		 Rawmap_ptr map_p_dest = new Rawmap{};
		 for(Nodebase_ptr p : map_p_src->children())
		 {
			 if (p->kind() != Kind::atoms) continue;
			 Atoms_ptr atoms_p_src = as_atoms_ptr(p);
			 Atoms_ptr atoms_p_dest = new Atoms{};
			 predecessor = nullptr;
			 for (Nodebase_ptr pp : atoms_p_src->children())
			 {
				 Nodebase_ptr r = evaluate_generic(pp,sym_table,env,root_node,predecessor,this_ptr);predecessor=pp;
				 if (r == nullptr)
					 continue;
				 atoms_p_dest->children().push_back(r);
			 }
			 map_p_dest->children().push_back(atoms_p_dest);
		 }
		 return map_p_dest;
	 }
	 case Kind::float_literal:
	 {
		 using namespace ceps::ast;
		 Double & v = as_double_ref(root_node);
		 return new Double(value(v), unit(v), nullptr, nullptr, nullptr);
	 }
	 case Kind::int_literal:
	 {
		 using namespace ceps::ast;
		 Int & v = as_int_ref(root_node);
		 return new Int( value(v), unit(v), nullptr, nullptr, nullptr);
	 }
	 case Kind::string_literal:
	 {
		 using namespace ceps::ast;
		 String & v = as_string_ref(root_node);
		 return new String(value(v), nullptr, nullptr, nullptr);
	 }
	 case Kind::loop:
	 {
		 auto l = evaluate_loop(as_loop_ptr(root_node),
				  	  	  	  sym_table,
				  	  	  	  env,root_node,predecessor);
		 return l;
	 }
	 case Kind::symbol:
	 {
		 std::string kind = ceps::ast::kind(ceps::ast::as_symbol_ref(root_node));
		 std::string name = ceps::ast::name(ceps::ast::as_symbol_ref(root_node));
		 if (ceps::interpreter::DEBUG_OUTPUT) 
		  std::cerr << "ceps::interpreter::evaluate_generic Kind::symbol: kind(" << kind<< ") name("<< name << ")" << std::endl;
		 if (env.symbol_mapping()[kind] != nullptr)
		 {

			 auto r = (*env.symbol_mapping()[kind])[name];
			 
			 if (r != nullptr) {
				auto rv = evaluate_generic(r,sym_table, env,root_node,predecessor,this_ptr);
				if (ceps::interpreter::DEBUG_OUTPUT) 
		      		std::cerr << "ceps::interpreter::evaluate_generic Kind::symbol: kind(" << kind<< ") name("<< name << ") -> "<<*rv << std::endl;
				 return rv;
			 }
			 return env.call_sym_undefined_clbk(root_node,parent_node);
		 }
         return new ceps::ast::Symbol(name,kind);
		 return root_node;
	 }
	 case Kind::ret:
	 {
		 auto& ret_stmt = ceps::ast::as_return_ref(root_node);
		 ceps::ast::Nodebase_ptr operand = evaluate_generic(ret_stmt.children()[0],sym_table,env,(ceps::ast::Nodebase_ptr)&ret_stmt,predecessor,this_ptr);
		 return new ceps::ast::Return(operand);
	 }
	 case Kind::macro_definition:{
		 auto& macrodef{ceps::ast::as_macrodef_ref(root_node)};

		 return new::ceps::ast::Macrodef(name(macrodef),symbol_table_info(macrodef),attributes(macrodef));
	 }
	 default:
		 return root_node;
		 //ERROR("Internal error: Kind of node unknown.")
	 }
	 return nullptr;
 }
ceps::ast::Nodebase_ptr ceps::interpreter::handle_binop(	ceps::ast::Nodebase_ptr binop_node,
															int op,
															ceps::ast::Nodebase_ptr lhs,
															ceps::ast::Nodebase_ptr rhs,
															ceps::parser_env::Symboltable & sym_table,
															Environment& env,
															ceps::ast::Nodebase_ptr parent_node,
															ceps::ast::Nodebase_ptr this_ptr
															)
{
	using namespace ceps::ast;
	if (lhs == nullptr || rhs == nullptr)
		{ CEPSERROR("Internal error: Operand(s) evaluates to null.") }

	if (op == '=')
	{
		using namespace ceps::parser_env;
		if (lhs->kind() != Kind::identifier){
			throw semantic_exception{binop_node," Left hand side of assignment should be a variable"};
		}

		ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(lhs);
		ceps::parser_env::Symbol* sym_ptr;

		if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr)
		{
		 throw semantic_exception{binop_node,"Variable '" +name(id)+"' is not defined."};
		}

		ceps::parser_env::Symbol& sym = *sym_ptr;
		if (sym.category != ceps::parser_env::Symbol::Category::VAR)
			 throw semantic_exception{binop_node,"Variable '" +name(id)+"' is not defined."};

		 sym.payload = rhs;

		return rhs;
	}

	
	//Promotions / Coercions
	if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::int_literal && op != '^')
	{
		rhs = mk_double_node(value(as_int_ref(rhs)), unit(as_int_ref(rhs)));
	}
	else if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::float_literal)
	{
		lhs = mk_double_node(value(as_int_ref(lhs)), unit(as_int_ref(lhs)));
	}

	if (op == ceps::Cepsparser::token::REL_OP_EQ)
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new Int( (value(lhs_ref) == value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new Int( (value(lhs_ref) == os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new Int( (value(rhs_ref) == os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				return new Int( 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			}
			return new Int( (value(lhs_ref) == value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				return new Int( 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			}
			return new Int( (value(lhs_ref) == value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
	}
	if (op == ceps::Cepsparser::token::REL_OP_NEQ)
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new Int( (value(lhs_ref) == value(rhs_ref)) ? 0 : 1, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new Int( (value(lhs_ref) == os.str()) ? 0 : 1, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new Int( (value(rhs_ref) == os.str()) ? 0 : 1, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				return new Int( 1, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			}
			return new Int( (value(lhs_ref) == value(rhs_ref)) ? 0 : 1, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				return new Int( 1, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			}
			return new Int( (value(lhs_ref) == value(rhs_ref)) ? 0 : 1, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
	}
	if (op == ceps::Cepsparser::token::REL_OP_GT)
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new Int( (value(lhs_ref) > value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new Int( (value(lhs_ref) > os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new Int( (value(rhs_ref) > os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '>': Incompatible units."};
			}
			return new Int( (value(lhs_ref) > value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '>': Incompatible units."};
			}
			return new Int( (value(lhs_ref) > value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
	}
	if (op == ceps::Cepsparser::token::REL_OP_LT)
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new Int( (value(lhs_ref) < value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new Int( (value(lhs_ref) < os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new Int( (value(rhs_ref) < os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '<': Incompatible units."};
			}
			return new Int( (value(lhs_ref) < value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '<': Incompatible units."};
			}
			return new Int( (value(lhs_ref) < value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
	}
	if (op == ceps::Cepsparser::token::REL_OP_GT_EQ)
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new Int( (value(lhs_ref) >= value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new Int( (value(lhs_ref) >= os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new Int( (value(rhs_ref) >= os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '>=': Incompatible units."};
			}
			return new Int( (value(lhs_ref) >= value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '>=': Incompatible units."};
			}
			return new Int( (value(lhs_ref) >= value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
	}
	if (op == ceps::Cepsparser::token::REL_OP_LT_EQ)
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new Int( (value(lhs_ref) <= value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new Int( (value(lhs_ref) <= os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new Int( (value(rhs_ref) <= os.str()) ? 1 : 0, ceps::ast::all_zero_unit(),nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '<=': Incompatible units."};
			}
			return new Int( (value(lhs_ref) <= value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '<=': Incompatible units."};
			}
			return new Int( (value(lhs_ref) <= value(rhs_ref)) ? 1 : 0, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}
	}
	/** Addition **/
	if (op == '+')
	{
        if (lhs->kind() == ceps::ast::Ast_node_kind::identifier || rhs->kind() == ceps::ast::Ast_node_kind::identifier)
            return binop_node;

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new String(value(lhs_ref) + value(rhs_ref), nullptr, nullptr, nullptr);
		}

		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new String( value(lhs_ref) + os.str(), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new String(os.str() + value(rhs_ref), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return new Int( value(lhs_ref) + value(rhs_ref), unit(lhs_ref), nullptr, nullptr, nullptr);
		}

		if (lhs->kind() == Kind::long_literal && rhs->kind() == Kind::long_literal)
		{
			auto lhs_ref = *static_cast<Int64*>(lhs);
			auto rhs_ref = *static_cast<Int64*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return mk_int64_node( value(lhs_ref) + value(rhs_ref), unit(lhs_ref));
		}
		if (lhs->kind() == Kind::long_literal && rhs->kind() == Kind::int_literal)
		{
			auto lhs_ref = *static_cast<Int64*>(lhs);
			auto rhs_ref = *static_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return mk_int64_node( value(lhs_ref) + value(rhs_ref), unit(lhs_ref));
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::long_literal)
		{
			auto lhs_ref = *static_cast<Int*>(lhs);
			auto rhs_ref = *static_cast<Int64*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return mk_int64_node( value(lhs_ref) + value(rhs_ref), unit(lhs_ref));
		}

		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return new Double(value(lhs_ref) + value(rhs_ref), unit(lhs_ref), nullptr, nullptr, nullptr);
		}
	}// Addition
    /** Modulo **/
    if (op == '%')
    {

        if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
        {
            Int lhs_ref = *dynamic_cast<Int*>(lhs);
            Int rhs_ref = *dynamic_cast<Int*>(rhs);
            if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
            {
                throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
            }
            return new Int( value(lhs_ref) % value(rhs_ref), unit(lhs_ref), nullptr, nullptr, nullptr);
        } else throw semantic_exception{lhs," Failed to apply '%': Incompatible types."};
    }// Modulo

	/*Logical And*/
	if (op == '&')
		{
			if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
			{
				String lhs_ref = *dynamic_cast<String*>(lhs);
				String rhs_ref = *dynamic_cast<String*>(rhs);
				return new Int( ( value(lhs_ref).length() > 0 && value(rhs_ref).length() > 0 ) ? 1 : 0,ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			}


			if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
			{
				Int lhs_ref = *dynamic_cast<Int*>(lhs);
				Int rhs_ref = *dynamic_cast<Int*>(rhs);
				if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
				{
					throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
				}
				return new Int( value(lhs_ref) != 0 && value(rhs_ref) != 0, unit(lhs_ref), nullptr, nullptr, nullptr);
			}
			if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
			{
				Double lhs_ref = *dynamic_cast<Double*>(lhs);
				Double rhs_ref = *dynamic_cast<Double*>(rhs);
				if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
				{
					throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
				}
				return new Double(value(lhs_ref) != 0 && value(rhs_ref) != 0, unit(lhs_ref), nullptr, nullptr, nullptr);
			}
		}// Addition

	if (op == '|')
			{
				if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
				{
					String lhs_ref = *dynamic_cast<String*>(lhs);
					String rhs_ref = *dynamic_cast<String*>(rhs);
					return new Int( ( (value(lhs_ref).length() + value(rhs_ref).length()) > 0 ) ? 1 : 0,ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
				}


				if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
				{
					Int lhs_ref = *dynamic_cast<Int*>(lhs);
					Int rhs_ref = *dynamic_cast<Int*>(rhs);
					if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
					{
						throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
					}
					return new Int( value(lhs_ref) != 0 || value(rhs_ref) != 0, unit(lhs_ref), nullptr, nullptr, nullptr);
				}
				if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
				{
					Double lhs_ref = *dynamic_cast<Double*>(lhs);
					Double rhs_ref = *dynamic_cast<Double*>(rhs);
					if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
					{
						throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
					}
					return new Double(value(lhs_ref) != 0 || value(rhs_ref) != 0, unit(lhs_ref), nullptr, nullptr, nullptr);
				}
			}// Addition


	/** Subtraction **/
	if (op == '-')
	{
		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			throw semantic_exception{binop_node," Can't (yet) subtract strings."};
		}

		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			throw semantic_exception{binop_node," Invalid operation."};
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			throw semantic_exception{binop_node," Invalid operation."};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '-': Incompatible units."};
			}
			return new Int(value(lhs_ref) - value(rhs_ref), unit(lhs_ref), nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '-': Incompatible units."};
			}
			return new Double(value(lhs_ref) - value(rhs_ref), unit(lhs_ref), nullptr, nullptr, nullptr);
		}
	}// Subtraction


	/****** Multiplication *******/
	if (op == '*')
	{
		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			throw semantic_exception{binop_node," Can't (yet) multiply strings."};
		}

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::int_literal)
		{
			int j = value(*dynamic_cast<Int*>(rhs));
			if (j <= 0)
				return new String("", nullptr, nullptr, nullptr);
			std::string buffer;
			std::string & s = value(*dynamic_cast<String*>(lhs));
			for(int i = 0; i < j;++i)
				buffer+= s;
			return new String(buffer, nullptr, nullptr, nullptr);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::string_literal)
		{
			int j = value(*dynamic_cast<Int*>(lhs));
			if (j <= 0)
				return new String("", nullptr, nullptr, nullptr);
			std::string buffer;
			std::string & s = value(*dynamic_cast<String*>(rhs));
			for(int i = 0; i < j;++i)
				buffer+= s;
			return new String(buffer, nullptr, nullptr, nullptr);
		}

		if (
				(lhs->kind() == Kind::string_literal && rhs->kind() == Kind::float_literal)
				|| (rhs->kind() == Kind::string_literal && lhs->kind() == Kind::float_literal)
			)
		{
			throw semantic_exception{binop_node," Can't (yet) multiply strings with floats."};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			return new Int(
								value(lhs_ref)*value(rhs_ref), //multiply values
								unit(lhs_ref)+unit(rhs_ref) // add units
								, nullptr, nullptr, nullptr
			);
		}
		if (lhs->kind() == Kind::long_literal && rhs->kind() == Kind::long_literal)
		{
			auto lhs_ref = *static_cast<Int64*>(lhs);
			auto rhs_ref = *static_cast<Int64*>(rhs);
			return mk_int64_node(
								value(lhs_ref)*value(rhs_ref), //multiply values
								unit(lhs_ref)+unit(rhs_ref) // add units								
			);
		}
		if (lhs->kind() == Kind::long_literal && rhs->kind() == Kind::int_literal)
		{
			auto lhs_ref = *static_cast<Int64*>(lhs);
			auto rhs_ref = *static_cast<Int*>(rhs);
			return mk_int64_node(
								value(lhs_ref)*value(rhs_ref), //multiply values
								unit(lhs_ref)+unit(rhs_ref) // add units								
			);
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::long_literal)
		{
			auto lhs_ref = *static_cast<Int*>(lhs);
			auto rhs_ref = *static_cast<Int64*>(rhs);
			return mk_int64_node(
								value(lhs_ref)*value(rhs_ref), //multiply values
								unit(lhs_ref)+unit(rhs_ref) // add units								
			);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			return new Double(value(lhs_ref)*value(rhs_ref), unit(lhs_ref) + unit(rhs_ref), nullptr, nullptr, nullptr);
		}
	}// Multiplication

	/****** Division *******/
	if (op == '/')
	{
		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			throw semantic_exception{binop_node," Can't divide strings."};
		}

		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::int_literal)
		{
			throw semantic_exception{binop_node," Can't divide strings."};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::string_literal)
		{
			throw semantic_exception{binop_node," Can't divide strings."};
		}

		if (
				(lhs->kind() == Kind::string_literal && rhs->kind() == Kind::float_literal)
				|| (rhs->kind() == Kind::string_literal && lhs->kind() == Kind::float_literal)
			)
		{
			throw semantic_exception{binop_node," Can't (yet) divide strings."};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			return new Int(
								value(lhs_ref)/value(rhs_ref), //multiply values
								unit(lhs_ref)-unit(rhs_ref) // add units
								, nullptr, nullptr, nullptr
			);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			return new Double(value(lhs_ref) / value(rhs_ref), unit(lhs_ref) - unit(rhs_ref), nullptr, nullptr, nullptr);
		}
	}// Division
	/****** Power *******/
	if (op == '^')
	{
		if ( (lhs->kind() == Kind::string_literal || rhs->kind() == Kind::string_literal))
		{
			throw semantic_exception{binop_node," Incompatible operands."};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if (unit(rhs_ref) != ceps::ast::all_zero_unit())
			{
				throw semantic_exception{binop_node," Incompatible operands. rhs operand in ^ - operation has to be a scalar."};
			}
			int lhs_v = value(lhs_ref);
			int rhs_v = value(rhs_ref);
			if (rhs_v >= 0)
			{
				int result = 1;
				if (rhs_v > 0)
				{
					for(int j = 0; j< rhs_v;++j)
						result *= lhs_v;
				}
				return new Int(
								result,
								rhs_v*unit(lhs_ref)
								, nullptr, nullptr, nullptr
				);
			}
			else return new Double(
				std::pow((double)lhs_v,rhs_v),
				rhs_v*unit(lhs_ref), nullptr, nullptr, nullptr
			);
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::int_literal)
		{
			Double & lhs_ref = *dynamic_cast<Double*>(lhs);
			Int & rhs_ref = *dynamic_cast<Int*>(rhs);
			if (unit(rhs_ref) != ceps::ast::all_zero_unit())
			{
				throw semantic_exception{binop_node," XXX Incompatible operands. rhs operand in ^ - operation has to be a scalar."};
			}
			int rhs_v = value(rhs_ref);
			double lhs_v = value(lhs_ref);

			return new Double(
								std::pow(lhs_v,(double)rhs_v),
								rhs_v*unit(lhs_ref), nullptr, nullptr, nullptr
				);
		}

		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if (unit(rhs_ref) != ceps::ast::all_zero_unit() || unit(lhs_ref) != ceps::ast::all_zero_unit() )
			{
				throw semantic_exception{binop_node," Incompatible operands. floating point operands in ^ - operation have to be scalar."};
			}

			return new Double(std::pow(value(lhs_ref), value(rhs_ref)), unit(lhs_ref), nullptr, nullptr, nullptr);
		}
	}// Power
	return mk_bin_op(op,lhs,rhs,ceps::ast::op_str(ceps::ast::as_binop_ref(binop_node)));
}

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate_nonleaf(ceps::ast::Nonleafbase& root,
		                                            ceps::parser_env::Symboltable & sym_table,
		                                            ceps::interpreter::Environment& env, 
													ceps::ast::Nodebase_ptr parent_node,
													ceps::ast::Nodebase_ptr predecessor,
										 			ceps::ast::Nodebase_ptr this_ptr)
{
	using namespace ceps::ast;
	Nonleafbase::Container_t v;

    auto old_scope = env.scope;
    env.scope = &v;
	Nodebase* root_ptr = dynamic_cast<Nodebase*>(&root);
	for(Nodebase_ptr p : root.children())
	{
		Nodebase_ptr r = evaluate_generic(p,sym_table,env,(Nodebase_ptr)&root,predecessor,this_ptr);predecessor=r;
		if(r == nullptr)
			continue;
		if (r->kind() == Ast_node_kind::stmts || r->kind() == Ast_node_kind::nodeset)
		{
			for(Nodebase_ptr elem : nlf_ptr(r)->children())
			{
				v.push_back(elem);
			}
		}
		else v.push_back(r);
	}
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

