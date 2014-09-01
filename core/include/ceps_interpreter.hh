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
#include "global_defs.hh"
#include <sstream>
#include<map>

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

 	 struct Environment
 	 {
 		using Fn_binop_overload = ceps::ast::Nodebase_ptr (*)(ceps::ast::Nodebase_ptr , ceps::ast::Nodebase_ptr);
 	  private:
		 int kind_to_id_map_ctr_{0};
		 std::map< std::string, int> kind_to_id_map_;
		 std::map< std::tuple<char,int,int>, ceps::interpreter::Environment::Fn_binop_overload >
		  global_binop_overloads_;
 	  public:

		int lookup_kind(std::string const&);

		void register_global_binop_overload( 	ceps::interpreter::Environment::Fn_binop_overload fn,
												char op,
												std::string const & lhs_kind,
												std::string const & rhs_kind );

		Fn_binop_overload get_glbl_binop_overload(  char op,
				                                    std::string const & lhs_kind,
													std::string const & rhs_kind);

 	 };

     using Symboltable = ceps::parser_env::Symboltable;
     using Kind = ceps::ast::Ast_node_kind;

     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Nonleafbase & root,
    		 	 	 	 	 	 	   Symboltable & sym_table,
    		 	 	 	 	 	 	   Environment& env);

     ceps::ast::Nodebase_ptr handle_binop(	ceps::ast::Nodebase_ptr binop_node,char op,
    		 	 	 	 	 	 	 	 	ceps::ast::Nodebase_ptr lhs,
    		 	 	 	 	 	 	 	 	ceps::ast::Nodebase_ptr rhs,
    		 	 	 	 	 	 	 	 	Symboltable & sym_table);

     ceps::ast::Nodebase_ptr evaluate(  ceps::ast::Nodebase_ptr root_node,
    		 	 	 	 	 	 	 	 Symboltable & sym_table,
    		 	 	 	 	 	 	 	 Environment& env);

     ceps::ast::Nodebase_ptr evaluate_and_remove(ceps::ast::Nonleafbase& root);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Stmt& stmt);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Expression& expr);
     ceps::ast::Nodebase_ptr evaluate(ceps::ast::Binary_operator& bin_op);

 }
}



#endif
