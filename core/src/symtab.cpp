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

#include "symtab.hh"
#include <exception>
#include <stdexcept>
#include <string>
#include <algorithm>


/**
 * FIXME:COMMENT
 */
void ceps::parser_env::Symboltable::push_scope()
{
 scopes.push_back(std::make_shared<Scope>());
}

/**
 * FIXME:COMMENT
 */
 void ceps::parser_env::Symboltable::pop_scope()
 {
  if (scopes.empty()) return;
  scopes.pop_back();
 }

 /**
  * FIXME:COMMENT
  */
 ceps::parser_env::Symbol* ceps::parser_env::Symboltable::lookup(const std::string name,
		                                                           bool insert,
		                                                           bool only_current_scope,
		                                                           bool return_null_if_already_defined)
 {

	 if (scopes.empty())
		 throw std::runtime_error("Symboltable: scopes.empty().");

	 for(scopes_t::reverse_iterator it = scopes.rbegin();it != scopes.rend();++it)
	 {
		 Symbolptr<Scope::Iter> res = it->get()->find(name);
		 if(res)
		 {
			 if (return_null_if_already_defined) return NULL;
			 else return res;
		 }
		 if (only_current_scope) break;
	 }

	 //INVARIANT: NOTHING FOUND
	 if(!insert)
		 return NULL;

	 return scopes.rbegin()->get()->insert(name);
 }

 ceps::parser_env::Symbol* ceps::parser_env::Symboltable::lookup_globally(const std::string name,
                                                                   bool insert,
                                                                   bool return_null_if_already_defined)
 {

     if (scopes.empty())
         throw std::runtime_error("Symboltable: scopes.empty().");

     Symbolptr<Scope::Iter> res = scopes[0]->find(name);
     if(res)
     {
      if (return_null_if_already_defined) return nullptr;
      else return res;
     }

     //INVARIANT: NOTHING FOUND
     if(!insert)
         return nullptr;

     return scopes[0]->insert(name);
 }
