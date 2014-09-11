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

#include <ceps_all.hh>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;
using namespace ceps::ast;

int main(int argc, char * argv[])
{

	ceps::Ceps_Environment ceps_env{""};
	cout << "nodesets: ";

	for(int i = 1; i < argc;++i)
	{
		cout << ".";
		ceps::ast::Root universe_;

		string filename{argv[i]};
		std::fstream test_file{filename};
		if (!test_file)
				throw runtime_error{"Couldn't open file '"+filename+"'"};
		Ceps_parser_driver driver{
				ceps_env.get_global_symboltable(),
						test_file};
		ceps::Cepsparser parser{driver};

		if (parser.parse() != 0)
				throw std::runtime_error{"A parser exception occured in file '"+filename+"'"};

		if (driver.errors_occured())
				throw std::runtime_error{"A parser exception occured in file '"+filename+"'"};

		Nodeset universe;

		ceps::interpreter::evaluate(universe, driver.parsetree().get_root(),ceps_env.get_global_symboltable(),ceps_env.interpreter_env());

		auto test = universe["test"];
		auto expectation = universe["expectation"];

		stringstream test_ss;
		test_ss << test;
		stringstream expectation_ss;
		expectation_ss << expectation;

		if (test_ss.str() != expectation_ss.str()) {cout << "[FAILED]\n";return 1;}

		}//for
	cout << "[PASSED]\n";
	return 0;
}




