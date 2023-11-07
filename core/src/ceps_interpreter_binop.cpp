/*
 (C) 2021 Tomas Prerovsky (cepsdev@hotmail.com).

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
#include <sstream>

std::tuple<bool,ceps::ast::node_t, ceps::ast::node_t,bool> symbolic_equality(ceps::ast::Nodebase_ptr lhs, ceps::ast::Nodebase_ptr rhs){
	using namespace ceps::ast;
	using namespace std;
	
	if (lhs == nullptr || rhs == nullptr) return {false,lhs,rhs,false};

	if (is<Ast_node_kind::int_literal>(lhs) &&  is<Ast_node_kind::int_literal>(rhs) )
	{
		auto& l = as_int_ref(lhs);auto& r = as_int_ref(rhs);
		return {value(l) == value(r) && unit(l) == unit(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::float_literal>(lhs) &&  is<Ast_node_kind::float_literal>(rhs) )
	{
		auto& l = as_double_ref(lhs);auto& r = as_double_ref(rhs);
		return {value(l) == value(r) && unit(l) == unit(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::uint8>(lhs) &&  is<Ast_node_kind::uint8>(rhs) )
	{
		auto& l = as_uint8_ref(lhs);auto& r = as_uint8_ref(rhs);
		return {value(l) == value(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::string_literal>(lhs) &&  is<Ast_node_kind::string_literal>(rhs) )
	{
		auto& l = as_string_ref(lhs);auto& r = as_string_ref(rhs);
		return {value(l) == value(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::long_literal>(lhs) &&  is<Ast_node_kind::long_literal>(rhs) )
	{
		auto& l = as_int64_ref(lhs);auto& r = as_int64_ref(rhs);
		return { value(l) == value(r) && unit(l) == unit(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::unsigned_long_literal>(lhs) &&  is<Ast_node_kind::unsigned_long_literal>(rhs) )
	{
		auto& l = as_uint64_ref(lhs);auto& r = as_uint64_ref(rhs);
		return {value(l) == value(r) && unit(l) == unit(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::symbol>(lhs) &&  is<Ast_node_kind::symbol>(rhs) )
	{
		auto& l = as_symbol_ref(lhs);auto& r = as_symbol_ref(rhs);
		return {name(l) == name(r) && kind(l) == kind(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::identifier>(lhs) &&  is<Ast_node_kind::identifier>(rhs) )
	{
		auto& l = as_id_ref(lhs);auto& r = as_id_ref(rhs);
		return {name(l) == name(r), lhs, rhs,false};
	}
	if (is<Ast_node_kind::undef>(lhs) &&  is<Ast_node_kind::undef>(rhs) ) return {true,lhs,rhs,false};

	if (is<Ast_node_kind::binary_operator>(lhs) &&  is<Ast_node_kind::binary_operator>(rhs) )
	{
		auto& l = as_binop_ref(lhs);auto& r = as_binop_ref(rhs);
		if (op_val(l) != op_val(r)) return {false, lhs, rhs,false};
		auto ll = symbolic_equality(l.left(),r.left());
		if (!get<0>(ll)) return ll;
		auto rr = symbolic_equality(l.right(),r.right());
		if (!get<0>(rr)) return rr;
		return {true,nullptr,nullptr,false};
	}
	if ( (is<Ast_node_kind::scope>(lhs) || is<Ast_node_kind::stmts>(lhs) || is<Ast_node_kind::nodeset>(lhs)) &&
	     (is<Ast_node_kind::scope>(rhs) || is<Ast_node_kind::stmts>(rhs) || is<Ast_node_kind::nodeset>(rhs))   )
	{
		auto& lv = is<Ast_node_kind::scope>(lhs) ? children(as_scope_ref(lhs)) : (is<Ast_node_kind::stmts>(lhs) ? children(as_stmts_ref(lhs)) : children(as_ast_nodeset_ref(lhs)) ); 
		auto& rv = is<Ast_node_kind::scope>(rhs) ? children(as_scope_ref(rhs)) : (is<Ast_node_kind::stmts>(rhs) ? children(as_stmts_ref(rhs)) : children(as_ast_nodeset_ref(rhs)) );
		if (lv.size() != rv.size()) return {false,lhs,rhs,false};
		for(size_t i = 0; i != lv.size(); ++i){
			auto r = symbolic_equality(lv[i],rv[i]);
			if (!get<0>(r)) return r;
		}
		return {true,nullptr,nullptr,false};				
	}
	if (is<Ast_node_kind::structdef>(lhs) && is<Ast_node_kind::structdef>(rhs)){
		auto& l = as_struct_ref(lhs);auto& r = as_struct_ref(rhs);
		if (name(l) != name(r) ) return {false,&l,&r,false};
		auto& lv = children(l);
		auto& rv = children(r);
		if(lv.size() == 1 && is<Ast_node_kind::symbol>(lv[0]) && "SymEqMatcherAny" == kind(as_symbol_ref(lv[0]))) return  {true,nullptr,nullptr,false};
		if(rv.size() == 1 && is<Ast_node_kind::symbol>(rv[0]) && "SymEqMatcherAny" == kind(as_symbol_ref(rv[0]))) return  {true,nullptr,nullptr,false};

		if (lv.size() != rv.size()) return {false,&l,&r,false};
		for(size_t i = 0; i != lv.size(); ++i){
			auto r = symbolic_equality(lv[i],rv[i]);
			if (!get<0>(r)) return r;
		}
		return  {true,nullptr,nullptr,false};					
	} if (is<Ast_node_kind::func_call>(lhs) && is<Ast_node_kind::func_call>(rhs)){
		auto& l = as_func_call_ref(lhs);auto& r = as_func_call_ref(rhs);
		auto ftargetl = children(l)[0];
		auto ftargetr = children(r)[0];
		auto r1 = symbolic_equality(ftargetl,ftargetr);
		if (!get<0>(r1)) return r1;
		auto& paramsl = as_call_params_ref( children(l)[1]);
		auto& paramsr = as_call_params_ref( children(r)[1]);
		if (children(paramsl).size() != children(paramsr).size())
			return{false,lhs,rhs,false};
		auto & vl = children(paramsl);
		auto & vr = children(paramsr);
		for(size_t i = 0; i != vl.size(); ++i){
			auto r = symbolic_equality(vl[i],vr[i]);
			if (!get<0>(r)) return r;
		}
		return {true,nullptr,nullptr,false};
	}
	
	return {false,lhs,rhs,true};
}

std::optional<bool> equality(ceps::ast::Nodebase_ptr lhs, ceps::ast::Nodebase_ptr rhs){
	using namespace std;
	auto r = symbolic_equality(lhs, rhs);
	if (get<3>(r)) return {};
	return get<0>(r);
}  


ceps::ast::Nodebase_ptr ceps::interpreter::handle_binop(	ceps::ast::Nodebase_ptr binop_node,
															int op,
															ceps::ast::Nodebase_ptr lhs,
															ceps::ast::Nodebase_ptr rhs,
															ceps::parser_env::Symboltable & sym_table,
															Environment& env,
															ceps::ast::Nodebase_ptr parent_node,
															ceps::ast::Nodebase_ptr this_ptr,
													        ceps::interpreter::thoroughness_t thoroughness,
															bool& symbols_found
															)
{
	using namespace ceps::ast;
	if (lhs == nullptr || rhs == nullptr)
		{ CEPSERROR("Internal error: Operand(s) evaluates to null.") }
    if (thoroughness == ceps::interpreter::thoroughness_t::shallow)
      return mk_bin_op(op,lhs,rhs,ceps::ast::op_str(ceps::ast::as_binop_ref(binop_node)));;

	if (op == '=')
	{
		using namespace ceps::parser_env;
		if (lhs->kind() != Kind::identifier){
			std::stringstream ss;
			ss << *binop_node;
			throw semantic_exception{binop_node," Left hand side of assignment should be a variable ["+ss.str()+"]"};
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
    
	if (
		is<Ast_node_kind::identifier>(lhs) || 
		is<Ast_node_kind::identifier>(rhs) || 
		is<Ast_node_kind::func_call>(rhs)  || 
		is<Ast_node_kind::func_call>(lhs)  || 
		is<Ast_node_kind::binary_operator>(rhs) || 
		is<Ast_node_kind::binary_operator>(lhs) || 
		is<Ast_node_kind::unary_operator>(lhs) || 
		is<Ast_node_kind::unary_operator>(rhs) ||  
		is<Ast_node_kind::symbol>(lhs) || 
		is<Ast_node_kind::symbol>(rhs) 		
		) return mk_bin_op(op,lhs,rhs,ceps::ast::op_str(ceps::ast::as_binop_ref(binop_node)));

	
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
		auto r = equality(lhs,rhs);
		if (r.has_value()) return r.value() ? mk_int_node(1) : mk_int_node(0);
	}
	else if (op == ceps::Cepsparser::token::REL_OP_NEQ)
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
	else if (op == ceps::Cepsparser::token::REL_OP_GT)
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
	else if (op == ceps::Cepsparser::token::REL_OP_LT)
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
	else if (op == ceps::Cepsparser::token::REL_OP_GT_EQ)
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
	else if (op == ceps::Cepsparser::token::REL_OP_LT_EQ)
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
	else  if (op == '+')
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
    else if (op == '%')
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
	else if (op == '&')
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

	else if (op == '|')
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
	else if (op == '-')
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
	else if (op == '*')
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
	else if (op == '/')
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
	else if (op == '^')
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
