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
#include"pugixml.hpp"


ceps::ast::Nodebase_ptr ceps::interpreter::eval_macro(
	        ceps::ast::Nodebase_ptr root_node,
            ceps::parser_env::Symbol* sym_ptr,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor,
			std::vector<ceps::ast::Nodebase_ptr>* args)
{
	ceps::ast::Nodebase_ptr body = (ceps::ast::Nodebase_ptr)(sym_ptr->payload);
	ceps::ast::Stmts* result = nullptr;
	ceps::ast::Struct_ptr arglist_ = nullptr;

	auto arglist = create_ast_nodeset("", 
	 args != nullptr ? *args : ceps::ast::as_struct_ptr(evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor))->children());
	auto symbol = sym_table.lookup("arglist",true,true,false);
	symbol->category = ceps::parser_env::Symbol::NODESET;
	symbol->payload = (void*)(arglist);
	result = ceps::ast::as_stmts_ptr(evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(body),sym_table,env,root_node,predecessor));
	return create_ast_nodeset("",result->children());
}

ceps::ast::Nodebase_ptr ceps::interpreter::eval_rewrite(ceps::ast::Nodebase_ptr root_node,ceps::parser_env::Symbol* sym_ptr,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor)
{


	ceps::ast::Nodeset result;
	if (sym_ptr->payload == nullptr) return create_ast_nodeset("",result.nodes());
	auto fn = (ceps::interpreter::struct_rewrite_fn_t)(sym_ptr->payload);
	ceps::ast::Struct_ptr arglist_ = nullptr;
	arglist_ = ceps::ast::as_struct_ptr(evaluate(*dynamic_cast<ceps::ast::Nonleafbase*>(root_node),sym_table,env,root_node,predecessor));
	result = fn(arglist_,root_node,sym_ptr,sym_table,env,parent_node,predecessor);
	return create_ast_nodeset("",result.nodes());
}

void ceps::interpreter::register_struct_rewrite_rule(ceps::parser_env::Symboltable & symtab,std::string which_struct,struct_rewrite_fn_t fn, void* ctxt){
	auto symbol = symtab.lookup(which_struct,true,true,false);
	symbol->category = ceps::parser_env::Symbol::REWRITE;
	symbol->payload = (void*)fn;
	symbol->data = ctxt;
}

