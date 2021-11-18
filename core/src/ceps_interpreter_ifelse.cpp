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

ceps::ast::Nodebase_ptr ceps::interpreter::eval_ifelse(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		ceps::interpreter::thoroughness_t thoroughness)
{
 ceps::ast::Ifelse& ifelse = ceps::ast::as_ifelse_ref(root_node);
 ceps::ast::Nodebase_ptr cond = evaluate_generic(ifelse.children()[0],sym_table,env,root_node,nullptr,nullptr,thoroughness);
 ceps::ast::Nodebase_ptr left_branch = nullptr,right_branch=nullptr;

 
 if (cond->kind() == ceps::ast::Ast_node_kind::int_literal){
	 ceps::ast::Nodebase_ptr r = nullptr;
	 if (ceps::ast::value(ceps::ast::as_int_ref(cond)) != 0 ){
		 if (ifelse.children().size() > 1) r = evaluate_generic(ifelse.children()[1],sym_table,env,root_node,ifelse.children()[0],nullptr,thoroughness);
	 } else {
		 if (ifelse.children().size() > 2) r = evaluate_generic(ifelse.children()[2],sym_table,env,root_node,ifelse.children()[1],nullptr,thoroughness);
	 }
	 if (r == nullptr || r->kind() != ceps::ast::Ast_node_kind::scope) return r;
	 //INVARIANT: r is a scope element
	 auto& scope = *ceps::ast::nlf_ptr(r);
	 if (scope.children().size() == 0) return nullptr;
	 if (scope.children().size() == 1) return scope.children()[0];
	 return create_ast_nodeset("", scope.children());
 }


 if (ifelse.children().size() > 1) left_branch = ifelse.children()[1]->clone(); //evaluate_generic(ifelse.children()[1],sym_table,env,root_node,ifelse.children()[0],nullptr); 
 if (ifelse.children().size() > 2) right_branch = ifelse.children()[2]->clone(); // evaluate_generic(ifelse.children()[2],sym_table,env,root_node,ifelse.children()[1],nullptr);
 return new ceps::ast::Ifelse(cond,left_branch,right_branch);
}
