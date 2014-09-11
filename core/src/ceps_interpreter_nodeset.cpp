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

#include "ceps_interpreter.hh"
#include "ceps_interpreter_nodeset.hh"
#include "cepsnodeset.hh"
#include "symtab.hh"




static void flatten(ceps::ast::Nodebase_ptr root, std::vector<ceps::ast::Nodebase_ptr>& acc)
{

	/*base cases*/
	if (root == nullptr)
		return;
	if (root->kind() != ceps::ast::Ast_node_kind::binary_operator || op(as_binop_ref(root)) != '.' )
	{
		acc.push_back(root);
		return;
	}
	/*Induction case*/
	auto & op = as_binop_ref(root);
	flatten(op.children()[0],acc);

	flatten(op.children()[1],acc);
}

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate_nodeset_expr_dot(	ceps::ast::Nodebase_ptr lhs,
															ceps::ast::Nodebase_ptr rhs ,
															ceps::parser_env::Symboltable & sym_table,
															ceps::interpreter::Environment& env)
{
	//INVARIANT: lhs is a nodeset

	if (rhs == nullptr) return lhs;

	std::vector<ceps::ast::Nodebase_ptr> acc;
	flatten(rhs,  acc);
	ceps::ast::Nodeset result{as_ast_nodeset_ref(lhs).children()};
	std::string last_identifier;
	last_identifier = apply_idx_op_operand(as_ast_nodeset_ref(lhs));

	//std::cout << acc.size() << std::endl;

	for (size_t i = 0; i < acc.size(); ++i)
	{
		if (is_an_identifier(acc[i]))
		{
			auto id_name = name(as_id_ref(acc[i]));
			if (last_identifier.length() == 0)
				result = result[ceps::ast::all{id_name}];
			else
				result = result[last_identifier][ceps::ast::all{id_name}];
			last_identifier= id_name;
		} else
		{
			last_identifier = std::string{};
		}
	}//for

	return create_ast_nodeset(last_identifier, result.nodes());
}//ceps::interpreter::evaluate_nodeset_expr_dot





