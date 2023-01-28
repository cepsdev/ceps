/*
Copyright 2021 Tomas Prerovsky (cepsdev@hotmail.com).

Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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

static void fetch_recursively_symbols(std::vector<ceps::ast::Nodebase_ptr> const & in,std::vector<ceps::ast::Nodebase_ptr> & out);

static void fetch_recursively_symbols(ceps::ast::Nodebase_ptr elem,std::vector<ceps::ast::Nodebase_ptr> & out){
 if (elem->kind() == ceps::ast::Ast_node_kind::symbol) out.push_back(elem);
 else if (elem->kind() == ceps::ast::Ast_node_kind::structdef){
     fetch_recursively_symbols(ceps::ast::as_struct_ref(elem).children(),out);
 } else if (elem->kind() == ceps::ast::Ast_node_kind::ifelse){
     auto ifelse = ceps::ast::as_ifelse_ptr(elem);
     ceps::ast::Nodebase_ptr cond = ifelse->children()[0];
     fetch_recursively_symbols(cond,out);
     if (ifelse->children().size() > 1) fetch_recursively_symbols(ifelse->children()[1],out);
     if (ifelse->children().size() > 2) fetch_recursively_symbols(ifelse->children()[2],out);
 } else if (elem->kind() == ceps::ast::Ast_node_kind::binary_operator){
     auto & binop = ceps::ast::as_binop_ref(elem);
     fetch_recursively_symbols(binop.left(),out);fetch_recursively_symbols(binop.right(),out);
 }
}



static void fetch_recursively_symbols(std::vector<ceps::ast::Nodebase_ptr> const & in,std::vector<ceps::ast::Nodebase_ptr> & out){
 for(auto e : in)
  fetch_recursively_symbols(e,out);
}

extern std::string default_text_representation(std::vector<ceps::ast::Nodebase_ptr> nodes);


static bool less_than (ceps::ast::node_t a, ceps::ast::node_t b){
	using namespace ceps::ast;
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int_ref(a)) < value(as_int_ref(b));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::int_literal>(b))
                                   return value(as_double_ref(a)) < (double)value(as_int_ref(b));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return (double)value(as_int_ref(a)) < value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::float_literal>(b))
                                   return value(as_double_ref(a)) < value(as_double_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int64_ref(a)) < value(as_int64_ref(a));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int_ref(a)) < value(as_int64_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int64_ref(a)) < value(as_int_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return	value(as_int64_ref(a)) < value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_double_ref(a)) < value(as_int64_ref(a));

                               return default_text_representation(a) < default_text_representation(b);
}

static bool less_equal_than (ceps::ast::node_t a, ceps::ast::node_t b){
	using namespace ceps::ast;
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int_ref(a)) <= value(as_int_ref(b));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::int_literal>(b))
                                   return value(as_double_ref(a)) <= (double)value(as_int_ref(b));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return (double)value(as_int_ref(a)) <= value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::float_literal>(b))
                                   return value(as_double_ref(a)) <= value(as_double_ref(a));

                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int64_ref(a)) <= value(as_int64_ref(a));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int_ref(a)) <= value(as_int64_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int64_ref(a)) <= value(as_int_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return	value(as_int64_ref(a)) <= value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_double_ref(a)) <= value(as_int64_ref(a));

                               return default_text_representation(a) <= default_text_representation(b);
}

static bool greater_than (ceps::ast::node_t a, ceps::ast::node_t b){
	using namespace ceps::ast;
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int_ref(a)) > value(as_int_ref(b));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::int_literal>(b))
                                   return value(as_double_ref(a)) > (double)value(as_int_ref(b));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return (double)value(as_int_ref(a)) > value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::float_literal>(b))
                                   return value(as_double_ref(a)) > value(as_double_ref(a));

                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int64_ref(a)) > value(as_int64_ref(a));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int_ref(a)) > value(as_int64_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int64_ref(a)) > value(as_int_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return	value(as_int64_ref(a)) > value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_double_ref(a)) > value(as_int64_ref(a));

                               return default_text_representation(a) > default_text_representation(b);
}

static bool greater_equal_than (ceps::ast::node_t a, ceps::ast::node_t b){
	using namespace ceps::ast;
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int_ref(a)) >= value(as_int_ref(b));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::int_literal>(b))
                                   return value(as_double_ref(a)) >= (double)value(as_int_ref(b));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return (double)value(as_int_ref(a)) >= value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::float_literal>(b))
                                   return value(as_double_ref(a)) >= value(as_double_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int64_ref(a)) >= value(as_int64_ref(a));
                               if (is<Ast_node_kind::int_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_int_ref(a)) >= value(as_int64_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::int_literal>(b) )
                                   return value(as_int64_ref(a)) >= value(as_int_ref(a));
                               if (is<Ast_node_kind::long_literal>(a) && is<Ast_node_kind::float_literal>(b) )
                                   return	value(as_int64_ref(a)) >= value(as_double_ref(a));
                               if (is<Ast_node_kind::float_literal>(a) && is<Ast_node_kind::long_literal>(b) )
                                   return value(as_double_ref(a)) >= value(as_int64_ref(a));


                               return default_text_representation(a) >= default_text_representation(b);
}

ceps::ast::Nodebase_ptr ceps::interpreter::evaluate_nodeset_expr_dot(	
	ceps::ast::Nodebase_ptr lhs,
															ceps::ast::Nodebase_ptr rhs ,
															ceps::parser_env::Symboltable & sym_table,
															ceps::interpreter::Environment& env,
															ceps::ast::Nodebase_ptr, 
															ceps::interpreter::thoroughness_t thoroughness,
															bool& symbols_found)
{
	using namespace ceps::ast;
	//INVARIANT: lhs is a nodeset

	if (rhs == nullptr) return lhs;

	std::vector<ceps::ast::Nodebase_ptr> acc;
	flatten(rhs,  acc);
	ceps::ast::Nodeset result{as_ast_nodeset_ref(lhs).children()};
	std::string last_identifier;
	last_identifier = apply_idx_op_operand(as_ast_nodeset_ref(lhs));

	for (size_t i = 0; i < acc.size(); ++i)
	{
		std::string method_name;
		std::vector<ceps::ast::Nodebase_ptr> args;

		if (is_an_identifier(acc[i]))
		{
			auto id_name = name(as_id_ref(acc[i]));

            if (last_identifier.length() == 0){
				result = result[ceps::ast::all{id_name}];
            }
            else {
				result = result[last_identifier][ceps::ast::all{id_name}];
            }
			last_identifier= id_name;
		} else if (is_a_simple_funccall(acc[i],method_name,args)){
            auto last_identifier_save = last_identifier;
            last_identifier = "";
            if (method_name == "select"){
                if (args.size() != 1 || args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
                    throw ceps::interpreter::semantic_exception{nullptr,"'select' missing/wrong argument."};;
                last_identifier = last_identifier_save;
                auto id_name = value(as_string_ref(args[0]));
                if (last_identifier.length() == 0){
                    result = result[ceps::ast::all{id_name}];
                }
                else {
                    result = result[last_identifier][ceps::ast::all{id_name}];
                }
                last_identifier= id_name;
            } else if (method_name == "content"){
				std::vector<ceps::ast::Nodebase_ptr> v;
				for(auto pe : result.nodes())
				{
					if (pe->kind() != ceps::ast::Ast_node_kind::structdef) continue;
					for(auto pes: as_struct_ref(pe).children()) v.push_back(pes);
				}
				result.nodes_ = v;
			} else if (method_name == "strip"){
				if(result.nodes().size() == 0) return nullptr;
				return result.nodes()[0];
			} else if (method_name == "size"){
				return new ceps::ast::Int(result.nodes().size(),ceps::ast::all_zero_unit(),nullptr,nullptr,nullptr);
			} else if (method_name == "first"){
				if (result.nodes().size() == 0) result.nodes_.clear();
				else{
				std::vector<ceps::ast::Nodebase_ptr> v;
				v.push_back(result.nodes()[0]);
				result.nodes_ = v;
				}
			} else if (method_name == "contains") {
				std::vector<ceps::ast::Nodebase_ptr> v;
				for(auto e: args){
					if (v.size()) break;
					if (is<Ast_node_kind::string_literal>(e)){
						auto s = value(as_string_ref(e));
						for(auto n:result.nodes()){
							if (!is<Ast_node_kind::string_literal>(n)) continue;
							if ( s == value(as_string_ref(n)) ){
							 v = {mk_int_node(1)};
							 break;
							}
						}
					}
				}
				if (v.size() == 0) v = {mk_int_node(0)};
				result.nodes_ = v;
			} else if (method_name == "contains_symbol"){
				std::vector<std::string> symbol_kinds;
				bool match_any_symbol = symbol_kinds.size() == 0;

				auto match_symbol = [&](std::string symbol_kind) -> bool {
					for(auto e : symbol_kinds) 
					 if (e == symbol_kind) return true;
					return false;
				};

				std::vector<ceps::ast::Nodebase_ptr> v;
				for(auto e : result.nodes())
				{
					if (ceps::ast::is_leaf(e->kind())) continue;
					auto & elements = nlf_ptr(e)->children();
					for (auto f: elements){
						if ( !is_a_symbol(f) ) continue;
						if (match_any_symbol) {
							v.push_back(e);break;
						} else if (match_symbol( kind(as_symbol_ref(f) )) ){
							v.push_back(e);break;
						}
					}					
				}
				result.nodes_ = v;				
			}  else if (method_name == "last"){
				if (!result.nodes().size()) result.nodes_.clear();
				else{
					std::vector<ceps::ast::Nodebase_ptr> v;
					v.push_back(result.nodes()[result.nodes().size()-1]);
					result.nodes_ = v;
				}
			} else if (method_name == "second"){
				if (result.nodes().size() < 2) result.nodes_.clear();
				else{
				std::vector<ceps::ast::Nodebase_ptr> v;
				v.push_back(result.nodes()[1]);
				result.nodes_ = v;
				}
			} else if (method_name == "slice"){
				std::vector<ceps::ast::Nodebase_ptr> t;
				if(!args.size()){
					// no op
				} else if( 1 == args.size()){
					 auto range_start = is_int(args[0]);
					 if(!range_start.first) throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' expects integer as parameter."};
					 if ((size_t)range_start.second < result.nodes().size()){
						 std::copy(result.nodes().begin()+range_start.second, result.nodes().end(),std::back_inserter(t));				 
					 }
					 result.nodes_ = t;
				} else {
					 auto range_start = is_int(args[0]); 
					 auto range_end = is_int(args[1]);
					 if(!range_start.first) throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' expects integer as parameter."};
					 if(!range_end.first) throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' expects integer as parameter."};
					 if ((size_t)range_start.second < result.nodes().size()){
						if (range_start.second <= range_end.second )
						{
						  if((size_t)range_end.second < result.nodes().size()) 
						   std::copy(result.nodes().begin()+range_start.second, result.nodes().begin()+range_end.second,std::back_inserter(t));
						  else
						   std::copy(result.nodes().begin()+range_start.second, result.nodes().end(),std::back_inserter(t));
						}
					 }
					 result.nodes_ = t;
				} 
            } else if (method_name == "at" && args.size() == 1){
			  auto r = is_int(args[0]);

              if(!r.first) throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' expects integer as parameter."};
			  if (result.nodes().size() <= (size_t) r.second) throw ceps::interpreter::semantic_exception{nullptr,"Nodeset method '"+method_name+"': index out of bounds."};
			  std::vector<ceps::ast::Nodebase_ptr> t;
			  t.push_back(result.nodes()[r.second]);
			  result.nodes_=t;
            } else if (method_name == "replace_with" && args.size() == 2){
			  auto r = is_int(args[0]);

              if(!r.first) throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' expects integer as parameter."};
			  if (result.nodes().size() <= (size_t) r.second) throw ceps::interpreter::semantic_exception{nullptr,"Nodeset method '"+method_name+"': index out of bounds."};
			  result.nodes_[r.second] = args[1];
            }else if (method_name == "fetch_recursively_symbols") {
                std::vector<ceps::ast::Nodebase_ptr> v;
                fetch_recursively_symbols(result.nodes_,v);
                result.nodes_ = v;
            } else if (method_name == "to_text") {
                std::vector<ceps::ast::Nodebase_ptr> v;
                for(auto p: result.nodes_)
                   v.push_back(ceps::ast::mk_string(default_text_representation(p)));
                result.nodes_ = v;				
            } else if (method_name == "sort") {
                last_identifier = last_identifier_save;
                if (args.size () == 0)
                 std::sort(result.nodes_.begin(),
                          result.nodes_.end(),
                          [](ceps::ast::Nodebase_ptr a,ceps::ast::Nodebase_ptr b ) {
                            return default_text_representation(a) < default_text_representation(b);
                          });
                else if (args.size() >= 1 && args[0]->kind() == ceps::ast::Ast_node_kind::string_literal){
                    auto const & key = value(ceps::ast::as_string_ref(args[0]));
					auto comp = less_than;

					if (args.size() >= 2 && is<Ast_node_kind::binary_operator>(args[1])){
						auto s = op_val(as_binop_ref(args[1]));
						if (s == ">") comp = greater_than;
						else if (s == ">=") comp = greater_equal_than;
						else if (s == "<=") comp = less_equal_than;
					}
                   	
					std::sort(result.nodes_.begin(),
                             result.nodes_.end(),
                             [key,comp](ceps::ast::Nodebase_ptr a_,ceps::ast::Nodebase_ptr b_ ) {
                               using namespace ceps::ast;
                               if (a_->kind() != ceps::ast::Ast_node_kind::structdef || b_->kind() != ceps::ast::Ast_node_kind::structdef)
                                   throw ceps::interpreter::semantic_exception{nullptr,"'sort' type mismatch."};
                               auto ans = ceps::ast::Nodeset{as_struct_ref(a_).children()}[key];
                               auto bns = ceps::ast::Nodeset{as_struct_ref(b_).children()}[key];
                               if (ans.nodes().size() == 0 || bns.nodes().size() == 0 )
                                   throw ceps::interpreter::semantic_exception{nullptr,"'sort' illformed key."};
                               auto a = ans.nodes()[0];auto b = bns.nodes()[0];
							   return comp(a,b);
                    });
                }
            } else if (method_name == "unique") {
                last_identifier = last_identifier_save;
                auto it = std::unique(result.nodes_.begin(),  result.nodes_.end(),[](ceps::ast::Nodebase_ptr a,ceps::ast::Nodebase_ptr b ) {return default_text_representation(a) == default_text_representation(b); });
                result.nodes_.erase(it,result.nodes_.end());
            } else if (method_name == "map") {
				if (args.size() != 2) throw ceps::interpreter::semantic_exception{nullptr,"'"+method_name+"' wrong number of arguments."};
				ceps::ast::Nodebase_ptr r = nullptr;
				ceps::ast::Nodebase_ptr a0 = args[0];
				ceps::ast::Nodebase_ptr a1 = args[1];
				if (a0->kind() == ceps::ast::Ast_node_kind::nodeset)
				 if (ceps::ast::as_ast_nodeset_ref(a0).children().size())
					 a0 = ceps::ast::as_ast_nodeset_ref(a0).children()[0];

				if (a1->kind() == ceps::ast::Ast_node_kind::nodeset)
				 if (ceps::ast::as_ast_nodeset_ref(a1).children().size())
					 a1 = ceps::ast::as_ast_nodeset_ref(a1).children()[0];

				for(size_t i = 0; i != result.nodes().size(); i+=2){
					auto v = result.nodes()[i];

					if (v->kind() != a0->kind()) continue;
					if (v->kind() == ceps::ast::Ast_node_kind::identifier)
						if (ceps::ast::name(ceps::ast::as_id_ref(v)) == ceps::ast::name(ceps::ast::as_id_ref(a0))){
							r = result.nodes()[i+1];
							break;
						}
				}
				if (r == nullptr) r = a1;
				result.nodes_ = std::vector<ceps::ast::Nodebase_ptr> {r};
			} else if (method_name == "is_string") {
				  std::vector<ceps::ast::Nodebase_ptr> v;
				  for(auto pe : result.nodes())
				  {
				  	if (pe->kind() == ceps::ast::Ast_node_kind::string_literal) v.push_back(pe);
				  }
				  result.nodes_ = v;

            }else if (method_name == "is_struct") {
			  std::vector<ceps::ast::Nodebase_ptr> v;
			  for(auto pe : result.nodes())
			  {
			  	if (pe->kind() == ceps::ast::Ast_node_kind::structdef) v.push_back(pe);
			  }
			  result.nodes_ = v;
            }else if (method_name == "is_id") {
              std::vector<ceps::ast::Nodebase_ptr> v;
              for(auto pe : result.nodes())
                  if (pe->kind() == ceps::ast::Ast_node_kind::identifier) v.push_back(pe);
                result.nodes_ = v;
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
			  result.nodes_ = v;
			} else {
				ceps::ast::Nodeset t;
				std::vector<node_t> v;
				if (last_identifier_save.length() == 0)
					t = result;
				else
					t = result[last_identifier_save];

				if(t.nodes().size() == 0) 
				 throw ceps::interpreter::semantic_exception{nullptr,
						"'"+method_name+"' Unknown method/Invalid parameters for nodeset called."};

				for(auto e : t.nodes()) {
					//std::cerr << *e << '\n';
					sym_table.push_scope();
					if (!is<Ast_node_kind::macro_definition>(e)) continue;
					auto& macrodef{ceps::ast::as_macrodef_ref(e)};
					auto body = ceps::ast::body(macrodef);
					if(!body) continue;	
					auto result =  eval_macro_no_nodeset(nullptr,
					  body,
		 			  sym_table,
		 			  env,
		 			  nullptr,
		 			  nullptr,
					  thoroughness,
					  symbols_found, &args);
					  if (result.size())
						v.insert(v.end(), result.begin(), result.end());
					sym_table.pop_scope();
				}
				result = Nodeset{v};
			}
		}
		else
		{
			auto r_int = is_int(acc[i]);
			if(r_int.first)
			{
				result = result[ceps::ast::nth{r_int.second}];last_identifier = std::string{};

			}else {
				last_identifier = std::string{};
			}
		}
	}//for
	return create_ast_nodeset(last_identifier, result.nodes());
}//ceps::interpreter::evaluate_nodeset_expr_dot





