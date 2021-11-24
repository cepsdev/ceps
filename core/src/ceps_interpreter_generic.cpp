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

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate_generic(ceps::ast::Nodebase_ptr root_node,
													ceps::parser_env::Symboltable & sym_table,
													ceps::interpreter::Environment& env,
													ceps::ast::Nodebase_ptr parent_node,
													ceps::ast::Nodebase_ptr predecessor,
													ceps::ast::Nodebase_ptr this_ptr,
													bool& symbols_found,
													ceps::interpreter::thoroughness_t thoroughness
													)
 {
	 if(!root_node)
		 return nullptr;
	 if (ceps::interpreter::DEBUG_OUTPUT) std::cerr << "ceps::interpreter::evaluate_generic " << *root_node << std::endl;

	 node_t result = nullptr;

	 switch(root_node->kind())
	 {
	 case Kind::kind_def:
	 {
		 return eval_kinddef(root_node,
		 			sym_table,
		 			env,
		 			parent_node,
		 			predecessor,
					symbols_found,
                    thoroughness);
	 }
	 case Kind::valdef:
	 {
		 return eval_valdef(root_node,
		 			sym_table,
		 			env,
		 			parent_node,
		 			predecessor,
					symbols_found,
                    thoroughness);
	 }
	 case Kind::let:
	 {
		ceps::ast::Let& let_node{ceps::ast::as_let_ref(root_node)};
 		ceps::parser_env::Symbol* sym_ptr;

 		ceps::ast::Nodebase_ptr rhs = evaluate_generic(let_node.children()[0],sym_table,env,root_node,nullptr,nullptr,symbols_found,thoroughness);

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
		 return evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
	 case Kind::scope:
	 {
		 sym_table.push_scope();
	 	 auto r = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
	 	 sym_table.pop_scope();
	 	 return r;
	 }
	 case Kind::structdef:
	 {
		ceps::ast::Nodebase_ptr result = nullptr;
		sym_table.push_scope();
		std::string id = ceps::ast::name(ceps::ast::as_struct_ref(root_node));

		ceps::parser_env::Symbol* sym_ptr = sym_table.lookup(id);
		if ( sym_ptr  != nullptr && sym_ptr->category ==  ceps::parser_env::Symbol::Category::MACRO){
			result =  eval_macro(root_node,
					  sym_ptr,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,
					  thoroughness,
					  symbols_found);
		} else if ( sym_ptr != nullptr && sym_ptr->category ==  ceps::parser_env::Symbol::Category::REWRITE){
			result =  eval_rewrite(root_node,
					  sym_ptr,
		 			  sym_table,
		 			  env,
		 			  parent_node,
					  predecessor,symbols_found,thoroughness);
		} else if (
				sym_ptr != nullptr &&
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
			result = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
			if (id.length()) ceps::ast::name(ceps::ast::as_struct_ref(result)) = id;

		}  else if (
				sym_ptr != nullptr &&
				sym_ptr->category ==  ceps::parser_env::Symbol::Category::VAR &&
				sym_ptr->payload != nullptr &&

				(
				 (((ceps::ast::Nodebase_ptr)sym_ptr->payload)->kind() == ceps::ast::Ast_node_kind::identifier)
				)
		){
			result = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
			name(as_struct_ref(result)) = name(as_id_ref((ceps::ast::Nodebase_ptr)(sym_ptr->payload)));
		}
		else result = evaluate_nonleaf(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
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

		ceps::ast::Nodebase_ptr result = evaluate_generic(nleaf.children()[0],sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
		//nleaf.children().clear();
		return result;
	 }
	 case Kind::unary_operator:
	 {
		 return eval_unaryop(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,symbols_found,thoroughness);
	 }
	 case Kind::ifelse:
	 {
		 return eval_ifelse(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,symbols_found,thoroughness);
	 }
	 case Kind::func_call:
	 {
		if (ceps::interpreter::DEBUG_OUTPUT) std::cerr << "ceps::interpreter::evaluate_generic Kind::func_call:\n";
		result = eval_funccall(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,
					  this_ptr,symbols_found, thoroughness);
		break;
	 }
	 case Kind::binary_operator:
	 {
		 auto& bop{as_binop_ref(root_node)};
		 if (op_val(bop) != "#") return eval_binaryop(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,symbols_found, thoroughness);
		return evaluate_generic(bop.left(),sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
	 }
	 case Kind::identifier:
	 {
		 return eval_id(root_node,
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor, symbols_found,thoroughness);
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
				 Nodebase_ptr r = evaluate_generic(pp,sym_table,env,root_node,predecessor,this_ptr,symbols_found,thoroughness);predecessor=pp;
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
				  	  	  	  env,root_node,predecessor,symbols_found,thoroughness);
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
				auto rv = evaluate_generic(r,sym_table, env,root_node,predecessor,this_ptr,symbols_found,thoroughness);
				if (ceps::interpreter::DEBUG_OUTPUT) 
		      		std::cerr << "ceps::interpreter::evaluate_generic Kind::symbol: kind(" << kind<< ") name("<< name << ") -> "<<*rv << std::endl;
				 return rv;
			 }
			 return env.call_sym_undefined_clbk(root_node,parent_node);
		 } else {
		  	
		 }
		 symbols_found = true;
         return new ceps::ast::Symbol(name,kind);
	 }
	 case Kind::ret:
	 {
		 auto& ret_stmt = ceps::ast::as_return_ref(root_node);
		 ceps::ast::Nodebase_ptr operand = evaluate_generic(ret_stmt.children()[0],sym_table,env,(ceps::ast::Nodebase_ptr)&ret_stmt,predecessor,this_ptr,symbols_found,thoroughness);
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
	
	if (result != nullptr && ( is<ceps::ast::Ast_node_kind::scope>(parent_node) || is<ceps::ast::Ast_node_kind::structdef>(parent_node) || is<ceps::ast::Ast_node_kind::root>(parent_node)) ) {
		auto r = env.handle_stmt(result,sym_table);
		if (r != nullptr) return r;
	}
 	return result;
 }
