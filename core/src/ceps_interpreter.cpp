/**
 The MIT License (MIT)

Copyright (c) 2014 The authors of ceps

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 **/


#include "ceps_interpreter.hh"
#include "symtab.hh"
#include <cmath>
#include "ceps_interpreter_loop.hh"
#include "ceps_interpreter_nodeset.hh"


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

	for(auto p : root->children())
	{
		if (p->kind() == Kind::stmts)
		{
			for(auto pp: as_stmts_ptr(p)->children() )
			{
				auto ev = evaluate(pp,sym_table,env,p);
				if (ev  != nullptr && ev->kind() == Kind::nodeset)
				{
					universe.nodes().insert(universe.nodes().end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
					if (generated_nodes != nullptr)
						generated_nodes->insert(generated_nodes->end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());

				}
				else if (ev != nullptr)
				{
					universe.nodes().push_back(ev);
					if (generated_nodes != nullptr) generated_nodes->push_back(ev);
				}
			}//for
			continue;
		}
		auto ev = evaluate(p,sym_table,env,p);
		if (ev != nullptr && ev->kind() == Kind::nodeset)
		{
			universe.nodes().insert(universe.nodes().end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
			if (generated_nodes != nullptr)
									generated_nodes->insert(generated_nodes->end(),as_ast_nodeset_ptr(ev)->children().begin(),as_ast_nodeset_ptr(ev)->children().end());
		}
		else if (ev != nullptr) {
			universe.nodes().push_back(ev);
			if (generated_nodes != nullptr) generated_nodes->push_back(ev);
		}
	}//for
}//ceps::interpreter::evaluate


ceps::ast::Nodebase_ptr ceps::interpreter::evaluate(ceps::ast::Nodebase_ptr root_node,
													  ceps::parser_env::Symboltable & sym_table,
													  ceps::interpreter::Environment& env,
													  ceps::ast::Nodebase_ptr parent_node)
 {
	 if(!root_node)
		 return nullptr;

	 switch(root_node->kind())
	 {
	 case Kind::kind_def:
	 {
		 using namespace ceps::parser_env;
		 ceps::ast::Kinddef& kind_def_node = *dynamic_cast<ceps::ast::Kinddef*>(root_node);


		 ceps::parser_env::Symbol* sym_kind_ptr =
				sym_table.lookup(ceps::ast::get<0>(kind_def_node),true);
		 sym_kind_ptr->category = Symbol::KIND;
		 for (ceps::ast::Nodebase* pnbs : kind_def_node.children())
		 {
			 ceps::ast::Identifier & v = ceps::ast::as_id_ref(pnbs);

			 ceps::parser_env::Symbol* sym_ptr =
			 				sym_table.lookup(ceps::ast::name(v),true);
			 sym_ptr->category = Symbol::SYMBOL;
			 ceps::parser_env::Symbol* copy_of_sym_kind_ptr = new ceps::parser_env::Symbol(Symbol::KIND);
			 copy_of_sym_kind_ptr->name = sym_kind_ptr->name;
			 copy_of_sym_kind_ptr->payload = nullptr;

			 sym_ptr->payload = copy_of_sym_kind_ptr;
		 }
		 return nullptr;
	 }
	 case Kind::valdef:
	 {
		 using namespace ceps::parser_env;
		 ceps::ast::Valdef& val_node = *dynamic_cast<ceps::ast::Valdef*>(root_node);
		 ceps::parser_env::Symbol* sym_ptr;

		 ceps::ast::Nodebase_ptr rhs = evaluate(dynamic_cast<ceps::ast::Nonleafbase*>(root_node)->children()[0],sym_table,env,root_node);

		 if ( (sym_ptr = sym_table.lookup(name(val_node),true,true,false)) == nullptr)
		 {
			 throw semantic_exception{root_node,"Couldn't define Variable '" +name(val_node)+"' already defined."};
		 }


		 ceps::parser_env::Symbol& sym = *sym_ptr;
		 sym.category = Symbol::Category::VAR;
		 sym.payload = rhs;//TODO: See comment in symtab.hh

		 return nullptr;//Called because of side effect, no return value. Variable definitions end up in the symbol table and disappear from the tree.
	 }
	 case Kind::stmts:
	 case Kind::root:
	 case Kind::stmt:
	 case Kind::call_parameters:
		 return evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node);
	 case Kind::scope:
	 {
		 sym_table.push_scope();
	 	 auto r = evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node);
	 	 sym_table.pop_scope();
	 	 return r;
	 }
	 case Kind::structdef:
	 {
		sym_table.push_scope();
		auto result = evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node);
		sym_table.pop_scope();
		return result;
	 }
	 case Kind::expr:
	 {
		ceps::ast::Nonleafbase& nleaf = *dynamic_cast<ceps::ast::Nonleafbase*>(root_node);


		if ( nleaf.children().size() != 1)
			{
				CEPSERROR("Internal error: Expression invalid.")
			}

		ceps::ast::Nodebase_ptr result = evaluate(nleaf.children()[0],sym_table,env,root_node);
		//nleaf.children().clear();
		return result;
	 }
	 case Kind::unary_operator:
	 {
		 ceps::ast::Unary_operator& unop = *dynamic_cast<ceps::ast::Unary_operator*>(root_node);
		 if (op(unop) == '-')
		 {
			 ceps::ast::Nodebase_ptr operand = evaluate(unop.children()[0],sym_table,env,root_node);
			 if (operand->kind() == Kind::int_literal)
			 {
				 value(as_int_ref(operand)) *= -1;
			 }
			 else if (operand->kind() == Kind::float_literal)
			 {
				 value(as_double_ref(operand)) *= -1.0;
			 }

			 return operand;
		 }
		 else if (op(unop) == '!')
		 {
			 ceps::ast::Nodebase_ptr operand = evaluate(unop.children()[0],sym_table,env,root_node);
			 if (operand->kind() == Kind::int_literal)
			 {
			 	if ( value(as_int_ref(operand)) == 0)
			 		return new ceps::ast::Int(1,ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			 	else return new ceps::ast::Int(0,ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
			 } else if (operand->kind() == Kind::symbol ||
					    operand->kind() == Kind::binary_operator ||
					    operand->kind() == Kind::unary_operator ||
					    operand->kind() == Kind::func_call
					    )
			 {
				 return new ceps::ast::Unary_operator(op(unop),operand);
			 }
		 }
		 throw semantic_exception{root_node,"Illformed unary operator expression"};
	 }
	 case Kind::ifelse:
	 {
		 ceps::ast::Ifelse& ifelse = ceps::ast::as_ifelse_ref(root_node);
		 ceps::ast::Nodebase_ptr cond = evaluate(ifelse.children()[0],sym_table,env,root_node);
		 ceps::ast::Nodebase_ptr left_branch = nullptr,right_branch=nullptr;
		 if (ifelse.children().size() > 1) left_branch = evaluate(ifelse.children()[1],sym_table,env,root_node);
		 if (ifelse.children().size() > 2) right_branch = evaluate(ifelse.children()[2],sym_table,env,root_node);
		 return new ceps::ast::Ifelse(cond,left_branch,right_branch);


		 /*if (cond == nullptr || cond->kind() == ceps::ast::Ast_node_kind::int_literal || cond->kind() == ceps::ast::Ast_node_kind::float_literal)
		 {
			 // We can choose one branch right now
			 bool b = true;
			 if (cond == nullptr) b = false;
			 else if ( cond->kind() == ceps::ast::Ast_node_kind::int_literal ) b = ceps::ast::value(ceps::ast::as_int_ref(cond)) != 0;
			 else b = ceps::ast::value(ceps::ast::as_double_ref(cond)) != 0;

			 if(b && ifelse.children().size()>1 && ifelse.children()[1] != nullptr)
			  {auto r = evaluate(ifelse.children()[1],sym_table,env);return r;}
			 else if (!b && ifelse.children().size()>2&& ifelse.children()[2] != nullptr)
			 {auto r = evaluate(ifelse.children()[2],sym_table,env);return r;}
			 return nullptr;
		 } else{
			 // There are symbols involved: postpone selection of branch to upper level
		 }*/
	 }
	 case Kind::func_call:
	 {
		 ceps::ast::Func_call& func_call = *dynamic_cast<ceps::ast::Func_call*>(root_node);

		 if (func_call.children()[0]->kind() == Kind::identifier)
		 {
			 ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(func_call.children()[0]);
			 ceps::ast::Nodebase_ptr params_ = evaluate(func_call.children()[1],sym_table,env,root_node);
			 ceps::ast::Call_parameters& params = *dynamic_cast<ceps::ast::Call_parameters*>(params_);

			 auto rr = env.call_func_callback(ceps::ast::name(id),&params);
			 if (rr != nullptr) return rr;
			 if (name(id) == "sin")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::sin(value(arg)),ceps::ast::all_zero_unit(),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::sin((double)value(arg)), ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id),nullptr,nullptr,nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }

			 } else 	if (name(id) == "abs")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::abs(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::abs((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }

			 } else	 if (name(id) == "cos")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::cos(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::cos((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }
			 }
			 if (name(id) == "tan")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::tan(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::tan((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }
			 }
			 if (name(id) == "atan")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::atan(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::atan((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }
			 }

			 if (name(id) == "acos")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::acos(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::acos((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }
			 }

			 if (name(id) == "asin")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::asin(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::asin((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }
			 }

			 if (name(id) == "ln" || name(id) == "log")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];

				 auto arg = evaluate(arg_,sym_table,env,root_node);



				 if (arg->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double(std::log(value(arg)),unit(arg),nullptr,nullptr,nullptr);
				 }
				 else if (arg->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double(std::log((double)value(arg)), unit(arg), nullptr, nullptr, nullptr);
				 }
				 else
				 {
					 auto fp = new ceps::ast::Call_parameters();
					 fp->children().push_back(arg);
					 ceps::ast::Func_call* f = new ceps::ast::Func_call();
					 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
					 f->children_.push_back(fp);
					 return f;
				 }
			 }
			 if (name(id) == "value" )
			 {
				 //std::cout << ceps::ast::Nodeset{&params} << std::endl;
				 //std::cout << params.children().size()  << std::endl;
				 /*if (params.children().size() != 1)
					 throw semantic_exception{root_node,"value: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (!is_a_nodeset(arg_))
					 throw semantic_exception{root_node,"value: wrong argument type, expecting a node set."};*/

				 std::vector<ceps::ast::Nodebase_ptr> result;
				 for ( auto & param : params.children() )
				 {
					 if (!ceps::ast::is_a_struct(param)) continue;
					 for(auto & child: as_struct_ref(param).children())
						 result.push_back(child);
				 }
				 return create_ast_nodeset( "", result);
			 }

			 ceps::ast::Func_call* f = new ceps::ast::Func_call();
			 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
			 f->children_.push_back(params_);
 			 return f;
		 }
	 }
	 case Kind::binary_operator:
	 {
		ceps::ast::Binary_operator& binop = *dynamic_cast<ceps::ast::Binary_operator*>(root_node);
		if (binop.children().size() != 2)
		{
			throw semantic_exception{root_node,"Expecting 2 arguments, given " +mk_string(binop.children().size())};
		}
		ceps::ast::Nodebase_ptr result;


		if (op(binop) != '=')
		{
		 ceps::ast::Nodebase_ptr lhs = evaluate(binop.children()[0],sym_table,env,root_node);
		 ceps::ast::Nodebase_ptr rhs = evaluate(binop.children()[1],sym_table,env,root_node);


		 if (lhs->kind() == ceps::ast::Ast_node_kind::symbol
			 || rhs->kind() == ceps::ast::Ast_node_kind::symbol
			 || lhs->kind() == ceps::ast::Ast_node_kind::binary_operator
			 || rhs->kind() == ceps::ast::Ast_node_kind::binary_operator
			 || lhs->kind() == ceps::ast::Ast_node_kind::func_call
			 || rhs->kind() == ceps::ast::Ast_node_kind::func_call)
		 {
			 auto override_value = env.call_binop_resolver(&binop,lhs,rhs,parent_node);
			 if (override_value) return override_value;
			 ceps::ast::Binary_operator* t = new ceps::ast::Binary_operator( ceps::ast::op(binop) , nullptr,nullptr,nullptr );
			 t->children().push_back(lhs);
			 t->children().push_back(rhs);
			 return t;
		 }
		 result = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table,env,root_node);
		}
		else
		{
			 ceps::ast::Nodebase_ptr lhs = binop.children()[0];
			 //if (lhs->kind() != ceps::ast::Ast_node_kind::identifier)
			 lhs = evaluate(binop.children()[0],sym_table,env,root_node);
			 ceps::ast::Nodebase_ptr rhs = evaluate(binop.children()[1],sym_table,env,root_node);
			 if( lhs->kind() == ceps::ast::Ast_node_kind::symbol ||
			     lhs->kind() == ceps::ast::Ast_node_kind::binary_operator ||
			     lhs->kind() == ceps::ast::Ast_node_kind::unary_operator ||
			     lhs->kind() == ceps::ast::Ast_node_kind::func_call )
			 {
				 ceps::ast::Binary_operator* t = new ceps::ast::Binary_operator( ceps::ast::op(binop) , nullptr,nullptr,nullptr );
				 t->children().push_back(lhs);
				 t->children().push_back(rhs);
				 return t;
			 }
			 else result  = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table,env,root_node);
		}
		return result;
	 }
	 case Kind::identifier:
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
		else if (name(id) == "root" && env.associated_universe() != nullptr)
			return create_ast_nodeset("", env.associated_universe()->nodes());

		//OK, let's take a look into the symboltable

		 ceps::parser_env::Symbol* sym_ptr;

		 if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr)
		 {
			 //throw semantic_exception{root_node,"Variable '" +name(id)+"' is not defined."};
			 std::string id_name = name(id);
			 return new ceps::ast::Identifier(id_name,nullptr,nullptr,nullptr);
		 }

		 if (sym_ptr->category == Symbol::Category::SYMBOL)
		 {
			 return new ceps::ast::Symbol(name(id), ((ceps::parser_env::Symbol*)sym_ptr->payload)->name, nullptr, nullptr, nullptr);
		 }


		 ceps::parser_env::Symbol& sym = *sym_ptr;
		 if (sym.category != Symbol::Category::VAR)
			 throw semantic_exception{root_node,"Variable '" +name(id)+"' is not defined."};

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
			 for (Nodebase_ptr pp : atoms_p_src->children())
			 {
				 Nodebase_ptr r = evaluate(pp,sym_table,env,root_node);
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
		 return evaluate_loop(as_loop_ptr(root_node),
				  	  	  	  sym_table,
				  	  	  	  env,root_node);
	 }
	 case Kind::symbol:
	 {
		 std::string kind = ceps::ast::kind(ceps::ast::as_symbol_ref(root_node));
		 std::string name = ceps::ast::name(ceps::ast::as_symbol_ref(root_node));
		 if (env.symbol_mapping()[kind] != nullptr)
		 {
			 auto r = (*env.symbol_mapping()[kind])[name];
			 if (r != nullptr) return evaluate(r,sym_table, env,root_node );
		 }
		 return root_node;
	 }
	 case Kind::ret:
	 {
		 auto& ret_stmt = ceps::ast::as_return_ref(root_node);
		 ceps::ast::Nodebase_ptr operand = evaluate(ret_stmt.children()[0],sym_table,env,(ceps::ast::Nodebase_ptr)&ret_stmt);
		 return new ceps::ast::Return(operand);
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
															ceps::ast::Nodebase_ptr parent_node
															)
{
	using namespace ceps::ast;
	if (lhs == nullptr || rhs == nullptr)
		{ CEPSERROR("Internal error: Operand(s) evaluates to null.") }

	if (op == '=')
	{
		using namespace ceps::parser_env;
		if (lhs->kind() != Kind::identifier)
			throw semantic_exception{binop_node," Left hand side of assignment should be a variable"};


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
		Double_ptr temp = new Double(static_cast<double>(value(as_int_ref(rhs))), unit(as_int_ref(rhs)), nullptr, nullptr, nullptr);
		rhs = temp;
	}
	else if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::float_literal)
	{
		Double_ptr temp = new Double( static_cast<double>(value(as_int_ref(lhs))), unit(as_int_ref(lhs)), nullptr, nullptr, nullptr);
		lhs = temp;
	}

	if (op == ceps::Cepsparser::token::REL_OP_EQ)
	{
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
	if(op == '.')
	{
		if(lhs->kind() == Kind::nodeset)
			return evaluate_nodeset_expr_dot(	lhs,
												rhs ,
												sym_table,
												env,binop_node
											);
	}

	return new Binary_operator{op,lhs,rhs};


	return nullptr;
}

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate(ceps::ast::Nonleafbase& root,
		                                              ceps::parser_env::Symboltable & sym_table,
		                                              ceps::interpreter::Environment& env, ceps::ast::Nodebase_ptr parent_node)
{
	using namespace ceps::ast;
	Nonleafbase::Container_t v;
	for(Nodebase_ptr p : root.children())
	{

		Nodebase_ptr r = evaluate(p,sym_table,env,(Nodebase_ptr)&root);
		if(r == nullptr)
		{
			continue;
		}
		if (r->kind() == Ast_node_kind::stmts || r->kind() == Ast_node_kind::scope || r->kind() == Ast_node_kind::nodeset)
		{
			for(Nodebase_ptr elem : nlf_ptr(r)->children())
			{
				v.push_back(elem);
			}
		}
		else v.push_back(r);
	}
	Nodebase* root_ptr = dynamic_cast<Nodebase*>(&root);
	if (root_ptr->kind() == ceps::ast::Ast_node_kind::root)
	{
		Root* result = new Root{};
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::stmts)
	{
		Stmts* result = new Stmts{};
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::stmt)
	{
		Stmt* result = new Stmt();
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::structdef)
	{
		ceps::ast::Struct& s = *dynamic_cast<ceps::ast::Struct*>(&root);
		Struct* result = new Struct(name(s), nullptr, nullptr, nullptr);
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::scope)
	{
		Scope* result = new Scope();
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::call_parameters)
	{
		Call_parameters* result = new Call_parameters();
		result->children() = v;
		return result;
	}

	CEPSERROR("Internal Error.")

}

