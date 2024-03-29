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
	 extern bool DEBUG_OUTPUT; 

	 enum class thoroughness_t{
		 normal,
		 shallow
	 };

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


 	 struct Environment
 	 {
 		using Fn_binop_overload = ceps::ast::Nodebase_ptr (*)(ceps::ast::Nodebase_ptr , ceps::ast::Nodebase_ptr);
		int kind_to_id_map_ctr_{0};
		std::map< std::string, int> kind_to_id_map_;
		std::map< std::tuple<char,int,int>, ceps::interpreter::Environment::Fn_binop_overload >
		global_binop_overloads_;
		ceps::ast::Nodeset * associated_universe_ = nullptr;
		std::map<std::string, std::map<std::string,ceps::ast::Nodebase_ptr >* > symbol_mapping_;
		typedef ceps::ast::Nodebase_ptr (*func_callback_t)(std::string const & , ceps::ast::Call_parameters*, void *, ceps::parser_env::Symboltable & );
		typedef ceps::ast::Nodebase_ptr (*func_stmt_claimer_t)(ceps::ast::node_t, void *, ceps::parser_env::Symboltable &, Environment*);
		typedef ceps::ast::Nodebase_ptr (*func_binop_resolver_t)(ceps::ast::Binary_operator_ptr binop,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr lhs ,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr rhs,
				 	 	 	 	 	 	 	 	 	 	 	 	  void* cxt,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr parent_node);
		typedef bool (*func_binop_resolver_pred_t)(ceps::ast::Binary_operator_ptr binop,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr lhs ,
				 	 	 	 	 	 	 	 	 	 	 	 	  void* cxt,
				 	 	 	 	 	 	 	 	 	 	 	 	  ceps::ast::Nodebase_ptr parent_node);

		 typedef ceps::ast::Nodebase_ptr (*func_callback_if_symbol_undefined_t)(ceps::ast::Nodebase_ptr,ceps::ast::Nodebase_ptr, void *);

		 func_callback_t func_callback_ = nullptr;
		 func_binop_resolver_t global_binop_resolver_ = nullptr;
		 func_binop_resolver_pred_t global_binop_resolver_pred_ = nullptr;
		 
		 func_stmt_claimer_t func_stmt_claimer = nullptr;
		 void*  ctxt_stmt_claimer = nullptr; 
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
		 bool binop_resolver_requires_evaluated_rhs(ceps::ast::Binary_operator_ptr binop,
				                                     ceps::ast::Nodebase_ptr lhs ,
				                                     ceps::ast::Nodebase_ptr parent_node)													 
		{
			if (global_binop_resolver_ == nullptr) return false;
			if (global_binop_resolver_pred_ == nullptr) return true;
			return global_binop_resolver_pred_(binop,lhs,func_binop_resolver_context_data_,parent_node);
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
		 int inside_func_call_ctr_ = 0;

 	  public:
	     int inside_func_call_ctr() const {return inside_func_call_ctr_;}
	     int& inside_func_call_ctr() {return inside_func_call_ctr_;}

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

		 ceps::ast::node_t handle_stmt(ceps::ast::node_t node, ceps::parser_env::Symboltable & sym_table){
			 if (func_stmt_claimer != nullptr)
			  return func_stmt_claimer(node,ctxt_stmt_claimer, sym_table, this);
			return nullptr;
		 }

		 void set_func_stmt_claimer(func_stmt_claimer_t f,void * context_data){func_stmt_claimer = f;ctxt_stmt_claimer = context_data;}
		 void get_func_stmt_claimer(func_stmt_claimer_t& f,void *& context_data){f = func_stmt_claimer;context_data=ctxt_stmt_claimer;}

		 void set_func_callback(func_callback_t f,void * func_callback_context_data){func_callback_ = f;func_callback_context_data_ = func_callback_context_data;}
		 void get_func_callback(func_callback_t& f,void *& func_callback_context_data){f = func_callback_;func_callback_context_data=func_callback_context_data_;}

		 void set_binop_resolver(func_binop_resolver_t f,void * cxt){global_binop_resolver_ = f;func_binop_resolver_context_data_ = cxt;}
		 void get_binop_resolver(func_binop_resolver_t& f,void * & cxt){f=global_binop_resolver_; cxt=func_binop_resolver_context_data_;}

		 void set_binop_resolver_predicate(func_binop_resolver_pred_t f){global_binop_resolver_pred_ = f;}
		 void get_binop_resolver_predicate(func_binop_resolver_pred_t& f){f=global_binop_resolver_pred_;}

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
				  ceps::interpreter::Environment&,ceps::ast::Nodebase_ptr,ceps::ast::Nodebase_ptr,ceps::ast::Nodebase_ptr);
#endif
 	 };

	ceps::ast::Nodebase_ptr evaluate_nonleaf(
		ceps::ast::Nonleafbase & root,
		Symboltable & sym_table,
		Environment& env, 
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
	    ceps::ast::Nodebase_ptr this_ptr,
		bool& symbols_found,
		thoroughness_t thoroughness = thoroughness_t::normal
	);

    ceps::ast::Nodebase_ptr evaluate_generic(  
		ceps::ast::Nodebase_ptr root_node,
    	Symboltable & sym_table,
    	Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		ceps::ast::Nodebase_ptr this_ptr,
		bool& symbols_found,	
		thoroughness_t thoroughness  = thoroughness_t::normal
    );

 	ceps::ast::Nodebase_ptr eval_kinddef(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,		
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_valdef(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,		
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_unaryop(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,		
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_ifelse(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_funccall(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		ceps::ast::Nodebase_ptr this_ptr,
		bool& symbols_found,
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_binaryop(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_id(ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

 	ceps::ast::Nodebase_ptr eval_macro(ceps::ast::Nodebase_ptr root_node,
	    ceps::ast::Nodebase_ptr body,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		thoroughness_t thoroughness,
		bool& symbols_found,
		std::vector<ceps::ast::Nodebase_ptr>* args = nullptr
	);

   std::vector<ceps::ast::Nodebase_ptr> eval_macro_no_nodeset(
	        ceps::ast::Nodebase_ptr root_node,
            ceps::ast::Nodebase_ptr body,
			ceps::parser_env::Symboltable & sym_table,
			ceps::interpreter::Environment& env,
			ceps::ast::Nodebase_ptr parent_node,
			ceps::ast::Nodebase_ptr predecessor,
			ceps::interpreter::thoroughness_t thoroughness,
			bool& symbols_found,
			std::vector<ceps::ast::Nodebase_ptr>* args = nullptr);
 
	ceps::ast::Nodebase_ptr handle_binop(	ceps::ast::Nodebase_ptr binop_node,
    	int op,
    	ceps::ast::Nodebase_ptr lhs,
    	ceps::ast::Nodebase_ptr rhs,
    	Symboltable & sym_table,
    	Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr this_ptr, 
		thoroughness_t thoroughness,
		bool& symbols_found
    );

 	ceps::ast::Nodebase_ptr eval_rewrite(ceps::ast::Nodebase_ptr root_node,ceps::parser_env::Symbol* sym_ptr,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		thoroughness_t thoroughness  = thoroughness_t::normal
	);

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


     

     ceps::ast::Nodebase_ptr evaluate_and_remove(ceps::ast::Nonleafbase& root);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Stmt& stmt);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Expression& expr);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Binary_operator& bin_op);

 }
}
namespace ceps{
	namespace interpreter{
		using namespace ceps::ast;
		using node_t = Nodebase_ptr;
        using node_int64_t = Int64*;
		using node_int_t = Int*;
		using node_double_t = Double*;
		using node_symbol_t = ceps::ast::Symbol*;
		using node_binary_operator_t = ceps::ast::Binary_operator*;
		using node_unary_operator_t = ceps::ast::Unary_operator*;
		using node_vec_t = std::vector<node_t>;
		using node_unaryop_t = ceps::ast::Unary_operator*;

		node_unaryop_t mk_unary_op_node(char op, node_t operand);
		node_int_t mk_int_node(int value);
		node_int64_t mk_int64_node(std::int64_t value);
		node_int64_t mk_int64_node(std::int64_t value, Unit_rep u);
		node_double_t mk_double_node(double value, Unit_rep u);
		node_symbol_t mk_symbol(std::string name, std::string kind);
		node_binary_operator_t mk_bin_op(int op,node_t lhs = nullptr, node_t rhs = nullptr, std::string full_name = "");
		node_unary_operator_t mk_unary_op(int op,node_t arg, std::string full_name = "");

		int read_int(node_t n);
		node_vec_t get_args(Call_parameters& params);
		node_t as_nodeset(node_t root_node, Symboltable & sym_table, Environment& env, node_t parent_node, node_t predecessor, Call_parameters& params);
		node_t mktime(node_t root_node, Symboltable & sym_table, Environment& env,node_t parent_node, node_t predecessor, Call_parameters& params);
		node_t push_back(node_t root_node, Symboltable & sym_table, Environment& env,node_t parent_node, node_t predecessor, Call_parameters& params);
	}
}
#endif
