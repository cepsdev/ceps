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

#include "ceps_serialize.hh"
#include <sstream>
#include <set>

void ceps::ast::helper_serialize_print_ws(std::ostream& os,int n)
{
	for(int i = 0; i < n;++i)
		os << " ";
}

std::string ceps::ast::helper_cast_string(std::string const & s)
{
	std::string result;
	for(unsigned int i = 0; i != s.length();++i)
	{
		if (s[i] == '\n')
			result+="\\n";
		else if (s[i] == '\\')
			result+="\\\\";
		else if (s[i] == '\t')
			result+="\\t";
		else if (s[i] == '\r')
			result+="\\r";
		else if (s[i] == '"')
			result+="\\\"";
		else if (std::isprint(s[i]))
			result += s[i];
	}
	return result;
}

void ceps::ast::serialize(std::ostream & os, Unit_rep const & u)
{
	if (u.m)
		os << "*m^"<<(int)u.m;
	if (u.kg)
		os << "*kg^"<<(int)u.kg;
	if (u.s)
		os << "*s^"<<(int)u.s;
	if (u.ampere)
		os << "*ampere^"<<(int)u.ampere;
	if (u.kelvin)
		os << "*kelvin^"<<(int)u.kelvin;
	if (u.mol)
		os << "*mol^"<<(int)u.mol;
	if (u.candela)
		os << "*cd^"<<(int)u.candela;
}

void ceps::ast::serialize(std::ostream & os, String & s,int indent,std::string const & eol)
{
	helper_serialize_print_ws(os,indent);
	os <<"\""<< helper_cast_string(value(s))<< "\"" <<eol;
}

void ceps::ast::serialize(std::ostream & os, Int & v,int indent,std::string const & eol,bool with_unit)
{
	helper_serialize_print_ws(os,indent);
	os << value(v);
	if ( with_unit && unit(v) != all_zero_unit())
	{
		serialize(os, unit(v) );
	}
	os <<eol;
}

void ceps::ast::serialize(std::ostream & os, Double & v,int indent,std::string const & eol,bool with_unit)
{
	helper_serialize_print_ws(os,indent);
	std::stringstream str;
	str.precision(21);
	str << std::showpoint << value(v);
	os << str.str();
	if (with_unit && unit(v) != all_zero_unit())
	{
		serialize(os, unit(v) );
	}
	os <<eol;
}

void ceps::ast::serialize(std::ostream & os, Struct_ptr s, int indent)
{
	std::set<std::string> struct_names;
	os << name(*s) << "{\n";
	++indent;
	for(Nodebase_ptr n : s->children())
	{
		if (n->kind() == Ast_node_kind::structdef)
		{
			struct_names.insert(name(as_struct_ref(n)));
		}
	}
	for (std::string const & s_n : struct_names)
	{
		helper_serialize_print_ws(os,indent);
		os << "struct "<<s_n<<";\n";
	}
	for(Nodebase_ptr n : s->children())
	{
		if (n->kind() == Ast_node_kind::structdef)
		{
			serialize(os,as_struct_ptr(n),indent);
		}
		else if(n->kind() == Ast_node_kind::int_literal)
		{
			serialize(os,as_int_ref(n),indent,";\n");
		}
		else if(n->kind() == Ast_node_kind::float_literal)
		{
			serialize(os,as_double_ref(n),indent,";\n");
		}
		else if(n->kind() == Ast_node_kind::string_literal)
		{
			serialize(os,as_string_ref(n),indent,";\n");
		}
		else if(n->kind() == Ast_node_kind::rawmap)
		{
			helper_serialize_print_ws(os,indent);
			os << "raw_map{\n";
			++indent;
			for(Nodebase_ptr atoms_ : as_rawmap_ptr(n)->children())
			{
				if (atoms_->kind() != Ast_node_kind::atoms) continue;
				helper_serialize_print_ws(os,indent);
				for (Nodebase_ptr node : nlf_ptr(atoms_)->children())
				{
					if (node->kind() == Ast_node_kind::string_literal)
						serialize(os,as_string_ref(node),0,"  ");
					else if (node->kind() == Ast_node_kind::int_literal)
						serialize(os,as_int_ref(node),0,"  ",false);
					else if (node->kind() == Ast_node_kind::float_literal)
						serialize(os,as_double_ref(node),0,"  ",false);
				}
				os << "\n";
			}
			--indent;
			helper_serialize_print_ws(os,indent);os << "};\n";
		}
	}
	--indent;
	helper_serialize_print_ws(os,indent);os << "};\n";
}

void ceps::ast::serialize(std::ostream & os,std::initializer_list<strct> l)
{
	std::set<std::string> struct_names;
	for(strct const & s:l)
	{
		struct_names.insert(name(*s.p_strct));
	}
	for(std::string const & n : struct_names)
	{
		os << "struct "<<n<<";\n";
	}
	for(strct const & s:l)
	{
		serialize(os,s.p_strct,0);
	}
}

