/**
 The MIT License (MIT)

Copyright (c) 2014,2015 The authors of ceps

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

#ifndef CEPS_INTERPRETER_H_
#define CEPS_INTERPRETER_H_

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <string>
#include <cstring>
#include <fstream>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <sstream>
#include <map>
#include <cmath>
#include <set>
#include <vector>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>
#include <utility>
#include <valarray>
// C Headers

#include <stdio.h>
#include <stdlib.h>



#include "symtab.hh"
#include "cepsparserdriver.hh"
#include "ceps_ast.hh"
#include "cepsnodeset.hh"
#include "global_defs.hh"


#include <sstream>
#include <map>

namespace ceps{
 namespace interpreter{
 	 class semantic_exception {
 		 ceps::ast::Nodebase_ptr loc_;
 		 std::string msg_;
 	 public:
 		 semantic_exception(ceps::ast::Nodebase_ptr where, std::string const & what)
 	 	 	 :loc_(where),msg_(what)
 	 	 {}
 	 	ceps::ast::Nodebase_ptr where() const
 	 	{
 	 		return loc_;
 	 	}
 	 	ceps::ast::Nodebase_ptr & where()
 	 	{
 	 		return loc_;
 	 	}
 	 	std::string const & what() const
 	 	{
 	 		return msg_;
 	 	}
 	 	std::string & what()
 	 	{
 	 		return msg_;
 	 	}

 	 };

	 struct Environment;
	 using Symboltable = ceps::parser_env::Symboltable;
	 using Kind = ceps::ast::Ast_node_kind;

	 ceps::ast::Nodebase_ptr evaluate(ceps::ast::Nonleafbase & root,
		 Symboltable & sym_table,
		 Environment& env, ceps::ast::Nodebase_ptr parent_node,ceps::ast::Nodebase_ptr predecessor
		 );

 	 struct Environment
 	 {
 		using Fn_binop_overload = ceps::ast::Nodebase_ptr (*)(ceps::ast::Nodebase_ptr , ceps::ast::Nodebase_ptr);
		 int kind_to_id_map_ctr_{0};
		 std::map< std::string, int> kind_to_id_map_;
		 std::map< std::tuple<char,int,int>, ceps::interpreter::Environment::Fn_binop_overload >
		 global_binop_overloads_;
		 ceps::ast::Nodeset * associated_universe_ = nullptr;
		 std::map<std::string, std::map<std::string,ceps::ast::Nodebase_ptr >* > symbol_mapping_;
		 typedef ceps::ast::Nodebase_ptr (*func_callback_t)(std::string const & , ceps::ast::Call_parameters*, void *, ceps::parser_env::Symboltable & sym_table);
		 typedef ceps::ast::Nodebase_ptr (*func_binop_resolver_t)(ceps::ast::Binary_operator_ptr binop,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr lhs ,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr rhs,
				 	 	 	 	 	 	 	 	 	 	 	 	  void* cxt,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr parent_node);
		 typedef ceps::ast::Nodebase_ptr (*func_callback_if_symbol_undefined_t)(ceps::ast::Nodebase_ptr,ceps::ast::Nodebase_ptr, void *);


		 func_callback_t func_callback_ = nullptr;
		 func_binop_resolver_t global_binop_resolver_ = nullptr;
		 bool (*is_lazy_func)(std::string const &) = nullptr;

		 void * func_callback_context_data_ = nullptr;
		 void * func_binop_resolver_context_data_ = nullptr;
		 func_callback_if_symbol_undefined_t func_callback_if_symbol_undefined_ = nullptr;
         void * func_callback_if_symbol_undefined_ctxt_ = nullptr;
		 // Set by inner nodes (like stmts) which define the scope of node set expressions.
		 std::vector<ceps::ast::Nodebase_ptr>* scope;

		 ceps::ast::Nodebase_ptr call_func_callback(std::string const & id, ceps::ast::Call_parameters* params, ceps::parser_env::Symboltable & sym_table)
		 {
			 if (func_callback_ == nullptr) return nullptr;
			 return func_callback_(id,params,func_callback_context_data_,sym_table);
		 }
		 ceps::ast::Nodebase_ptr call_binop_resolver(ceps::ast::Binary_operator_ptr binop,
				                                     ceps::ast::Nodebase_ptr lhs ,
				                                     ceps::ast::Nodebase_ptr rhs,
				                                     ceps::ast::Nodebase_ptr parent_node)
		 {
		 	 if (global_binop_resolver_ == nullptr) return nullptr;
		 	 return global_binop_resolver_(binop,lhs,rhs,func_binop_resolver_context_data_,parent_node);
		 }

		 std::map<std::string,std::string> meta_info_;
 	  public:

		 void reg_sym_undefined_clbk(func_callback_if_symbol_undefined_t f,void* ctxt){func_callback_if_symbol_undefined_ = f;func_callback_if_symbol_undefined_ctxt_ = ctxt;}
		 func_callback_if_symbol_undefined_t get_sym_undefined_clbk(){return func_callback_if_symbol_undefined_;}
		 ceps::ast::Nodebase_ptr call_sym_undefined_clbk(ceps::ast::Nodebase_ptr  n, ceps::ast::Nodebase_ptr pred){
			 if(nullptr == get_sym_undefined_clbk()) return n;
			 return get_sym_undefined_clbk()(n,pred,func_callback_if_symbol_undefined_ctxt_);
		 }

		 std::map<std::string, std::map<std::string,ceps::ast::Nodebase_ptr >* >& symbol_mapping() {return symbol_mapping_;}
		 std::map<std::string, std::map<std::string,ceps::ast::Nodebase_ptr >* > const & symbol_mapping() const {return symbol_mapping_;}

		 std::map<std::string,std::string>& meta_info() {return meta_info_;}
		 int lookup_kind(std::string const&);

		 void set_func_callback(func_callback_t f,void * func_callback_context_data){func_callback_ = f;func_callback_context_data_ = func_callback_context_data;}
		 void get_func_callback(func_callback_t& f,void *& func_callback_context_data){f = func_callback_;func_callback_context_data=func_callback_context_data_;}



		 void set_binop_resolver(func_binop_resolver_t f,void * cxt){global_binop_resolver_ = f;func_binop_resolver_context_data_ = cxt;}
		 void get_binop_resolver(func_binop_resolver_t& f,void * & cxt){f=global_binop_resolver_; cxt=func_binop_resolver_context_data_;}

		 void register_global_binop_overload( 	ceps::interpreter::Environment::Fn_binop_overload fn,
												char op,
												std::string const & lhs_kind,
												std::string const & rhs_kind );

		 Fn_binop_overload get_glbl_binop_overload(  char op,
				                                    std::string const & lhs_kind,
													std::string const & rhs_kind);


		 ceps::ast::Nodeset * & associated_universe() {return associated_universe_;}
#ifndef _MSC_VER
		 friend ceps::ast::Nodebase_ptr evaluate(ceps::ast::Nodebase_ptr,
				  ceps::parser_env::Symboltable & ,
				  ceps::interpreter::Environment&,ceps::ast::Nodebase_ptr,ceps::ast::Nodebase_ptr predecessor);
#endif
 	 };

    

  
 	ceps::ast::Nodebase_ptr eval_kinddef(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);
 	ceps::ast::Nodebase_ptr eval_valdef(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);
 	ceps::ast::Nodebase_ptr eval_unaryop(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);
 	ceps::ast::Nodebase_ptr eval_ifelse(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);
 	ceps::ast::Nodebase_ptr eval_funccall(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);
 	ceps::ast::Nodebase_ptr eval_binaryop(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);
 	ceps::ast::Nodebase_ptr eval_id(ceps::ast::Nodebase_ptr root_node,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);

 	ceps::ast::Nodebase_ptr eval_macro(ceps::ast::Nodebase_ptr root_node,
	                                   ceps::parser_env::Symbol* sym_ptr,
			                           ceps::parser_env::Symboltable & sym_table,
			                           ceps::interpreter::Environment& env,
			                           ceps::ast::Nodebase_ptr parent_node,
			                           ceps::ast::Nodebase_ptr predecessor,
									   std::vector<ceps::ast::Nodebase_ptr>* args = nullptr);

 	ceps::ast::Nodebase_ptr eval_rewrite(ceps::ast::Nodebase_ptr root_node,ceps::parser_env::Symbol* sym_ptr,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor);

 	using struct_rewrite_fn_t = ceps::ast::Nodeset (*)(
 				ceps::ast::Struct_ptr,
 				ceps::ast::Nodebase_ptr,ceps::parser_env::Symbol* ,
 				ceps::parser_env::Symboltable &,
 				ceps::interpreter::Environment& ,
 				ceps::ast::Nodebase_ptr ,
 				ceps::ast::Nodebase_ptr );

 	void register_struct_rewrite_rule(ceps::parser_env::Symboltable &,std::string which_struct, struct_rewrite_fn_t fn, void* ctxt);

     void evaluate(	 ceps::ast::Nodeset & universe,
    		 	 	 ceps::ast::Nodebase_ptr root,
    		 	 	 Symboltable & sym_table,
    		 	 	 Environment& env,
    		 	 	 std::vector<ceps::ast::Nodebase_ptr>* generated_nodes = nullptr
    		 	 	 );
     void evaluate_without_modifying_universe (	 ceps::ast::Nodeset & universe,
                                                 ceps::ast::Nodebase_ptr root,
                                                 Symboltable & sym_table,
                                                 Environment& env,
                                                 std::vector<ceps::ast::Nodebase_ptr>* generated_nodes = nullptr
                                                 );

     ceps::ast::Nodebase_ptr handle_binop(	ceps::ast::Nodebase_ptr binop_node,
    		 	 	 	 	 	 	 	 	int op,
    		 	 	 	 	 	 	 	 	ceps::ast::Nodebase_ptr lhs,
    		 	 	 	 	 	 	 	 	ceps::ast::Nodebase_ptr rhs,
    		 	 	 	 	 	 	 	 	Symboltable & sym_table,
    		 	 	 	 	 	 	 	 	Environment& env,ceps::ast::Nodebase_ptr parent_node
    		 	 	 	 	 	 	 	 	);

     ceps::ast::Nodebase_ptr evaluate(  ceps::ast::Nodebase_ptr root_node,
    		 	 	 	 	 	 	 	 Symboltable & sym_table,
    		 	 	 	 	 	 	 	 Environment& env,ceps::ast::Nodebase_ptr parent_node,ceps::ast::Nodebase_ptr predecessor
    		 	 	 	 	 	 	 	 );

     ceps::ast::Nodebase_ptr evaluate_and_remove(ceps::ast::Nonleafbase& root);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Stmt& stmt);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Expression& expr);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Binary_operator& bin_op);

 }
}



#endif
