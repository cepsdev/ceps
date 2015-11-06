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


#include "cepslexer.hh"
#include "cepsparserdriver.hh"



void read_number(std::string& s,std::istream& in,std::vector< std::set<char> > available_digits,int num_enc,char& ch,ceps::Cepsparser::location_type* yylloc = nullptr)
{

	for(;
			in.get(ch) && available_digits[num_enc].find(ch) != available_digits[num_enc].end();s += ch)
		if(yylloc) yylloc->end.column+=1;
}

ceps::Cepsparser::token_type yylex(
		ceps::Cepsparser::semantic_type* yylval,
		ceps::Cepsparser::location_type* yylloc ,
		Ceps_parser_driver& driver)
{
	using namespace std;
	enum Numeric_encoding{dec = 0, hex = 1, oct = 2};

	vector< set<char> > available_digits;/* = { {'0','1','2','3','4','5','6','7','8','9'}, //decimal digits
	                                       {'0','1','2','3','4','5','6','7','8','9','A','a','B','b','C','c','D','d','E','e','F','f'},
	                                       {'0','1','2','3','4','5','6','7'}
										};*/
	available_digits.push_back(set<char>());
	available_digits.push_back(set<char>());
	available_digits.push_back(set<char>());

	available_digits[0].insert('0');
	available_digits[0].insert('1');
	available_digits[0].insert('2');
	available_digits[0].insert('3');
	available_digits[0].insert('4');
	available_digits[0].insert('5');
	available_digits[0].insert('6');
	available_digits[0].insert('7');
	available_digits[0].insert('8');
	available_digits[0].insert('9');

	available_digits[1].insert('0');
	available_digits[1].insert('1');
	available_digits[1].insert('2');
	available_digits[1].insert('3');
	available_digits[1].insert('4');
	available_digits[1].insert('5');
	available_digits[1].insert('6');
	available_digits[1].insert('7');
	available_digits[1].insert('8');
	available_digits[1].insert('9');

	available_digits[1].insert('a');
	available_digits[1].insert('A');
	available_digits[1].insert('b');
	available_digits[1].insert('B');
	available_digits[1].insert('c');
	available_digits[1].insert('C');
	available_digits[1].insert('d');
	available_digits[1].insert('D');
	available_digits[1].insert('e');
	available_digits[1].insert('E');
	available_digits[1].insert('f');
	available_digits[1].insert('F');



	available_digits[2].insert('0');
	available_digits[2].insert('1');
	available_digits[2].insert('2');
	available_digits[2].insert('3');
	available_digits[2].insert('4');
	available_digits[2].insert('5');
	available_digits[2].insert('6');
	available_digits[2].insert('7');




	std::istream& in = driver.input_stream();
	char ch = EOF;
	if(yylval != NULL) yylval->sval = NULL;

	/*skip whitespace and comments*/
	for(;in;)
	{
		/*
		 * Remark:
		 * The evaluation of the guard-expression in the following for-loop is
		 * evaluated from left to right according to the C++ standard (ISO/IEC C++11,Section 5.14 [expr.log.and].
		 * See also section 6.2.2 in B.Stroustrup:"The C++ Programming Language",3rd ed.).
		 * So the variable "ch" is well defined when isspace is called.
		 *
		 */
		bool white_space_read = false;
		for(;in.get(ch) && std::isspace(ch);)
		{
			/*if (yylloc && ch == '\n') yylloc->lines(1);
			if (yylloc) yylloc->step();*/
			white_space_read = true;
			if (yylloc)
			{
				if (ch != '\n')
				  yylloc->end.column+=1;
				else
				{
					yylloc->end.line+=1;
					yylloc->end.column = 1;
				}
				//std::cout << "UPS\n";
			}
			if (ch == '\n' && driver.raw_mode())
				return ceps::Cepsparser::token::EOL;
		}
		/*if (white_space_read && yylloc) yylloc->end = yylloc->begin;
		else if (yylloc)yylloc->begin = yylloc->end;
		*/
		if (yylloc)
		{
			yylloc->begin.line = yylloc->end.line;
			yylloc->begin.column = yylloc->end.column;
		}

		if(ch == EOF)
			return ceps::Cepsparser::token::END;

		if (ch == '/')
		{
			char ch_temp = in.get();
			if (ch_temp != '/' && ch_temp != '*') // a single '/'
			{
				in.putback(ch_temp);
				break;
			}

			if ( (ch = ch_temp) == '/') // two '/'
			{
				for(;in.get(ch) && ch != '\n';);
				if(yylloc)
					{
						yylloc->end.column=1;
						yylloc->end.line+=1;
						yylloc->begin = yylloc->end;
					}
				if(!in || ch == EOF)
						return ceps::Cepsparser::token::END;
				continue;
			}
			else // start of block comment: '/*'
			{
				for(;in;)
				{
					for(;in.get(ch) && ch != '*';){
						if(yylloc)
							{
								if (ch != '\n') yylloc->end.column+=1;
								else {yylloc->end.column=1;yylloc->end.line+=1;}
							}
					}
					for(;in.get(ch) && ch == '*';)
					{
						if(yylloc) yylloc->end.column+=1;
					}
					if (ch == '/')
					{
						if(yylloc) yylloc->end.column+=1;
						break;
					}
				}
				if (yylloc)
						{
							yylloc->begin.line = yylloc->end.line;
							yylloc->begin.column = yylloc->end.column;
						}
				continue;
			}
		} else break;
	}//for(;in;)

	if (!in || ch == EOF)
		return ceps::Cepsparser::token::END;

	//INVARIANT: Input stream is valid. Next character in stream is neither whitespace nor belongs to a comment.

	if (isalpha(ch) || ch == '_' || ch == '$')
	{

		// struct,val,STRUCT-ID or STRUCT
		//if(yylloc) yylloc->begin = yylloc->end+1;
		std::string s;
		s += ch;
		for(;in.get(ch) && (isalnum(ch) || ch == '_' || ch == '$' || ch == '\'');)
		{
		 s += ch;
		 if (yylloc) yylloc->end+=1;
		}
		if (in && ch != EOF)
			in.putback(ch);

		if(yylval != NULL)
				{
					yylval->sval = new std::string;//FIXME
					*yylval->sval = s;
				}

		if (s == "struct")
			return ceps::Cepsparser::token::STRUCT;
		if (s == "val")
			return ceps::Cepsparser::token::VAL;
		if (s == "fun")
			return ceps::Cepsparser::token::FUN;
		if (s == "return")
				return ceps::Cepsparser::token::RET;
		if (s == "if")
					{return ceps::Cepsparser::token::IF;}
		if (s == "else")
					return ceps::Cepsparser::token::ELSE;
		if (s == "for")
		{

					return ceps::Cepsparser::token::FOR;
		}
		if (s == "kind")
		{
					return ceps::Cepsparser::token::KIND;
		}
		if (s == "raw_map")
		{
					driver.set_raw_mode(true);
					return ceps::Cepsparser::token::RAWMAP;
		}
		if (s == "template") return ceps::Cepsparser::token::TEMPLATE;


		auto symbol = driver.symboltable().lookup(s,false,false,false);
		if (symbol)
		{
			if (symbol->category == ceps::parser_env::Symbol::KIND)
				return ceps::Cepsparser::token::KINDID;

			return ceps::Cepsparser::token::STRUCTID;
		}


		return ceps::Cepsparser::token::STRUCTID;
	}
	else if (std::isdigit(ch) || ch == '.')
	{
		if (ch == '.')
		{
			char temp;
			in.get(temp);

			if (!std::isdigit(temp) ) {
				if (temp == '.') return ceps::Cepsparser::token::DOTDOT;
				else {in.putback(temp);return ceps::Cepsparser::token_type('.');}
			}
		}
		//NUMBER
		/*Numeric literals can have one of the following form:
		 *
		 * 12 (INTEGER,VALUE=12) 011 (INTEGER,OCTAL,VALUE=8) 0xA (INTEGER,HEX,VALUE=10)
		 *
		 * 1.0 (DOUBLE,VALUE=1.0) .1 (DOUBLE,VALUE=0.1) 1. (DOUBLE,VALUE=1.0) 1.0e-2 (DOUBLE,VALUE=0.01)
		 *
		 */
		Numeric_encoding num_enc = dec; // we assume a decimal encoding per default
		bool dot_read = ch == '.'; //dot_read indicates whether we have read a dot, if dot_read == true we have a floating point number
		std::string integral_part;
		std::string fractional_part;

		if (ch == '0')
		{
			char ch_temp = in.get();
			if (ch_temp == 'x')
				num_enc = hex;
			else if (std::isdigit(ch_temp))
			{
				num_enc = oct;
				in.putback(ch_temp);
			}
			else in.putback(ch_temp);
		}


		if (dot_read)
			read_number(fractional_part,in,available_digits,num_enc,ch,yylloc);
		else
		{
			if (ch != '0') integral_part += ch;
			read_number(integral_part,in,available_digits,num_enc,ch,yylloc);
			if((dot_read = ch =='.' ))
				read_number(fractional_part,in,available_digits,num_enc,ch,yylloc);
		}
		if (dot_read)
		{
			double factor = 1.0;

			if (ch == 'e' || ch == 'E')
			{
				//scientific notation
				double sign = 1.0;
				for(; in.get(ch) && ch == '-' ;sign *= -1.0) if(yylloc) yylloc->end.column+=1;
				if(in) in.putback(ch);
				string exp_rep = "";
				for(;in.get(ch) && isdigit(ch);exp_rep+=ch)if(yylloc) yylloc->end.column+=1;
				double exponent = 0.0;
				istringstream{exp_rep} >> exponent;
				factor = pow(10.0,sign*exponent);
			}
			double value = 0.0;
			istringstream is(integral_part+"."+fractional_part);
			if (num_enc == hex) is >> std::hex >> value;
			else if (num_enc == oct) is >> std::oct >> value;
			else is >> value;
			value *= factor;
			if(yylval != NULL) yylval->fval = value;
		}
		else
		{
			int value = 0;
			istringstream is(integral_part);
			if (num_enc == hex) is >> std::hex >> value;
			else if (num_enc == oct) is >> std::oct >> value;
			else is >> value;
			if(yylval != NULL) yylval->ival = value;
		}
		if(in) in.putback(ch);
		if (dot_read) return ceps::Cepsparser::token::FLOATNUM;
		else return ceps::Cepsparser::token::INTNUM;
	}
	else if (ch == '"') //A string literal
	{
		std::ostringstream in_str;
		for(;in.get(ch) && ch != '"';)
		{
			if(yylloc) yylloc->end.column+=1;
			if (ch == '\\')
			{
				in.get(ch);
				switch (ch)
				{
				case 'n' :
					in_str << '\n';
					break;
				case 'r':
					in_str << '\r';
					break;
				case 't':
					in_str << '\t';
					break;
				default:
					in_str << ch;
				}
			}
			else in_str << ch;
		}

		if(yylval != NULL)
		{
			yylval->sval = new std::string;//FIXME
			*yylval->sval = in_str.str();
		}
		return ceps::Cepsparser::token::LITERAL;
	}
	else
	{
		if (ch == '}' && driver.raw_mode())
			driver.set_raw_mode(false);
		if (ch == '|' || ch == '&'){
			char t;in.get(t);
			if (t == ch) return ceps::Cepsparser::token_type(ch);
			in.unget();
		} else if (ch == '>')	{
			char t;in.get(t);
			if (t == '=') return ceps::Cepsparser::token::REL_OP_GT_EQ;
			in.unget();
			return ceps::Cepsparser::token::REL_OP_GT;
		} else if (ch == '<')	{
			char t;in.get(t);
			if (t == '=') return ceps::Cepsparser::token::REL_OP_LT_EQ;
			in.unget();
			return ceps::Cepsparser::token::REL_OP_LT;
		} else if (ch == '!')	{
			char t;in.get(t);
			if (t == '=') return ceps::Cepsparser::token::REL_OP_NEQ;
			in.unget();return ceps::Cepsparser::token_type(ch);
		} else if (ch == '='){
			char t;in.get(t);
			if (t == '=') return ceps::Cepsparser::token::REL_OP_EQ;
			in.unget();return ceps::Cepsparser::token_type(ch);
		}

		return ceps::Cepsparser::token_type(ch);
	}
	return ceps::Cepsparser::token::END;
}

