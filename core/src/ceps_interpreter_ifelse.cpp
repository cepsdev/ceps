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
#include "ceps_interpreter_nodeset.hh"
namespace ceps::interpreter{
	using namespace ceps::ast;

node_t eval_ifelse(node_t root_node,
		ceps::parser_env::Symboltable & sym_table,
		Environment& env,
		node_t parent_node,
		node_t predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness
		)
	{

		auto& ifelse = as_ifelse_ref(root_node);
		bool s1{false};bool s2{false};bool s3{false};
		auto cond = evaluate_generic(ifelse.children()[0],sym_table,env,root_node,nullptr,nullptr,s1,thoroughness);
		node_t left_branch{},right_branch{};
	
		auto cond_val  = ceps::ast::is_int(cond);
		if ( cond_val.first ){
			node_t r{};
			if (cond_val.second  != 0 ){
				if (num_of_children(ifelse) > 1) 
					r = evaluate_generic(
							children(ifelse)[1],
							sym_table,
							env,
							root_node,
							children(ifelse)[0],
							nullptr,
							s2,
							thoroughness);
			} else {
				if (num_of_children(ifelse) > 2) 
					r = evaluate_generic(
							children(ifelse)[2],
							sym_table,
							env,
							root_node,
							children(ifelse)[1],
							nullptr,
							s3,
							thoroughness);
			}
			symbols_found = false;
			if (r == node_t{} || !is<Ast_node_kind::scope>(r) ) return r;
			//INVARIANT: r is a scope element
			auto& scope = *nlf_ptr(r);
			if (num_of_children(scope) == 0) return nullptr;
			if (num_of_children(scope) == 1) return children(scope)[0];
			return create_ast_nodeset("", children(scope));
		}//if 

		// Conditional is a symbolic expression => shallow expansion of left/right branches

		if (num_of_children(ifelse) > 1) 
			left_branch = evaluate_generic(		children(ifelse)[1],
												sym_table,
												env,
												root_node,
												children(ifelse)[0],
												nullptr,
												s2,
												thoroughness_t::shallow); 
		if (num_of_children(ifelse) > 2) 
			right_branch = evaluate_generic( 	children(ifelse)[2],
												sym_table,
												env,
												root_node,
												children(ifelse)[1],
												nullptr,
												s3,
												thoroughness_t::shallow);
		symbols_found = s1;
		return mk_ifelse(cond,left_branch,right_branch);
	}//eval_ifelse(...)
}//namespace ceps::interpreter

