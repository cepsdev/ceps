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

#include "ceps_ast.hh"

using namespace ceps::ast;

const char * ceps::ast::ast_node_kind_to_text[] = {	"ROOT","STRUCT","ID","STRING","INT",
																	"FLOAT","EXPR","SI_UNIT","OPERATOR",
																	"STMTS","STMT","VAL","LAMBDA","PARAMS",
																	"PARAM","LAMBDA_BODY","RAWMAP","ATOMS",
																	"VEC","NEG","SCOPE","FUNC_CALL",
																	"CALL_PARAMETERS","INT64","UINT64",
																	"KINDDEF","KIND","SYMBOL","FOR","FOR_HEAD"};

const int ceps::ast::Nodebase::iword_index = std::ios_base::xalloc();


inline const char * ceps::ast::c_str(ceps::ast::Ast_node_kind k)
{
	return ast_node_kind_to_text[(int)k];
}


//////////////////////////////////

//////////////////////////////////  ceps::ast::Nodebase

std::ostream& ceps::ast::Nodebase::pretty_print(std::ostream& os)
{
	os.iword(iword_index) = true;
	return os;
}

 std::ostream& ceps::ast::Nodebase::pretty_print_off(std::ostream& os)
{
	os.iword(iword_index) = false;
	return os;
}

void ceps::ast::Nodebase::print_ws(std::ostream& out,int n) const
	{
		if (n < 0) return;
		for(int i = 0; i++ != n;) out << ' ';
	}




void ceps::ast::Nodebase::print(std::ostream& out,bool pretty_print,int indent) const
{
	out << "("<<ast_node_kind_to_text[(int)kind()] << ' ';
	print_content(out,pretty_print,indent);
	out << ")";
}

std::ostream& ceps::ast::operator << (std::ostream & out, Nodebase const & n)
{
	n.print(out,out.iword(Nodebase::iword_index),0);
	return out;
}

ceps::ast::Nodebase_ptr ceps::ast::box(int j)
 {
	 return  new Int(j,Unit_rep{});
 }

ceps::ast::Nodebase_ptr ceps::ast::box(unsigned int j)
 {
	 return  new Int((int)j,Unit_rep{});
 }


ceps::ast::Nodebase_ptr ceps::ast::box(long j)
 {
	 return  new Int64(j,Unit_rep{});
 }

ceps::ast::Nodebase_ptr ceps::ast::box(unsigned long j)
 {
	 return  new Int64((long)j,Unit_rep{});
 }

ceps::ast::Nodebase_ptr ceps::ast::box(double d)
  {
 	 return  new Double(d,Unit_rep{});
  }

ceps::ast::Nodebase_ptr ceps::ast::box(std::string const & s)
   {
  	 return  new String{s};
   }



Struct_ptr ceps::ast::make_struct(std::string const & name)
{
	return new Struct{name};
}

std::ostream& ceps::ast::operator << (std::ostream& out, strct & s)
{
	out << *s.get_root();
	return out;
}


