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


#include "ceps_all.hh"


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include <mutex>
#include <map>
#include <thread>
#include <sstream>
#include <fstream>

#define VERSION_CEPSVALIDATE_MAJOR 0
#define VERSION_CEPSVALIDATE_MINOR 5

using namespace std;
using namespace ceps::ast;

const bool DEBUG = false; 

std::string help_text = 
R"(
Usage: cepsvalidate FILE [FILE...] [--evaluate|-e] [--print-evaluated|-pe] [--print-raw|-pr] [--pretty-print|-pp] [--version|-v] [--verbose|-v]
)";



int main(int argc, char*argv[])
{
	
	bool evaluate = false;
	bool print_evaluated = false;
	bool print_raw = false;
	bool print_pretty = false;
	bool print_version = false;
	bool print_verbose = false;

	std::vector<std::string> files;
	if (argc == 1)
	{
		std::cout << help_text << std::endl;
		return EXIT_SUCCESS;
	}
	
	for (int i = 1; i < argc;++i)
	{
		string arg{argv[i]};
		if (arg == "-e" || arg == "--evaluate")
		{
			evaluate = true;
		}
		else if (arg == "-pp" || arg == "--pretty-print")
		{
			print_pretty  = true;
		}
		else if (arg == "-pe" || arg == "--print-evaluated")
		{
			print_evaluated = true;
		}
		else if (arg == "-pr" || arg == "--print-raw")
		{
			print_raw = true;
		}
		else if (arg == "--version")
		{
			print_version = true;
		}
		else if (arg == "--verbose" || arg == "-v")
		{
			print_verbose = true;
		}
		else {
			
			if (!ifstream{arg})
			{
				std::cerr << "\n***Error: Couldn't open file '" << arg << "' " << std::endl;
				return EXIT_FAILURE;
			}
			files.push_back(arg);
		}
	}

	if (print_version)
	{

#ifdef __GNUC__
		std::cout << "\n"
			<< "VERSION " <<  VERSION_CEPSVALIDATE_MAJOR << "." << VERSION_CEPSVALIDATE_MINOR << " (" __DATE__ << ") BUILT WITH GCC " << "" __VERSION__ "" << " on GNU/LINUX "
#ifdef __LP64__
			<< "64BIT"
#else
			<< "32BIT"
#endif
			<< "\n(C) BY THE AUTHORS OF ceps\n" << std::endl;
#else 
#ifdef _MSC_FULL_VER
		std::cout << "\n"
			<< "VERSION " << VERSION_CEPSVALIDATE_MAJOR << "." << VERSION_CEPSVALIDATE_MINOR << " (" __DATE__ << ") BUILT WITH MS VISUAL C++ " << _MSC_FULL_VER << " FOR WINDOWS "
#ifdef _WIN64
			<< "64BIT"
#else
			<< "32BIT"
#endif
			<< "\n(C) BY THE AUTHORS OF ceps\n" << std::endl;
#endif
#endif


	}

	ceps::Ceps_Environment ceps_env{""};
	Nodeset universe;
	for(std::string const & filename : files)
	{
		ifstream in{filename};
		if (!in)
		{
			std::cerr << "\n***Error: Couldn't open file '" << filename << "' " << std::endl;
			return EXIT_FAILURE;
		}


		try{
			Ceps_parser_driver driver(ceps_env.get_global_symboltable(),in);
			ceps::Cepsparser parser(driver);

			if (parser.parse() != 0)
				continue;

			if (driver.errors_occured())
				continue;

			auto root = ceps::ast::nlf_ptr(driver.parsetree().get_root());

			char buffer[PATH_MAX] = {};
			if ( buffer != realpath(filename.c_str(),buffer) ){
				std::cerr << "\n***Error: realpath() failed for '" << filename << "' " << std::endl;
				return EXIT_FAILURE;
			}


			root->children().insert(root->children().begin(),new ceps::ast::Struct("@@file",new ceps::ast::String(std::string{buffer}),nullptr,nullptr));

			if (print_raw)
			{
				if (print_pretty )
					std::cout << ceps::ast::Nodebase::pretty_print <<  *driver.parsetree().get_root() << std::endl << std::endl;
				else
					std::cout << *driver.parsetree().get_root() << std::endl << std::endl;
			}
			//ceps_env.eval_and_merge( driver.parsetree() , false);
			if (evaluate)
			{
				/*ceps::ast::Nodebase_ptr p = ceps::interpreter::evaluate(	driver.parsetree().get_root(),
																			ceps_env.get_global_symboltable(),
																			ceps_env.interpreter_env());*/

				std::vector<ceps::ast::Nodebase_ptr> generated_nodes;

				ceps::interpreter::evaluate(universe,
						                    driver.parsetree().get_root(),
						                    ceps_env.get_global_symboltable(),
						                    ceps_env.interpreter_env(),&generated_nodes);

				auto p = new Root();
				//p->children().insert(p->children().end(), universe.nodes().begin(), universe.nodes().end());
				p->children().insert(p->children().end(), generated_nodes.begin(), generated_nodes.end());


				if (print_evaluated)
				{
					if (print_verbose){
					 if (print_pretty )
						std::cout << ceps::ast::Nodebase::pretty_print <<  *p << std::endl;
					 else
						std::cout << *p << std::endl;
					} else {
						for(auto pp: p->children()){
							if (pp->kind() == ceps::ast::Ast_node_kind::structdef && ceps::ast::name(ceps::ast::as_struct_ref(pp)).substr(0,2) == "@@" ) continue;
							 if (print_pretty )
								std::cout << ceps::ast::Nodebase::pretty_print <<  *pp << std::endl;
							 else
								std::cout << *pp;
						}
					}
				}
			}
		} catch (ceps::interpreter::semantic_exception & se)
		{
			std::cerr << "[ERROR][Interpreter]:"<< se.what() << std::endl;
		}
		catch (std::runtime_error & re)
		{
			std::cerr << "[ERROR][System]:"<< re.what() << std::endl;
		}
	}

	
}
