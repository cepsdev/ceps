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

#include "ceps_interpreter_loop.hh"
#include "cepsnodeset.hh"
#include "symtab.hh"

#include "include_gen/ceps.tab.hh"




static void flatten(ceps::ast::Nodebase_ptr root, std::vector<ceps::ast::Nodebase_ptr>& acc,bool& is_range)
{

	is_range = false;
	/*base cases*/
	if (root == nullptr)
		return;
	if (root->kind() == ceps::ast::Ast_node_kind::binary_operator && op(as_binop_ref(root)) == ',' )
	{
		/*Induction case*/
		bool t;
		auto & op = as_binop_ref(root);
		flatten(op.children()[0],acc,t);

		flatten(op.children()[1],acc,t);

	}
	else if (root->kind() == ceps::ast::Ast_node_kind::binary_operator && op(as_binop_ref(root)) == ceps::Cepsparser::token::DOTDOT )
		{
			auto & op = as_binop_ref(root);
			acc.push_back(op.children()[0]);
			acc.push_back(op.children()[1]);
			is_range = true;
		}
	else if (ceps::ast::is_a_nodeset(root))
	{
		std::string last_identifier;
		last_identifier = apply_idx_op_operand(as_ast_nodeset_ref(root));

		for (auto & e : ceps::ast::as_ast_nodeset_ref(root).children())
		{
			std::vector<ceps::ast::Nodebase_ptr> v;
			v.push_back(e);
			acc.push_back(ceps::ast::create_ast_nodeset(last_identifier,v));
		}
	}
	else acc.push_back(root);


}

static void loop( std::vector<ceps::ast::Nodebase_ptr>& result,
		   ceps::ast::Nodebase_ptr body,
		   ceps::ast::Loop_head& loop_head,
		   int i,
		   ceps::parser_env::Symboltable & sym_table,
		   ceps::interpreter::Environment& env,ceps::ast::Nodebase_ptr rootnode,ceps::ast::Nodebase_ptr predecessor)
{
	using namespace ceps::parser_env;


	bool is_range_loop = false;
	bool last_head = i*2 +1 == loop_head.children().size() - 1;
	ceps::ast::Identifier& id  = ceps::ast::as_id_ref(loop_head.children()[2*i]);
	ceps::ast::Nodebase_ptr coll_  = evaluate(loop_head.children()[2*i+1],sym_table,env,&loop_head,loop_head.children()[2*i]);
	std::vector<ceps::ast::Nodebase_ptr> collection;




	flatten(coll_,collection,is_range_loop);

	sym_table.push_scope();

	ceps::parser_env::Symbol* sym_ptr;

	if ( (sym_ptr = sym_table.lookup(name(id),true,true,false)) == nullptr)
	{
		 throw ceps::interpreter::semantic_exception{body,"Variable '" +name(id)+"' could not be instantiated defined."};
	}
    //std::cout << "*****" << name(id) << std::endl;

	sym_ptr->category = Symbol::Category::VAR;

	if (is_range_loop)
	{
		int from = value(as_int_ref(collection[0]));
		int to = value(as_int_ref(collection[1]));
		int step = 1;
		if (from > to) step *= -1;
		ceps::ast::Int counter_node( from , ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		sym_ptr->payload = &counter_node;

		for(int h = from; step*h <= step*to ; h += step)
		{
			value(counter_node) = h;
			if (last_head)
			{
				auto new_node = evaluate(body,sym_table,env,rootnode,predecessor);
				if (new_node != nullptr)
				{
					if (new_node->kind() == ceps::ast::Ast_node_kind::stmts)
					{
						for (auto const & e : as_stmts_ref(new_node).children() )
							result.push_back(e);
					}
					else result.push_back(new_node);
				}

			} else loop (result,body,loop_head,i+1,sym_table,env,rootnode,predecessor);

		}

	}
	else for(auto col_node : collection)
	{
		sym_ptr->payload = col_node;//TODO: See comment in symtab.hh

		//std::cout << "###" <<ceps::ast::Nodeset{col_node} << std::endl;

		if (last_head)
		{

			auto new_node = evaluate(body,sym_table,env,rootnode,predecessor);
			if (new_node != nullptr)
			{
				if (new_node->kind() == ceps::ast::Ast_node_kind::stmts)
				{
					for (auto const & e : as_stmts_ref(new_node).children() )
						result.push_back(e);
				}
				else result.push_back(new_node);
			}

		}
		else
		{
			loop (result,body,loop_head,i+1,sym_table,env,rootnode,predecessor);
		}


	}//for

	sym_table.pop_scope();
}


/*
 *
 *  LOOP = LOOP_HEADER BODY
 *
 *
 *
 * */
ceps::ast::Nodebase_ptr  ceps::interpreter::evaluate_loop(ceps::ast::Loop_ptr loop_node,
									  ceps::parser_env::Symboltable & sym_table,
									  ceps::interpreter::Environment& env,ceps::ast::Nodebase_ptr rootnode,ceps::ast::Nodebase_ptr predecessor)
{
	const auto for_loop_head = 0;
	const auto for_loop_body = 1;



	auto& loop_head =  as_loop_head_ref(loop_node->children()[for_loop_head]);
	ceps::ast::Nodebase_ptr body = loop_node->children()[for_loop_body];

	std::vector<ceps::ast::Nodebase_ptr> result_vec;

	loop(result_vec, body, loop_head, 0, sym_table, env,rootnode,predecessor);

	ceps::ast::Stmts * result = new ceps::ast::Stmts{};
	for(auto p : result_vec)
		result->children().push_back(p);



	return result;
}






