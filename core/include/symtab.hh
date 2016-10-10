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

#ifndef SYMTAB_H_
#define SYMTAB_H_

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <exception>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include "global_defs.hh"

// Yet Another Model Description Language
namespace ceps
{
 namespace parser_env
 {
 	 class Symbol
 	 {
 	 public:
 		 //In Eclipse CDT 8.1.2 the declaration below is marked by the code-analyzer as errorneous.
 		 //This is a well known bug (https://bugs.eclipse.org/bugs/show_bug.cgi?id=385144) and will
 		 //be fixed in Eclipse 8.2 (released in june).
 		 enum Category{UNDEFINED,CTOR,VAR,LAMBDA,KIND,SYMBOL,MACRO,NODESET};
 		 Category category;
 		 std::string name;
 		 void* payload = nullptr; //TODO: THAT'S F***ING UGLY
 		 int userdef_ = 0;
 		 Symbol(Category symbol_category):category(symbol_category){}
 		 Symbol():category(UNDEFINED){}
  		 operator bool() {return category != UNDEFINED;}
  		 int& userdefined() {return userdef_;}
  		 int userdefined() const {return userdef_;}
 	 };

 	 template <typename Iter>
		 class Symbolptr
		 {
	 		 	 Iter it_;
	 		 	 bool valid_;
		 public:
			 Symbolptr(const Iter& it,bool valid):it_(it),valid_(valid) {}
			 Symbol& operator *() {return it_->second;}
			 const Symbol& operator *() const {return it_->second;}
			 operator bool() const {return valid_;}
			 operator bool() {return valid_;}
			 operator Symbol* () {return &it_->second;}
			 operator const Symbol* () const {return &it_->second;}
			 bool valid() {return valid_;}
		 };


 	 class Scope
 	 {
 	 public:

 	 	typedef std::map<std::string,Symbol>::iterator Iter;
 	 	typedef std::map<std::string,Symbol>::const_iterator Iter_const;

 	 	 Symbolptr<Iter> find(const std::string& name)
 	 	 {
 	 		 Iter it = name_to_symbol.find(name);
 	 		 return Symbolptr<Iter>(it,it != name_to_symbol.end());
 	 	 }
 	 	 const Symbolptr<Iter_const> find(const std::string& name) const
 	 	 {
 	 		 Iter_const it = name_to_symbol.find(name);
 	 		 return Symbolptr<Iter_const>(it,it != name_to_symbol.end());
 	 	 }
 	 	 Symbol* insert(const std::string& name)
 	 	 {

 	 		 name_to_symbol[name].name = name;
 	 		 return &name_to_symbol[name];
 	 	 }

 		 std::map<std::string,Symbol> name_to_symbol;
 	 };
 	 class Symboltable
 	 {
 	 public:
 		 void push_scope();
 		 void pop_scope();
 		 Symbol* lookup( const std::string name,
 				 	 	 bool insert = false,
 				 	 	 bool only_current_scope = false,
 				 	 	 bool return_null_if_already_defined=false);
 		 Symbol* lookup_global(const std::string name,bool insert=false)
 		 {
 			if (scopes.size() == 0) return nullptr;
 			Symbolptr<Scope::Iter> res = scopes[0].find(name);
 			if (!res)
 			{
 				if (!insert) return nullptr;
 				return scopes[0].insert(name);
 			} else return res;
 		 }

 		 std::vector<Scope> scopes;
 	 };
 }
}




#endif /* SYMTAB_H_ */
