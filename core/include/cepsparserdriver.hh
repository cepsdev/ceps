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

#ifndef YAMDLPARSERDRIVER_HH_
#define YAMDLPARSERDRIVER_HH_

#include <istream>
#include "symtab.hh"
#include "include_gen/ceps.tab.hh"
#include "ceps_ast.hh"

class Ceps_parser_driver
{
	typedef ceps::parser_env::Symboltable Symboltable;
public:
	struct err_entry
	{
		ceps::Cepsparser::location_type loc;
		std::string msg;
	};
private:
	std::vector<err_entry> err_log_;
	bool cerr_output_ = true;
public:
	void cerr_output(bool b)
	{
		cerr_output_ = b;
	}
	bool cerr_output() const
	{
		return cerr_output_;
	}
	void push_err(err_entry err)
	{
		err_log_.push_back(err);
	}
	void push_err(ceps::Cepsparser::location_type loc, std::string msg)
	{
		push_err(err_entry{loc,msg});
	}
	bool errors_occured() const
	{
		return err_log_.size() > 0;
	}
	void clear_err_log()
	{
		err_log_.clear();
	}
	Ceps_parser_driver(Symboltable& symboltable,
						std::istream& inputstream = std::cin)
		:
			symtab(symboltable),
			is(inputstream),
			parsetree_(NULL),
			raw_mode_(false)
			{

			}
	std::istream& input_stream()
		{
			return is;
		}
	Symboltable& symboltable()
		{
			return symtab;
		}
	void error(const ceps::Cepsparser::location_type& l, const std::string& m);
	ceps::ast::Parsetree parsetree()
		{
			return parsetree_;
		}
	void set_parsetree(ceps::ast::Parsetree pt)
		{
			parsetree_ = pt;
		}

	/*
	 * Recognized by the lexer, if in raw mode eof line characters are treated as tokens (EOL).
	 * Used in the context of raw data like raw_map.
	 * */
	bool raw_mode() const
	{
		return raw_mode_;
	}

	/**
	 * Sets raw mode on(b=true)/off(b=false). Returns the old state.
	 */
	bool set_raw_mode(bool b)
	{
		bool temp = raw_mode_;
		raw_mode_ = b;
		return temp;
	}
private:
	Symboltable& symtab;
	std::istream& is;
	ceps::ast::Parsetree parsetree_;
	bool raw_mode_;

};



#endif /* RMDLPARSERDRIVER_HH_ */
