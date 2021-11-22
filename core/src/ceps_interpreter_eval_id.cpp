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


ceps::ast::Nodebase_ptr ceps::interpreter::eval_id(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
        ceps::interpreter::thoroughness_t thoroughness
		)
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
		 symbols_found = true;
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
		symbols_found = true;
		return new ceps::ast::Symbol(name(v), kind(v), nullptr, nullptr, nullptr);
	 }
	 else
		 return node_ptr;

}
