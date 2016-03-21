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

extern std::string default_text_representation(ceps::ast::Nodebase_ptr root_node);


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
															ceps::interpreter::Environment& env,ceps::ast::Nodebase_ptr )
{
	//INVARIANT: lhs is a nodeset



	if (rhs == nullptr) return lhs;

	//std::cout << "######################### RHS:" << *rhs << std::endl << std::endl;
	//std::cout << "######################### LHS:" << *lhs << std::endl << std::endl;

	std::vector<ceps::ast::Nodebase_ptr> acc;
	flatten(rhs,  acc);
	ceps::ast::Nodeset result{as_ast_nodeset_ref(lhs).children()};
	std::string last_identifier;
	last_identifier = apply_idx_op_operand(as_ast_nodeset_ref(lhs));

	//std::cout <<"----------------------------- "<< acc.size() << std::endl;

	for (size_t i = 0; i < acc.size(); ++i)
	{
		std::string method_name;
		std::vector<ceps::ast::Nodebase_ptr> args;
		if (is_an_identifier(acc[i]))
		{
			auto id_name = name(as_id_ref(acc[i]));
			if (last_identifier.length() == 0)
				result = result[ceps::ast::all{id_name}];
			else
				result = result[last_identifier][ceps::ast::all{id_name}];
			last_identifier= id_name;

		} else if (is_a_funccall(acc[i],method_name,args)){
			last_identifier="";
			if (method_name == "content"){
				std::vector<ceps::ast::Nodebase_ptr> v;
				for(auto pe : result.nodes())
				{
					if (pe->kind() != ceps::ast::Ast_node_kind::structdef) continue;
					for(auto pes: as_struct_ref(pe).children()) v.push_back(pes);
				}
				result.nodes_ = v;
			} else if (method_name == "first"){
				if (result.nodes().size() == 0) result.nodes_.clear();
				else{
				std::vector<ceps::ast::Nodebase_ptr> v;
				v.push_back(result.nodes()[0]);
				result.nodes_ = v;
				}
			} else if (method_name == "at" && args.size() == 1){
			  auto r = is_int(args[0]);

			  if(!r.first) ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' expects integer as parameter."};
			  if (result.nodes().size() <= (size_t) r.second) throw ceps::interpreter::semantic_exception{nullptr,"Nodeset method '"+method_name+"': index out of bounds."};
			  std::vector<ceps::ast::Nodebase_ptr> t;
			  t.push_back(result.nodes()[r.second]);
			  result.nodes_=t;
			} else if (method_name == "is_kind" && args.size() == 1 && args[0]->kind() == ceps::ast::Ast_node_kind::string_literal){
			  std::string kind_name = value(as_string_ref(args[0]));
			  std::vector<ceps::ast::Nodebase_ptr> v;
			  for(auto pe : result.nodes())
			  {
			  	if (pe->kind() != ceps::ast::Ast_node_kind::symbol) continue;
			  	if (kind_name == kind(as_symbol_ref(pe))) v.push_back(pe);
			  }
			  result.nodes_ = v;
			} else if (method_name == "text_value_of_content_equals" && args.size() == 1 && args[0]->kind() == ceps::ast::Ast_node_kind::string_literal){
			  std::string cv = value(as_string_ref(args[0]));

			  std::vector<ceps::ast::Nodebase_ptr> v;

			  for(auto pe : result.nodes())
			  {

				  if (pe->kind() == ceps::ast::Ast_node_kind::string_literal) continue;
				  if (pe->kind() == ceps::ast::Ast_node_kind::int_literal) continue;
				  if (pe->kind() == ceps::ast::Ast_node_kind::float_literal) continue;
				  std::string c;
				  for(auto p: ceps::ast::nlf_ptr(pe)->children()){
					  c+=default_text_representation(p);
				  }
				  if (c!=cv) continue;
				  v.push_back(pe);
			  }
			  //if (v.size() != 1) throw ceps::interpreter::semantic_exception{nullptr,"first_struct_with_name(): no matching node found."};
			  result.nodes_ = v;
			}

			else throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' Unknown method/Invalid parameters for nodeset called."};
		}
		else
		{
			auto r_int = is_int(acc[i]);
			if(r_int.first)
			{
				//std::cout << "################GAGAGAG " << result << std::endl;
				result = result[ceps::ast::nth{r_int.second}];last_identifier = std::string{};

			}else {
				//std::cout << "!!!!!!!!!!!!!!!!!!!!" << (int)acc[i]->kind() << std::endl;
				last_identifier = std::string{};
			}
		}
	}//for

	return create_ast_nodeset(last_identifier, result.nodes());
}//ceps::interpreter::evaluate_nodeset_expr_dot





