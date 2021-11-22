/*
Copyright 2021 Tomas Prerovsky (cepsdev@hotmail.com).

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
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

ceps::ast::Nodebase_ptr ceps::interpreter::eval_macro(
	        ceps::ast::Nodebase_ptr root_node,
            ceps::parser_env::Symbol* sym_ptr,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor,
			ceps::interpreter::thoroughness_t thoroughness,
			bool& symbols_found,
			std::vector<ceps::ast::Nodebase_ptr>* args)
{
	ceps::ast::Nodebase_ptr body = (ceps::ast::Nodebase_ptr)(sym_ptr->payload);
	ceps::ast::Stmts* result = nullptr;
	ceps::ast::Struct_ptr arglist_ = nullptr;
	bool s1{false};
	auto arglist = create_ast_nodeset("", 
	 args != nullptr ? *args : ceps::ast::as_struct_ptr(evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),
	 sym_table,
	 env,
	 root_node,
	 predecessor,
	 nullptr, s1, thoroughness))->children());
	auto symbol = sym_table.lookup("arglist",true,true,false);
	symbol->category = ceps::parser_env::Symbol::NODESET;
	symbol->payload = (void*)(arglist);
	bool s2{false};
	result = ceps::ast::as_stmts_ptr(evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(body),sym_table,env,root_node,predecessor,nullptr,s2,thoroughness));
	symbols_found = s1 || s2;
	return create_ast_nodeset("",result->children());
}

ceps::ast::Nodebase_ptr ceps::interpreter::eval_rewrite(ceps::ast::Nodebase_ptr root_node,ceps::parser_env::Symbol* sym_ptr,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor,
			bool& symbols_found,
			ceps::interpreter::thoroughness_t thoroughness)
{
	ceps::ast::Nodeset result;
	if (sym_ptr->payload == nullptr) return create_ast_nodeset("",result.nodes());
	auto fn = (ceps::interpreter::struct_rewrite_fn_t)(sym_ptr->payload);
	ceps::ast::Struct_ptr arglist_ = nullptr;
	arglist_ = ceps::ast::as_struct_ptr(evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,nullptr,symbols_found,thoroughness));
	result = fn(arglist_,root_node,sym_ptr,sym_table,env,parent_node,predecessor);
	return create_ast_nodeset("",result.nodes());
}

void ceps::interpreter::register_struct_rewrite_rule(ceps::parser_env::Symboltable & symtab,std::string which_struct,struct_rewrite_fn_t fn, void* ctxt){
	auto symbol = symtab.lookup(which_struct,true,true,false);
	symbol->category = ceps::parser_env::Symbol::REWRITE;
	symbol->payload = (void*)fn;
	symbol->data = ctxt;
}

