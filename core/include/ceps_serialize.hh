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


#ifndef YAMDL_SERIALIZE_H_
#define YAMDL_SERIALIZE_H_

#include "ceps_ast.hh"
#include<string>
#include<ostream>

namespace ceps{
	namespace ast {

	void helper_serialize_print_ws(std::ostream& os,int n);
	std::string helper_cast_string(std::string const & s);
	void serialize(std::ostream & os, Unit_rep const & u);
	void serialize(std::ostream & os, String & s,int indent,std::string const & eol);
	void serialize(std::ostream & os, Int & v,int indent,std::string const & eol,bool with_unit = true);
	void serialize(std::ostream & os, Double & v,int indent,std::string const & eol,bool with_unit = true);
	void serialize(std::ostream & os, Struct_ptr s, int indent);
	void serialize(std::ostream & os,std::initializer_list<strct> l);

	}
}


#endif

