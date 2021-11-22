/*
Copyright 2021 Tomas Prerovsky (cepsdev@hotmail.com).

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.elete
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/



#ifndef CEPS_INTERPRETER_LOOP_HH
#define CEPS_INTERPRETER_LOOP_HH

#include "ceps_ast.hh"
#include "ceps_interpreter.hh"


namespace ceps
{
	namespace interpreter{
		ceps::ast::Nodebase_ptr evaluate_loop(
		ceps::ast::Loop_ptr loop_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr rootnode,
		ceps::ast::Nodebase_ptr predecessor,
		bool& symbols_found,
		ceps::interpreter::thoroughness_t thoroughness
		);
	}
}

#endif

