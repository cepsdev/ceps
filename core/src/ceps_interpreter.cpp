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





ceps::ast::Nodebase_ptr ceps::interpreter::evaluate(ceps::ast::Nodebase_ptr root_node,
													  ceps::parser_env::Symboltable & sym_table,
													  ceps::interpreter::Environment& env)
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
			 sym_ptr->payload = sym_kind_ptr;
		 }
		 return nullptr;
	 }
	 case Kind::valdef:
	 {
		 using namespace ceps::parser_env;
		 ceps::ast::Valdef& val_node = *dynamic_cast<ceps::ast::Valdef*>(root_node);
		 ceps::parser_env::Symbol* sym_ptr;

		 ceps::ast::Nodebase_ptr rhs = evaluate(dynamic_cast<ceps::ast::Nonleafbase*>(root_node)->children()[0],sym_table,env);

		 if ( (sym_ptr = sym_table.lookup(name(val_node),true,true,true)) == nullptr)
		 {
			 throw semantic_exception{root_node,"Variable '" +name(val_node)+"' already defined."};
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
		 return evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env);
	 case Kind::scope:
		 sym_table.push_scope();
	 	 return evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env);
	 	 sym_table.pop_scope();
	 case Kind::structdef:
	 {
		sym_table.push_scope();
		auto result = evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env);
		sym_table.pop_scope();
		return result;
	 }
	 case Kind::expr:
	 {
		ceps::ast::Nonleafbase& nleaf = *dynamic_cast<ceps::ast::Nonleafbase*>(root_node);


		if ( nleaf.children().size() != 1)
			{
				ERROR("Internal error: Expression invalid.")
			}

		ceps::ast::Nodebase_ptr result = evaluate(nleaf.children()[0],sym_table,env);
		//nleaf.children().clear();
		return result;
	 }
	 case Kind::unary_operator:
	 {
		 ceps::ast::Unary_operator& unop = *dynamic_cast<ceps::ast::Unary_operator*>(root_node);
		 if (op(unop) == '-')
		 {
			 ceps::ast::Nodebase_ptr operand = evaluate(unop.children()[0],sym_table,env);
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
		 throw semantic_exception{root_node,"Unsupported unary operator "};
	 }
	 case Kind::func_call:
	 {
		 ceps::ast::Func_call& func_call = *dynamic_cast<ceps::ast::Func_call*>(root_node);

		 if (func_call.children()[0]->kind() == Kind::identifier)
		 {
			 ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(func_call.children()[0]);
			 ceps::ast::Nodebase_ptr params_ = evaluate(func_call.children()[1],sym_table,env);
			 ceps::ast::Call_parameters& params = *dynamic_cast<ceps::ast::Call_parameters*>(params_);
			 if (name(id) == "sin")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::sin(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::sin((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"sin: Expecting numerical value as argument"};

			 }
			 if (name(id) == "cos")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"cos: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::cos(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::cos((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"cos: Expecting numerical value as argument"};
			 }
			 if (name(id) == "tan")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"tan: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::tan(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::tan((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"tan: Expecting numerical value as argument"};
			 }
			 if (name(id) == "atan")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"atan: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::atan(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::atan((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"atan: Expecting numerical value as argument"};
			 }

			 if (name(id) == "acos")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"acos: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::acos(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::acos((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"acos: Expecting numerical value as argument"};
			 }

			 if (name(id) == "asin")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"asin: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::asin(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::asin((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"asin: Expecting numerical value as argument"};
			 }

			 if (name(id) == "ln" || name(id) == "log")
			 {
				 if (params.children().size() != 1)
					 throw semantic_exception{root_node,"ln: Expecting 1 argument"};
				 ceps::ast::Nodebase_ptr arg_ = params.children()[0];
				 if (arg_->kind() == Kind::float_literal)
				 {
					 ceps::ast::Double& arg = *dynamic_cast<ceps::ast::Double*>(arg_);
					 return new ceps::ast::Double{std::log(value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else if (arg_->kind() == Kind::int_literal)
				 {
					 ceps::ast::Int& arg = *dynamic_cast<ceps::ast::Int*>(arg_);
					 return new ceps::ast::Double{std::log((double)value(arg)),ceps:ast::all_zero_unit()};
				 }
				 else
					 throw semantic_exception{root_node,"ln: Expecting numerical value as argument"};
			 }

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
		 ceps::ast::Nodebase_ptr lhs = evaluate(binop.children()[0],sym_table,env);
		 ceps::ast::Nodebase_ptr rhs = evaluate(binop.children()[1],sym_table,env);


		 if (lhs->kind() == ceps::ast::Ast_node_kind::symbol
			 || rhs->kind() == ceps::ast::Ast_node_kind::symbol
			 || lhs->kind() == ceps::ast::Ast_node_kind::binary_operator
			 || rhs->kind() == ceps::ast::Ast_node_kind::binary_operator)
		 {
			 if (lhs->kind() == ceps::ast::Ast_node_kind::symbol &&
				 rhs->kind() == ceps::ast::Ast_node_kind::symbol)
			 {
				 ceps::ast::Symbol& lhs_symbol = *dynamic_cast<ceps::ast::Symbol*>(lhs);
				 ceps::ast::Symbol& rhs_symbol = *dynamic_cast<ceps::ast::Symbol*>(rhs);
				// std::cout << op(binop) << "  '" << ceps::ast::get<1>(lhs_symbol)<< "' '" << ceps::ast::get<1>(rhs_symbol) << "'" << std::endl;
				 auto binary_op = env.get_glbl_binop_overload(op(binop),ceps::ast::get<1>(lhs_symbol),ceps::ast::get<1>(rhs_symbol));

				 if (binary_op != nullptr)
				 {
					 return binary_op(lhs,rhs);
				 }
			 }
			 else if (lhs->kind() == ceps::ast::Ast_node_kind::symbol)
			 {
				 ceps::ast::Symbol& lhs_symbol = *dynamic_cast<ceps::ast::Symbol*>(lhs);
				 if (rhs->kind() == ceps::ast::Ast_node_kind::float_literal)
				 {
					 auto binary_op = env.get_glbl_binop_overload(op(binop),ceps::ast::get<1>(lhs_symbol),"@FLOAT");
					 if (binary_op != nullptr)
					 {
						 return binary_op(lhs,rhs);
					 }
				 }
				 else throw semantic_exception{root_node," Tree Rewrites not fully supported. "};
			 }


			 auto t = new ceps::ast::Binary_operator{ceps::ast::op(binop)};
			 t->children().push_back(lhs);
			 t->children().push_back(rhs);
			 return t;
		 }
		 result = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table);
		}
		else
		{
			 ceps::ast::Nodebase_ptr lhs = binop.children()[0];
			 if (lhs->kind() != ceps::ast::Ast_node_kind::identifier)
				 lhs = evaluate(binop.children()[0],sym_table,env);
			 ceps::ast::Nodebase_ptr rhs = evaluate(binop.children()[1],sym_table,env);
			 result  = handle_binop(root_node,ceps::ast::op(binop),lhs,rhs,sym_table);
		}
		return result;
	 }
	 case Kind::identifier:
	 {
		using namespace ceps::parser_env;

		ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(root_node);
		if (name(id) == "m" || name(id) == "metre" || name(id) == "meter")
		{
 			return new ceps::ast::Int{1,ceps::ast::m_unit()};
		}
		else if (name(id) == "s" || name(id) == "second")
		{
 			return new ceps::ast::Int{1,ceps::ast::s_unit()};
		}
		else if (name(id) == "kg" || name(id) == "kilogram")
		{
 			return new ceps::ast::Int{1,ceps::ast::kg_unit()};
		}
		else if (name(id) == "celsius" || name(id) == "kelvin")
		{
 			return new ceps::ast::Int{1,ceps::ast::kelvin_unit()};
		}
		else if (name(id) == "ampere" )
		{
 			return new ceps::ast::Int{1,ceps::ast::ampere_unit()};
		}
		else if (name(id) == "cd" || name(id) == "candela" )
		{
 			return new ceps::ast::Int{1,ceps::ast::candela_unit()};
		}
		else if (name(id) == "mol" || name(id) == "mole" )
		{
 			return new ceps::ast::Int{1,ceps::ast::mol_unit()};
		}

		//OK, let's take a look into the symboltable

		 ceps::parser_env::Symbol* sym_ptr;

		 if ( (sym_ptr = sym_table.lookup(name(id))) == nullptr)
		 {
			 throw semantic_exception{root_node,"Variable '" +name(id)+"' is not defined."};
		 }

		 if (sym_ptr->category == Symbol::Category::SYMBOL)
		 {
			 return new ceps::ast::Symbol{name(id), ((ceps::parser_env::Symbol*)sym_ptr->payload)->name };
		 }


		 ceps::parser_env::Symbol& sym = *sym_ptr;
		 if (sym.category != Symbol::Category::VAR)
			 throw semantic_exception{root_node,"Variable '" +name(id)+"' is not defined."};

		 ceps::ast::Nodebase_ptr node_ptr = reinterpret_cast<ceps::ast::Nodebase_ptr>(sym.payload);

		 if (node_ptr->kind() == ceps::ast::Ast_node_kind::float_literal)
		 {
			 ceps::ast::Double & v = *dynamic_cast<ceps::ast::Double*>(node_ptr);
			 return new ceps::ast::Double{value(v),unit(v)};
		 }
		 if (node_ptr->kind() == ceps::ast::Ast_node_kind::int_literal)
		 {
			 ceps::ast::Int & v = *dynamic_cast<ceps::ast::Int*>(node_ptr);
			 return new ceps::ast::Int{value(v),unit(v)};
		 }
		 if (node_ptr->kind() == ceps::ast::Ast_node_kind::string_literal)
		 {
			 ceps::ast::String & v = *dynamic_cast<ceps::ast::String*>(node_ptr);
			 return new ceps::ast::String{value(v)};
		 }
		 if (node_ptr->kind() == ceps::ast::Ast_node_kind::symbol)
		 {
		 	 auto & v = as_symbol_ref(node_ptr);
		 	 //std::cerr << v << std::endl;
		 	return new ceps::ast::Symbol{name(v),kind(v)};

		 }
		 throw semantic_exception{root_node,"Variable '" + name(as_id_ref(root_node))+"' of unknown type."};
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
				 Nodebase_ptr r = evaluate(pp,sym_table,env);
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
		 return new Double{value(v),unit(v)};
	 }
	 case Kind::int_literal:
	 {
		 using namespace ceps::ast;
		 Int & v = as_int_ref(root_node);
		 return new Int{value(v),unit(v)};
	 }
	 case Kind::string_literal:
	 {
		 using namespace ceps::ast;
		 String & v = as_string_ref(root_node);
		 return new String{value(v)};
	 }
	 case Kind::loop:
	 {
		 return evaluate_loop(as_loop_ptr(root_node),
				  	  	  	  sym_table,
				  	  	  	  env);
	 }
	 default:
		 ERROR("Internal error: Kind of node unknown.")
	 }
	 return nullptr;
 }
ceps::ast::Nodebase_ptr ceps::interpreter::handle_binop(	ceps::ast::Nodebase_ptr binop_node,
															char op,
															ceps::ast::Nodebase_ptr lhs,
															ceps::ast::Nodebase_ptr rhs,
															ceps::parser_env::Symboltable & sym_table
															)
{
	using namespace ceps::ast;
	if (lhs == nullptr || rhs == nullptr)
		{ ERROR("Internal error: Operand(s) evaluates to null.") }

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
		Double_ptr temp = new Double{static_cast<double>(value(as_int_ref(rhs))),unit(as_int_ref(rhs))};
		rhs = temp;
	}
	else if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::float_literal)
	{
		Double_ptr temp = new Double{static_cast<double>(value(as_int_ref(lhs))),unit(as_int_ref(lhs))};
		lhs = temp;
	}

	/** Addition **/
	if (op == '+')
	{
		if (lhs->kind() == Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String lhs_ref = *dynamic_cast<String*>(lhs);
			String rhs_ref = *dynamic_cast<String*>(rhs);
			return new String{value(lhs_ref)+value(rhs_ref)};
		}

		if (lhs->kind() == Kind::string_literal && rhs->kind() != Kind::string_literal)
		{
			String & lhs_ref = *dynamic_cast<String*>(lhs);
			std::stringstream os;
			rhs->print_value(os);
			return new String{value(lhs_ref)+os.str()};
		}
		if (lhs->kind() != Kind::string_literal && rhs->kind() == Kind::string_literal)
		{
			String & rhs_ref = *dynamic_cast<String*>(rhs);
			std::stringstream os;
			lhs->print_value(os);
			return new String{os.str()+value(rhs_ref)};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::int_literal)
		{
			Int lhs_ref = *dynamic_cast<Int*>(lhs);
			Int rhs_ref = *dynamic_cast<Int*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return new Int{value(lhs_ref)+value(rhs_ref),unit(lhs_ref)};
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '+': Incompatible units."};
			}
			return new Double{value(lhs_ref)+value(rhs_ref),unit(lhs_ref)};
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
			return new Int{value(lhs_ref)-value(rhs_ref),unit(lhs_ref)};
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if ( ceps::ast::unit(lhs_ref) != ceps::ast::unit(rhs_ref) )
			{
				throw semantic_exception{&lhs_ref," Failed to apply '-': Incompatible units."};
			}
			return new Double{value(lhs_ref)-value(rhs_ref),unit(lhs_ref)};
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
				return new String{""};
			std::string buffer;
			std::string & s = value(*dynamic_cast<String*>(lhs));
			for(int i = 0; i < j;++i)
				buffer+= s;
			return new String{buffer};
		}
		if (lhs->kind() == Kind::int_literal && rhs->kind() == Kind::string_literal)
		{
			int j = value(*dynamic_cast<Int*>(lhs));
			if (j <= 0)
				return new String{""};
			std::string buffer;
			std::string & s = value(*dynamic_cast<String*>(rhs));
			for(int i = 0; i < j;++i)
				buffer+= s;
			return new String{buffer};
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
			return new Int{
								value(lhs_ref)*value(rhs_ref), //multiply values
								unit(lhs_ref)+unit(rhs_ref) // add units
			};
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			return new Double{value(lhs_ref)*value(rhs_ref),unit(lhs_ref)+unit(rhs_ref)};
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
			return new Int{
								value(lhs_ref)/value(rhs_ref), //multiply values
								unit(lhs_ref)-unit(rhs_ref) // add units
			};
		}
		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			return new Double{value(lhs_ref)/value(rhs_ref),unit(lhs_ref)-unit(rhs_ref)};
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
				return new Int{
								result,
								rhs_v*unit(lhs_ref)
				};
			}
			else return new Double{
				std::pow((double)lhs_v,rhs_v),
				rhs_v*unit(lhs_ref)
			};
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

			return new Double{
								std::pow(lhs_v,(double)rhs_v),
								rhs_v*unit(lhs_ref)
				};
		}

		if (lhs->kind() == Kind::float_literal && rhs->kind() == Kind::float_literal)
		{
			Double lhs_ref = *dynamic_cast<Double*>(lhs);
			Double rhs_ref = *dynamic_cast<Double*>(rhs);
			if (unit(rhs_ref) != ceps::ast::all_zero_unit() || unit(lhs_ref) != ceps::ast::all_zero_unit() )
			{
				throw semantic_exception{binop_node," Incompatible operands. floating point operands in ^ - operation have to be scalar."};
			}

			return new Double{std::pow(value(lhs_ref),value(rhs_ref)),unit(lhs_ref)};
		}
	}// Power

	return new Binary_operator{op,lhs,rhs};


	return nullptr;
}

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate(ceps::ast::Nonleafbase& root,
		                                              ceps::parser_env::Symboltable & sym_table,
		                                              ceps::interpreter::Environment& env)
{
	using namespace ceps::ast;
	Nonleafbase::Container_t v;
	for(Nodebase_ptr p : root.children())
	{
		Nodebase_ptr r = evaluate(p,sym_table,env);
		if(r == nullptr)
		{
			continue;
		}
		if (r->kind() == Ast_node_kind::stmts || r->kind() == Ast_node_kind::scope)
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
		Stmt* result = new Stmt{};
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::structdef)
	{
		ceps::ast::Struct& s = *dynamic_cast<ceps::ast::Struct*>(&root);
		Struct* result = new Struct{name(s)};
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::scope)
	{
		Scope* result = new Scope{};
		result->children() = v;
		return result;
	}
	else if (root_ptr->kind() == ceps::ast::Ast_node_kind::call_parameters)
	{
		Call_parameters* result = new Call_parameters{};
		result->children() = v;
		return result;
	}

	ERROR("Internal Error.")

}

