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
#include "symtab.hh"
#include <cmath>
#include "ceps_interpreter_loop.hh"
#include "ceps_interpreter_nodeset.hh"
#include "pugixml.hpp"
#include <random>
#include <unordered_map>
#include <time.h>

extern char **environ;

static void traverse_xml(ceps::ast::Nonleafbase* root, const pugi::xml_node & xn);
static void flatten_args(ceps::ast::Nodebase_ptr r, std::vector<ceps::ast::Nodebase_ptr>& v, char op_val = ',')
{
	using namespace ceps::ast;
	if (r == nullptr) return;
	if (r->kind() == ceps::ast::Ast_node_kind::binary_operator && op(as_binop_ref(r)) ==  op_val)
	{
		auto& t = as_binop_ref(r);
		flatten_args(t.left(),v,op_val);
		flatten_args(t.right(),v,op_val);
		return;
	}
	v.push_back(r);
}

static ceps::ast::Nodebase_ptr include_xml_file(std::string path){

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (result.status != pugi::xml_parse_status::status_ok) throw ceps::interpreter::semantic_exception{nullptr,"include_xml: Invaid format"};

	auto r = new ceps::ast::Stmts();
	for(auto xn: doc.children())
	{
		traverse_xml(r,xn);
	}
	return r;
}

ceps::ast::Nodebase_ptr ceps::ast::read_xml_file(std::string path){
	return include_xml_file(path);
}



static ceps::ast::Nodebase_ptr include_xml_file(std::string path,std::string xpath){
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());
	if (result.status != pugi::xml_parse_status::status_ok) throw ceps::interpreter::semantic_exception{nullptr,"include_xml: Invaid format"};
	auto r = new ceps::ast::Stmts();
	auto nodes = doc.select_nodes(xpath.c_str());
	for(auto xn: nodes)
	{
		traverse_xml(r,xn.node());
	}
	return r;
}

static void traverse_xml(ceps::ast::Nonleafbase* root, const pugi::xml_node & xn){

	if (xn.type() == pugi::xml_node_type::node_pcdata)
	{
		root->children().push_back(new ceps::ast::String(xn.value(),nullptr,nullptr,nullptr));
		return;
	}

	auto t = new ceps::ast::Struct(xn.name());
	root->children().push_back(t);
	for(auto attr:xn.attributes()){
		std::string id = attr.name();
		std::string value = attr.value();
		auto tt = new ceps::ast::Struct("@"+id);t->children().push_back(tt);
		tt->children().push_back(new ceps::ast::String(value,nullptr,nullptr,nullptr));
	}
	for(auto x:xn.children())
		{
			//if (xn.value()) std::cout << x
			traverse_xml(t,x);
		}
}

static void default_text_representation_impl(std::stringstream& ss,ceps::ast::Nodebase_ptr root_node, bool enable_check_for_html = false){
	if (root_node->kind() == ceps::ast::Ast_node_kind::identifier) {
		ss << name(as_id_ref(root_node));
	} else if (root_node->kind() == ceps::ast::Ast_node_kind::string_literal) {
		ss << value(as_string_ref(root_node));
	} else if (root_node->kind() == ceps::ast::Ast_node_kind::int_literal) {
		ss << value(as_int_ref(root_node));
	} else if (root_node->kind() == ceps::ast::Ast_node_kind::float_literal) {
		ss << value(as_double_ref(root_node));
	} else if (root_node->kind() == ceps::ast::Ast_node_kind::binary_operator) {
		auto& binop = ceps::ast::as_binop_ref(root_node);
        ss << "(";
        default_text_representation_impl(ss,binop.left());
		char buffer[2] = {};buffer[0] = op(binop);
		ss << buffer;
		default_text_representation_impl(ss,binop.right());
        ss << ")";
    } else if (root_node->kind() == ceps::ast::Ast_node_kind::symbol) {
        ss << name(as_symbol_ref(root_node));
    } else if (root_node->kind() == ceps::ast::Ast_node_kind::structdef){
        auto & s = ceps::ast::as_struct_ref(root_node);
        ss <<name(s)<<  "{";
        for (auto p: s.children()){
            default_text_representation_impl(ss,p,enable_check_for_html);
            if (p && p->kind() !=  ceps::ast::Ast_node_kind::structdef) ss << ";";
        }
        ss << "};";
    } else if (ceps::ast::is<ceps::ast::Ast_node_kind::scope>(root_node)){
        auto & s = ceps::ast::as_scope_ref(root_node);
        ss << "{";
        for (auto p: s.children()){
            default_text_representation_impl(ss,p,enable_check_for_html);
            if (p && p->kind() !=  ceps::ast::Ast_node_kind::structdef) ss << ";";
        }
        ss << "};";
    } else if (ceps::ast::is<ceps::ast::Ast_node_kind::nodeset>(root_node)){
        auto & s = ceps::ast::as_ast_nodeset_ref(root_node);
        ss << "as_nodeset({";
        for (auto p: s.children()){
            default_text_representation_impl(ss,p,enable_check_for_html);
            if (p && p->kind() !=  ceps::ast::Ast_node_kind::structdef) ss << ";";
        }
        ss << "})";
    } else if (root_node->kind() == ceps::ast::Ast_node_kind::long_literal) {
		ss << value(as_int64_ref(root_node));
	} else ss << *root_node;

}

extern std::string default_text_representation(ceps::ast::Nodebase_ptr root_node){
	std::stringstream ss;
    default_text_representation_impl(ss,root_node);
	return ss.str();
}

extern std::string default_text_representation(std::vector<ceps::ast::Nodebase_ptr> nodes){
    std::stringstream ss;
    for(auto e : nodes) default_text_representation_impl(ss,e);
    return ss.str();
}



static void dump_nodes(std::ofstream& fout,ceps::ast::Nodeset nodes){
	for(auto n_:nodes){
		auto n = n_["node"];
		auto id = n["id"];
		if (id.size() == 0) continue;
		fout << "\"" << id.as_str() << "\";" << std::endl;
	}
}



static void dump_subdigraph(std::ofstream& fout,ceps::ast::Nodeset edges){
 for(auto e_:edges){
  auto e = e_["edge"];

   std::string from = default_text_representation(e["from"].nodes()[0]);
   std::string to = default_text_representation(e["to"].nodes()[0]);

   fout << "\"" << from << "\"" << " -> " << "\"" << to << "\"" <<";\n";

   }
}


static void write_copyright_and_timestamp(std::ostream& out, std::string title){
	time_t timer;time(&timer);tm * timeinfo;timeinfo = localtime(&timer);
	out
		<< "/* "<< title <<" " << std::endl
		<< "   CREATED " << asctime(timeinfo) << std::endl
        << "   GENERATED BY THE cepS2dot GENERATOR VERSION 0.10 (c) 2016,2017 Tomas Prerovsky <tomas.prerovsky@gmail.com>. \n"
		<< "   THIS IS A GENERATED FILE.\n\n"
		<< "   *** DO NOT MODIFY. ***\n*/\n"
		<< std::endl << std::endl;
}

struct Indent{
	int indentation = 0;

	void print_indentation(std::ostream& out)
	{
		for(int i = 0; i < indentation; ++i)
			out << " ";
	}

	int indent_incr(){
		return ++indentation;
	}

	int indent_decr(){
		return --indentation;
	}
};

static bool read_id(ceps::ast::Nonleafbase& parent, std::string& result){
 using namespace ceps::ast;
 for(auto c : parent.children()){
	 if (c->kind() != ceps::ast::Ast_node_kind::structdef) continue;
	 auto& child = as_struct_ref(c);
	 if (name(child) != "id") continue;
	 if (child.children().size() != 1 || (child.children()[0]->kind() != Ast_node_kind::identifier &&
			 child.children()[0]->kind() != Ast_node_kind::string_literal) )
		 return false;
	 if (child.children()[0]->kind() == Ast_node_kind::identifier) result = name(as_id_ref(child.children()[0]));
	 else result = "\""+value(as_string_ref(child.children()[0]))+"\"";
	 return true;
 }
 return false;
}

static void dump_digraph_elem(ceps::ast::Nodebase_ptr elem,std::ostream& fout, bool enable_html = false){
 using namespace ceps::ast;
 if (elem->kind() == Ast_node_kind::identifier){
  fout << name(as_id_ref(elem));
 }
 else if (elem->kind() == Ast_node_kind::int_literal){
  fout << value(as_int_ref(elem));
 }
 else if (elem->kind() == Ast_node_kind::float_literal){
  fout << value(as_double_ref(elem));
 } else if (elem->kind() == Ast_node_kind::string_literal){
	 bool is_html = false;
	 std::string v = value(as_string_ref(elem));
	 if (enable_html)
	 {
		 if (v.length() > 4 && v[0] == '<' && v[1] == '<'  && v[v.length()-2] == '>' && v[v.length()-1] == '>') is_html = true;
	 }
	 if (is_html )
		   fout << v;
	 else  fout << "\"" << v << "\"";
 } else throw ceps::interpreter::semantic_exception{nullptr,"dump_digraph: Unsupported attribute type."};
}

static void dump_digraph_attr(ceps::ast::Nodebase_ptr elem,std::ostream& fout,Indent& indent){
 using namespace ceps::ast;
 if (elem->kind() == Ast_node_kind::binary_operator){
  auto& e = ceps::ast::as_binop_ref(elem);
  dump_digraph_elem(e.left(),fout);
  bool is_label = e.left()->kind() == Ast_node_kind::identifier &&
		  (name(as_id_ref(e.left())) == "label" || name(as_id_ref(e.left())) == "taillabel" || name(as_id_ref(e.left())) == "headlabel");
  fout << "=";
  dump_digraph_elem(e.right(),fout,is_label);
 } else throw ceps::interpreter::semantic_exception{nullptr,"dump_digraph: illformed attribute assignment."};
}

static void dump_digraph_attrlist(std::vector<ceps::ast::Nodebase_ptr> elems,std::ostream& fout,Indent& indent){
 for(size_t i = 0; i!=elems.size();++i){
	 dump_digraph_attr(elems[i],fout,indent);
	 if (i + 1 != elems.size()) fout << ", ";
 }
}
static bool dump_digraph_stmt(ceps::ast::Nodebase_ptr root,std::ostream& fout,Indent& indent);

static void dump_digraph_subgraph( ceps::ast::Struct& digraph, std::ostream& fout,Indent& indent){
	std::string id;
	read_id(digraph,id);
	fout << "subgraph "<<id<<" {\n";
    indent.indent_incr();
	for(auto elem: digraph.children())
		{
		  std::stringstream str;
		  if(dump_digraph_stmt(elem,str,indent)){
			  indent.print_indentation(fout);
			  fout << str.str();
			  fout << ";\n";
		  }
		}
    indent.indent_decr();
    indent.print_indentation(fout);fout << "}";
}

static void dump_port(ceps::ast::Nodebase_ptr port_,std::ostream& fout,Indent& indent){
	using namespace ceps::ast;
	if(port_ == nullptr) return;
    auto& port = as_struct_ref(port_);
    for(auto e : port.children()){
    	if (e->kind() == Ast_node_kind::structdef && name(as_struct_ref(e))=="location"){
    	    auto& location = as_struct_ref(e);
            if (location.children().size() == 1){fout << ":";dump_digraph_elem(location.children()[0],fout);}
            else if (location.children().size() > 1){
            	fout << ":(";
            	dump_digraph_elem(location.children()[0],fout);
            	fout << ",";
            	dump_digraph_elem(location.children()[1],fout);
            	fout << ")";
            }
    	}
    	else if (e->kind() == Ast_node_kind::structdef && name(as_struct_ref(e))=="angle"){
    	    auto& angle = as_struct_ref(e);
            if (angle.children().size() >= 1){fout << "@";dump_digraph_elem(angle.children()[0],fout);}

    	}
    }
}

static void dump_endpoint(ceps::ast::Nodebase_ptr endpoint,std::ostream& fout,Indent& indent){
	using namespace ceps::ast;
	if (endpoint == nullptr) return;
	if (endpoint->kind() == Ast_node_kind::identifier) dump_digraph_elem(endpoint,fout);
	else if (endpoint->kind() == Ast_node_kind::string_literal) dump_digraph_elem(endpoint,fout);
	else if ( (endpoint->kind() == Ast_node_kind::structdef && (name(as_struct_ref(endpoint)) == "subgraph" ))){
		std::string id;
		read_id(as_struct_ref(endpoint),id);
		if (as_struct_ref(endpoint).children().size() == 1){
		  fout << "subgraph " << id;
		} else dump_digraph_subgraph(as_struct_ref(endpoint),fout,indent);
	}


}

static void dump_digraph_edge( ceps::ast::Struct& edge, std::ostream& fout,Indent& indent){
	using namespace ceps::ast;
	bool directed = true;
	Nodebase_ptr from = nullptr;
	Nodebase_ptr to = nullptr;
	Nodebase_ptr port_from = nullptr;
	Nodebase_ptr port_to = nullptr;
    for(auto child:edge.children()){
    	if ( (child->kind() == Ast_node_kind::structdef && (name(as_struct_ref(child)) == "subgraph" )))
    		{
				if (from == nullptr) 
					from = child;
				else 
					to=child;
			}
    	if (child->kind() == Ast_node_kind::identifier ){
    		if (name(as_id_ref(child)) == "undirected") directed = false;
    		else {
				if (from == nullptr) 
					from = child;
				else 
					to=child;
			}
    	}
    	if (child->kind() == Ast_node_kind::string_literal ){
    		if (from == nullptr) 
				from = child;
			else 
				to=child;
		}
    	if ( (child->kind() == Ast_node_kind::structdef && (name(as_struct_ref(child)) == "port" ))){
    		if (from == nullptr) 
				port_from = child;
			else 
				port_to=child;
		}
    }

    dump_endpoint(from,fout,indent);
    dump_port(port_from,fout,indent);

    if (directed) fout << " -> "; else fout << " -- ";
    dump_endpoint(to,fout,indent);
    dump_port(port_to,fout,indent);

    bool opening_bracket = false;
    for(auto child:edge.children()){

    	if ( (child->kind() == Ast_node_kind::structdef && (name(as_struct_ref(child)) == "a" || name(as_struct_ref(child)) == "attributes" ))){
    		if (opening_bracket) fout << ", ";
    		if (!opening_bracket){opening_bracket=true; fout << " [";}
    		dump_digraph_attrlist( as_struct_ref(child).children(),fout,indent);
    	}
    	else if ( child->kind() == Ast_node_kind::scope ){
    		if (opening_bracket) fout << ", ";
    		if (!opening_bracket){opening_bracket=true; fout << " [";}
    		dump_digraph_attrlist( ceps::ast::nlf_ptr(child)->children(),fout,indent);
    	}
    }
    if (opening_bracket)fout << "]";
}


static bool dump_digraph_stmt(ceps::ast::Nodebase_ptr root,std::ostream& fout,Indent& indent){
	using namespace ceps::ast;
	if ( (root->kind() == Ast_node_kind::structdef && (name(as_struct_ref(root)) == "a" || name(as_struct_ref(root)) == "attributes" ))){
		dump_digraph_attrlist( as_struct_ref(root).children(),fout,indent);
		return true;
	}
	else if ( root->kind() == Ast_node_kind::scope ){
		dump_digraph_attrlist( ceps::ast::nlf_ptr(root)->children(),fout,indent);
		return true;
	}
	else if ( root->kind() == Ast_node_kind::binary_operator ){
		dump_digraph_attr( root,fout,indent);
		return true;
	}
	else if ( (root->kind() == Ast_node_kind::structdef && (name(as_struct_ref(root)) == "node" ))){
	   fout << "node " << "[";
	   dump_digraph_attrlist( as_struct_ref(root).children(),fout,indent);
	   fout << "]";
	   return true;
	}
	else if ( (root->kind() == Ast_node_kind::structdef && (name(as_struct_ref(root)) == "subgraph" ))){
	   dump_digraph_subgraph( as_struct_ref(root),fout,indent);
	   return true;
	}
	else if ( (root->kind() == Ast_node_kind::structdef && (name(as_struct_ref(root)) == "edge" ))){
	   dump_digraph_edge( as_struct_ref(root),fout,indent);
	   return true;
	}
	else if ( root->kind() == Ast_node_kind::structdef && name(as_struct_ref(root))!="id" ){
		   fout << name(as_struct_ref(root)) << " [";
		   dump_digraph_attrlist( as_struct_ref(root).children(),fout,indent);
		   fout << "]";
		   return true;
	}

	return false;
}

static void dump_digraph_impl(std::ofstream& fout,Indent& indent, ceps::ast::Struct& digraph,std::string type, std::string id){

	fout << "\n";
	indent.print_indentation(fout);
	if (type == "digraph") fout << "digraph ";
	else if (type == "strict_digraph") fout << "strict digraph ";
	else if (type == "graph") fout << "graph ";
	else if (type == "strict_graph") fout << "strict graph ";
	else throw ceps::interpreter::semantic_exception{nullptr,"dump_digraph: unknown type, should be one of digraph,graph,strict_digraph,strict_graph."};

	fout << id << " {\n";
	indent.indent_incr();
	for(auto elem: digraph.children())
		{
		  std::stringstream str;
		  if(dump_digraph_stmt(elem,str,indent)){
			  indent.print_indentation(fout);
			  fout << str.str();
			  fout << ";\n";
		  }

		}
	indent.indent_decr();
	indent.print_indentation(fout);fout << "}\n";
}


static void dump_digraph(std::ofstream& fout, ceps::ast::Struct& digraph){
 using namespace ceps::ast;
 Indent indent;
 std::string id;

 if (!read_id(digraph,id) ) throw ceps::interpreter::semantic_exception{nullptr,"dump_digraph: id not defined."};
 write_copyright_and_timestamp(fout,name(digraph)+" "+id);
 dump_digraph_impl(fout,indent,digraph,name(digraph),id);

 fout.flush();
}





static void dump_subgraph(std::ofstream& fout,ceps::ast::Nodeset edges,std::set<std::string> internal_states,std::string prefix){
 for(auto e_:edges){
  auto e = e_["edge"];
  auto ev = e["event"];
  std::string label;
  std::string events;
  std::string guards;
  std::string actions;


  if (ev.size())
  {

	 for(size_t i = 0; i != ev.size();++i)
	  events += name(as_symbol_ref(ev.nodes()[i]))+ (i + 1 < ev.size()?",":"") ;
   }

   label = events;
   if (guards.length())
		 label+= (events.length()?" / ":"") + guards;

   if (label.length()) label = " [ label = \"" + label + "\" ] ";

   std::string from = default_text_representation(e["from"].nodes()[0]);
   std::string to = default_text_representation(e["to"].nodes()[0]);
   if (internal_states.find(from) != internal_states.end()) from = prefix + "." + from;
   if (internal_states.find(to) != internal_states.end()) to = prefix + "." + to;

   fout << "\"" << from << "\"" << " -> " << "\"" << to << "\"" << label <<";\n";

   }
}

static void dump_smgraph(std::ofstream& fout, ceps::ast::Struct& digraph){
 using namespace ceps::ast;
 ceps::ast::Nodeset ns(&digraph);
 fout << "digraph " <<  ns["smgraph"]["id"].as_str() << "{\n";
 fout << R"(
 graph [layout = dot]
 node [fontname="Arial",fontsize="14",shape="box", style="rounded,filled", fillcolor=PowderBlue,margin=0];
 edge [fontname="Arial bold italic",fontsize="12",fontcolor="Indigo"];
 )";


 auto dg_edges = ns["smgraph"][all{"edge"}];
 dump_subgraph(fout,dg_edges,{},{});

 auto threads = ns["smgraph"][all{"thread"}];
 int ctr = 1;
 for(auto thrd_ : threads){
	 auto thrd = thrd_["thread"];
	 std::string prefix = "thread"+std::to_string(ctr);
	 fout <<"subgraph "<< "cluster_"<< ctr-1 <<" {\n";
	 fout <<"  label=\""<< prefix <<"\";\n" <<"style=filled;\ncolor=lightgrey;\n";
	 std::set<std::string> internal_states;
	 auto localstates = thrd["localstates"];
	 for(auto ls : localstates.nodes()) internal_states.insert(default_text_representation(ls));
	 dump_subgraph(fout,thrd[all{"edge"}],internal_states,prefix);
	 fout<<"}\n";
	 ++ctr;
 }


 fout << "}";
}

static void dump_html_impl(std::ofstream& fout,ceps::ast::Nodebase_ptr elem){
 if (elem->kind() == ceps::ast::Ast_node_kind::structdef){

  auto& cont = ceps::ast::as_struct_ref(elem);
  if (cont.children().size() == 0){ fout << "<" << ceps::ast::name(cont) << "/>";}

  else{

	  fout << "<" << ceps::ast::name(cont);
	  for(auto e: cont.children()){
		  if(e->kind() != ceps::ast::Ast_node_kind::structdef) continue;
		  if (ceps::ast::name(ceps::ast::as_struct_ref(e)) != "attr") continue;
		  fout << " " << ceps::ast::name( ceps::ast::as_struct_ref(ceps::ast::as_struct_ref(e).children()[0])) << "=\"";
		  auto& attr_cont = ceps::ast::as_struct_ref(ceps::ast::as_struct_ref(e).children()[0]);

		  for(auto ee: attr_cont.children())	fout << default_text_representation(ee);
		  fout << "\"";
	  }
	  fout << ">";
	  for(auto e: cont.children()){
		  if (e->kind() == ceps::ast::Ast_node_kind::structdef && ceps::ast::name(ceps::ast::as_struct_ref(e)) == "attr") continue;
		  if (e->kind() == ceps::ast::Ast_node_kind::structdef){
			  dump_html_impl(fout,e);
		  } else fout << default_text_representation(e);
	  }
	  fout << "</" << ceps::ast::name(cont)<< ">";
  }
 } else fout << default_text_representation(elem);

}
static void dump_html(std::ofstream& fout, ceps::ast::Struct& html){
 fout << "<html>";
 for (auto e: html.children()) dump_html_impl(fout,e);
 fout << "</html>"; fout << std::endl;
}

static bool find_node(ceps::ast::Nodebase_ptr root, ceps::ast::Nodebase_ptr p){
	if (p == root) return true;
	if (is_leaf(root->kind())) return false;
	
	auto r = ceps::ast::nlf_ptr(root);
	for(auto e: r->children())
		if (find_node(e,p)) return true;
	return false;
}

static std::string get_meta_info(ceps::ast::Nodeset * universe,ceps::ast::Nodebase_ptr p, std::string meta_inf){

	std::string v;
	meta_inf = "@@"+meta_inf;
	bool meta_info_found = false;
	for(auto e:universe->nodes())
	{
		if (e->kind() == ceps::ast::Ast_node_kind::structdef){
			auto& s = ceps::ast::as_struct_ref(e);
			auto nm = ceps::ast::name(s);
			if (nm == meta_inf){
				meta_info_found=true;
				if (s.children().size()!=0)
				v=default_text_representation(s.children()[0]);
			}
		}
		if (find_node(e,p)) { if (!meta_info_found)
			throw ceps::interpreter::semantic_exception{p,"Meta info '"+meta_inf+"' not available."}; return v; }
	}
	return v;
}


static std::random_device randomd;
static std::default_random_engine e1(randomd());
static bool contains_symbols(ceps::ast::Nodebase_ptr node){
	return ceps::ast::is<ceps::ast::Ast_node_kind::symbol>(node);
}

void peel_off_nodesets(ceps::ast::Nodebase_ptr p,std::vector<ceps::ast::Nodebase_ptr> & v){
    if(p == nullptr) return;
    if (p->kind() == ceps::ast::Ast_node_kind::nodeset){
        for(auto pp: ceps::ast::as_ast_nodeset_ref(p).children()){
            peel_off_nodesets(pp,v);
        }
    } else v.push_back(p);
}


bool is_macro(std::string const & s, ceps::parser_env::Symboltable & sym_table){
	auto sym  = sym_table.lookup(s);
	return sym != nullptr && sym->category ==  ceps::parser_env::Symbol::Category::MACRO;
}

namespace ceps{
	namespace interpreter{
		using namespace ceps::ast;
		using node_t = Nodebase_ptr;
        using node_int64_t = Int64*;
		using node_vec_t = std::vector<node_t>;

		node_unaryop_t mk_unary_op_node(char op, node_t operand){
			return new ceps::ast::Unary_operator(op,operand);
		}

		node_int_t mk_int_node(int value){
			return new ceps::ast::Int(value, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}

		node_int64_t mk_int64_node(std::int64_t value){
			return new ceps::ast::Int64(value, ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		}

		node_int64_t mk_int64_node(std::int64_t value, Unit_rep u){
			return new ceps::ast::Int64(value, u, nullptr, nullptr, nullptr);
		}

		node_double_t mk_double_node(double value, Unit_rep u){
			return new ceps::ast::Double(value, u, nullptr, nullptr, nullptr);
		}

		node_binary_operator_t mk_bin_op(int op,node_t lhs, node_t rhs, std::string full_name){
			return new ceps::ast::Binary_operator(op, full_name, lhs, rhs, nullptr);
		}

		node_unary_operator_t mk_unary_op(int op,node_t arg, std::string full_name){
			return new ceps::ast::Unary_operator(op, arg,nullptr, nullptr);
		}

		node_symbol_t mk_symbol(std::string name, std::string kind){
			return new ceps::ast::Symbol(name,kind,nullptr,nullptr,nullptr);
		}


		int read_int(node_t n){
			if (n->kind() != ceps::ast::Ast_node_kind::int_literal) return 0;
			return value(as_int_ref(n));
		}


		node_vec_t get_args(Call_parameters& params){
        	node_vec_t v;
            if (params.children().size()) //flatten_args(params.children()[0], v);
			 for(auto e : params.children()){
				 node_vec_t args_temp;
				 flatten_args(e, args_temp);
				 v.insert(v.end(), args_temp.begin(),args_temp.end());				 
			 }
			 return v;
		}

		node_t mod(node_t root_node, Symboltable & sym_table, Environment& env, node_t parent_node, node_t predecessor, Call_parameters* params)
		{
			using namespace ceps::ast;
        	node_vec_t args{get_args(*params)};
            if(args.size() != 2)
                 throw semantic_exception{root_node,"mod() requires two arguments."};

			if ( is<Ast_node_kind::int_literal>(args[0]) && is<Ast_node_kind::int_literal>(args[1]))
              return mk_int_node(value(as_int_ref(args[0])) % value(as_int_ref(args[1])));

            ceps::ast::Func_call* f = new ceps::ast::Func_call();
		 	f->children_.push_back(new ceps::ast::Identifier("mod", nullptr, nullptr, nullptr));
		 	f->children_.push_back(params);
		 	return f;
		}

		node_t as_nodeset(node_t root_node, Symboltable & sym_table, Environment& env, node_t parent_node, node_t predecessor, Call_parameters* params)
		{
        	node_vec_t args{get_args(*params)};
            if(args.size() == 0)
                 throw semantic_exception{root_node,"as_nodeset(): argument has to be a non empty list of nodes."};

			node_vec_t elements;
			for(auto e:args){
				if (is<Ast_node_kind::scope>(e))
				{
					auto& scp = as_scope_ref(e);
					for(auto ee: children(scp))
					 elements.push_back(ee);

				} else elements.push_back(e);
			}
            return ceps::ast::create_ast_nodeset("",elements);
		}

		node_t mktime(node_t root_node, Symboltable & sym_table, Environment& env,node_t parent_node, node_t predecessor, Call_parameters* params)
		{
        	node_vec_t args{get_args(*params)};
            if(args.size() == 0) return mk_int64_node(0);
			tm timeinfo = {};
			if (args.size() > 0) timeinfo.tm_year = read_int(args[0]) - 1900;
			if (args.size() > 1) timeinfo.tm_mon = read_int(args[1]) - 1;
			if (args.size() > 2) timeinfo.tm_mday = read_int(args[2]);
			if (args.size() > 3) timeinfo.tm_hour = read_int(args[3]);
			if (args.size() > 4) timeinfo.tm_min = read_int(args[4]);
			if (args.size() > 5) timeinfo.tm_sec = read_int(args[5]);

			auto r =::mktime(&timeinfo);
			return(mk_int64_node(r));
		}

		node_t push_back(node_t root_node, Symboltable & sym_table, Environment& env,node_t parent_node, node_t predecessor, Call_parameters* params)
		{
			using namespace ceps::ast;
			if (ceps::interpreter::DEBUG_OUTPUT) 
		      std::cerr << "ceps::interpreter::push_back() " << std::endl;

        	node_vec_t args{get_args(*params)};
			if (ceps::interpreter::DEBUG_OUTPUT)  
			 for(auto e: args)
			  std::cerr << " ceps::interpreter::push_back() argument:" <<  *e << std::endl;
			
			if (args.size() < 2)
			 throw semantic_exception{root_node,"push_back(): at least two  arguments expected."};
			if (contains_symbols(args[0])) {
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
		 		f->children_.push_back(new ceps::ast::Identifier("push_back", nullptr, nullptr, nullptr));
		 		f->children_.push_back(params);
		 		return f;
			}

			if (!is_leaf(args[0]->kind())){
			  auto& v = nlf_ptr(args[0])->children();
			  for(size_t i = 1; i < args.size(); ++i  ){
				  v.push_back(args[i]);
			  }
			}


            return args[0];
		}
	}
}

static std::unordered_map< std::string, 
                           ceps::ast::Nodebase_ptr  (*) (	ceps::ast::Nodebase_ptr root_node,
						   									ceps::parser_env::Symboltable & sym_table,
															ceps::interpreter::Environment& env,
		    												ceps::ast::Nodebase_ptr parent_node,
															ceps::ast::Nodebase_ptr predecessor,
															ceps::ast::Call_parameters* params) > func_cache;



template<typename P>
	bool contains_at_least_one_non_terminal_subexpression(ceps::ast::node_t n,P p){
		if (n == nullptr) return false;
		if (p(n)) return true;
        if (!is_leaf(n->kind())){
			for(auto e : nlf_ptr(n)->children() ){
				if (contains_at_least_one_non_terminal_subexpression(e,p)) return true;
			}
		}
		return false;
	}

ceps::ast::Nodebase_ptr ceps::interpreter::eval_funccall(
		ceps::ast::Nodebase_ptr root_node,
		ceps::parser_env::Symboltable & sym_table,
		ceps::interpreter::Environment& env,
		ceps::ast::Nodebase_ptr parent_node,
		ceps::ast::Nodebase_ptr predecessor,
		ceps::ast::Nodebase_ptr this_ptr,
		bool& symbols_found,
		ceps::interpreter::thoroughness_t thoroughness
		)
{
	if (ceps::interpreter::DEBUG_OUTPUT) std::cerr << "ceps::interpreter::eval_funccall:" << *root_node << std::endl;

	auto func_call = as_func_call_ref(root_node);
	auto fcall_target_ = func_call_target(func_call);
	bool func_symbolic{false};

	auto fcall_target = evaluate_generic(fcall_target_,sym_table,env,root_node,predecessor,nullptr,func_symbolic,thoroughness);

	if (!is_an_identifier(fcall_target)){
 		ceps::ast::Func_call* f = new ceps::ast::Func_call();
		f->children_.push_back(fcall_target);
		f->children_.push_back(evaluate_generic(func_call.children()[1],sym_table,env,root_node,predecessor,nullptr,func_symbolic,thoroughness));
	 	return f;
	} else {
		ceps::ast::Identifier& id = as_id_ref(fcall_target);
        ceps::ast::Nodebase_ptr params_ = nullptr;
		 
		
		if (env.is_lazy_func != nullptr && env.is_lazy_func(name(id))) params_ = func_call.children()[1];
		else params_ = evaluate_generic(func_call.children()[1],sym_table,env,root_node,predecessor,nullptr,func_symbolic,thoroughness);
		

		ceps::ast::Call_parameters& params = *static_cast<ceps::ast::Call_parameters*>(params_);
		if (ceps::interpreter::DEBUG_OUTPUT) std::cerr << "ceps::interpreter::eval_funccall: params:" << params << std::endl;

		if (this_ptr != nullptr){
			ceps::ast::Func_call* f = new ceps::ast::Func_call();
		 	f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
		 	f->children_.push_back(params_);
		 	return f;
		}

		auto rr = env.call_func_callback(ceps::ast::name(id),&params,sym_table);
		if (rr != nullptr) return rr;
		symbols_found = symbols_found || func_symbolic;

		if (func_symbolic){
			ceps::ast::Func_call* f = new ceps::ast::Func_call();
			f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
			f->children_.push_back(params_->clone());
			return f;
		}

		auto fit = func_cache.find(name(id));
		if (fit != func_cache.end()){
		  	return fit->second(root_node,sym_table,env,parent_node,predecessor,static_cast<ceps::ast::Call_parameters*> (params_));
		}

		if (is_macro(name(id), sym_table)) {
			bool do_eval_macro = !contains_at_least_one_non_terminal_subexpression(&params, [&](node_t n){
				if(n->kind() == ceps::ast::Ast_node_kind::func_call){
					auto func_call = as_func_call_ref(n);
					auto fcall_target = func_call_target(func_call);
					if (is_an_identifier(func_call_target(func_call))){
						ceps::ast::Identifier& id = as_id_ref(fcall_target);
						if (name(id) == "argv") return true;
					}
				}
				return false;
			}); 
			if (!do_eval_macro){
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
		 		f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
		 		f->children_.push_back(params_);
				return f;
				//return new ceps::ast::Func_call{fcall_target,&params};
			}
			std::vector<ceps::ast::Nodebase_ptr> args = extract_functioncall_arguments_from_param_block(params);

			ceps::interpreter::Environment::func_callback_t old_callback;
			void * old_func_callback_context_data;
			ceps::interpreter::Environment::func_binop_resolver_t old_binop_res;
			void * old_cxt;

			env.get_func_callback(old_callback,old_func_callback_context_data);
	        env.get_binop_resolver(old_binop_res,old_cxt);

			env.set_func_callback(nullptr,nullptr);
			env.set_binop_resolver(nullptr,nullptr);

			sym_table.push_scope();
			bool local_contains_symbols{false};

			auto result =  eval_macro(root_node,
					  sym_table.lookup(name(id)),
		 			  sym_table,
		 			  env,
		 			  parent_node,
		 			  predecessor,thoroughness,
					  local_contains_symbols,
					  &args);
			symbols_found = symbols_found || local_contains_symbols;

			sym_table.pop_scope();

			env.set_func_callback(old_callback,old_func_callback_context_data);
			env.set_binop_resolver(old_binop_res,old_cxt);
			
			return result;
		 }

		if (name(id) == "hd"){
			if(params.children().size() == 0)
				throw semantic_exception{root_node,"head(): argument has to be a non empty list of nodes."};

			return params.children()[0];
        } else if (name(id) == "strip"){
             if(params.children().size() != 1)
              throw semantic_exception{root_node,name(id)+": expecting one argument."};
             auto p = params.children()[0];
             if (p->kind() == ceps::ast::Ast_node_kind::nodeset){
              auto& an = ceps::ast::as_ast_nodeset_ref(p);
              if (an.children().size() != 1)
                  throw semantic_exception{root_node,name(id)+": size of nodeset unequal one."};
              return an.children()[0];
             } else return p;
        } else if (name(id) == "last"){
			if(params.children().size() == 0)
			 throw semantic_exception{root_node,"last(): argument has to be a non empty list of nodes."};
			std::vector<ceps::ast::Nodebase_ptr> a;
				a.push_back(params.children()[params.children().size()-1]);
			return ceps::ast::create_ast_nodeset("",a);
        } else if (name(id)=="as_nodeset"){
			 func_cache[name(id)] = ceps::interpreter::as_nodeset;
			 return ceps::interpreter::as_nodeset(root_node,sym_table,env,parent_node,predecessor,static_cast<ceps::ast::Call_parameters*>(params_));
        } else if (name(id)=="mktime"){
			 func_cache[name(id)] = ceps::interpreter::mktime; 
			 return ceps::interpreter::mktime(root_node,sym_table,env,parent_node,predecessor,static_cast<ceps::ast::Call_parameters*>(params_));
        } else if (name(id)=="push_back"){
			 func_cache[name(id)] = ceps::interpreter::push_back; 
			 return ceps::interpreter::push_back(root_node,sym_table,env,parent_node,predecessor,static_cast<ceps::ast::Call_parameters*>(params_));
        } else if (name(id)=="mod"){
			 func_cache[name(id)] = ceps::interpreter::mod; 
			 return ceps::interpreter::mod(root_node,sym_table,env,parent_node,predecessor,static_cast<ceps::ast::Call_parameters*>(params_));
        } else if (name(id) == "tail"){
			if(params.children().size() == 0)
				throw semantic_exception{root_node,"tail(): argument has to be a non empty list of nodes."};
			params.children().erase(params.children().begin());
			return ceps::ast::create_ast_nodeset("",params.children());
		 } else if (name(id) == "as_symbol") {
			 std::vector<ceps::ast::Nodebase_ptr> args;
			 if (params.children().size()) flatten_args(params.children()[0], args);
			 if (args.size() < 2)
				 throw semantic_exception{root_node,"as_symbol(): at least two arguments expected."};
			 if (args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
				 throw semantic_exception{root_node,"as_symbol(): wrong arguments (expect two strings)."};
			 if (args[1]->kind() != ceps::ast::Ast_node_kind::string_literal)
				 throw semantic_exception{root_node,"as_symbol(): wrong arguments (expect two strings)."};
			 if (args.size() >= 3 && args[2]->kind() == ceps::ast::Ast_node_kind::identifier && "into_symtab_at_global_scope" == ceps::ast::name(ceps::ast::as_id_ref(args[2])) ){
				 auto kind_name = ceps::ast::value(ceps::ast::as_string_ref(args[1]));
				 auto sym_name = ceps::ast::value(ceps::ast::as_string_ref(args[0]));
				 auto sym_kind_ptr = sym_table.lookup(kind_name,false);
				 if (sym_kind_ptr == nullptr || sym_kind_ptr->category != ceps::parser_env::Symbol::KIND)
					 throw semantic_exception{root_node,"as_symbol(...,into_symtab_at_global_scope): '"+kind_name+"' not a kind ."};
				 auto sym = sym_table.scopes[0]->insert(sym_name);
				 sym->category = ceps::parser_env::Symbol::SYMBOL;
				 ceps::parser_env::Symbol* copy_of_sym_kind_ptr = new ceps::parser_env::Symbol(ceps::parser_env::Symbol::KIND);
				 copy_of_sym_kind_ptr->name = sym_kind_ptr->name;
				 copy_of_sym_kind_ptr->payload = nullptr;
				 sym->payload = copy_of_sym_kind_ptr;
			 }
			 return new ceps::ast::Symbol(ceps::ast::value(ceps::ast::as_string_ref(args[0])),ceps::ast::value(ceps::ast::as_string_ref(args[1])), nullptr, nullptr, nullptr);
         } else if (name(id) == "make_struct") {
             std::vector<ceps::ast::Nodebase_ptr> args;
             if (params.children().size()) flatten_args(params.children()[0], args);
             if (args.size() == 0)
                 throw semantic_exception{root_node,"make_struct(): at least one argument expected."};
             if (args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
                 throw semantic_exception{root_node,"make_struct(): first argument expected to be a string."};

             std::string id = ceps::ast::value(ceps::ast::as_string_ref(args[0]));
             auto n = new ceps::ast::Struct(id);

             for(std::size_t i = 1; i != args.size();++i){
                 auto p = args[i];
                 if (p->kind() == ceps::ast::Ast_node_kind::nodeset){
                     peel_off_nodesets(p,n->children());
                 } else n->children().push_back(p);
             }
             ceps::parser_env::Symbol* sym_ptr = sym_table.lookup(id);
             if ( sym_ptr != nullptr && sym_ptr->category ==  ceps::parser_env::Symbol::Category::MACRO){
				bool local_contains_symbols{false};
                auto r =  eval_macro(n,
                           sym_ptr,
                           sym_table,
                           env,
                           nullptr,
                           nullptr,
						   thoroughness,
						   local_contains_symbols);
				symbols_found = symbols_found || local_contains_symbols;
				return r;				
             }

             return n;
         } else if (name(id) == "append") {
             std::vector<ceps::ast::Nodebase_ptr> args;
             if (params.children().size()) flatten_args(params.children()[0], args);
             if (args.size() == 0)
                 throw semantic_exception{root_node,"append(): at least one argument expected."};
             return args[0];
         } else if (name(id) == "as_identifier") {
			 std::vector<ceps::ast::Nodebase_ptr> args;
			 if (params.children().size()) flatten_args(params.children()[0], args);
			 if (args.size() != 1)
				 throw semantic_exception{root_node,"as_identifier(): one argument expected."};
			 if (args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
				 throw semantic_exception{root_node,"as_identifier(): wrong arguments (expect a string)."};
			 return new ceps::ast::Identifier(ceps::ast::value(ceps::ast::as_string_ref(args[0])), nullptr, nullptr, nullptr);
         }else if (name(id) == "as_int"){
             std::vector<ceps::ast::Nodebase_ptr> args;
             if (params.children().size()) flatten_args(params.children()[0], args);
             std::string s;
             for (auto p : args){
                    if(p == nullptr) continue;
                    s+=default_text_representation(p);
             }
             return new ceps::ast::Int(std::stoi(s),ceps::ast::all_zero_unit());
         } else if (name(id) == "text"){
			 std::vector<ceps::ast::Nodebase_ptr> args;
			 if (params.children().size()) flatten_args(params.children()[0], args);
			 std::string s;
			 for (auto p : args){
                    if(p == nullptr) continue;
					s+=default_text_representation(p);
			 }
			 return new ceps::ast::String(s,nullptr,nullptr,nullptr);
		 } else if (name(id) == "name") {
			 std::vector<ceps::ast::Nodebase_ptr> args;
			 if (params.children().size()) flatten_args(params.children()[0], args);
			 std::string s;
			 for (auto p : args){

			  	if(p == nullptr) continue;
                if (p->kind() == ceps::ast::Ast_node_kind::symbol)
                    s+=ceps::ast::name(ceps::ast::as_symbol_ref(p));
                else if (p->kind() == ceps::ast::Ast_node_kind::structdef)
			 	 s+=ceps::ast::name(ceps::ast::as_struct_ref(p));
                else if (p->kind() == ceps::ast::Ast_node_kind::func_call){
                    ceps::ast::Func_call& func_call = *dynamic_cast<ceps::ast::Func_call*>(p);
                    if (func_call.children()[0]->kind() == Kind::identifier)
                    {
                        ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(func_call.children()[0]);
                        s+=ceps::ast::name(id);
                    }
                }
			  	else if (p->kind() == ceps::ast::Ast_node_kind::nodeset){
				 auto& an = ceps::ast::as_ast_nodeset_ref(p);
			  	 for (auto pp:an.children()) {
			  	  if (pp->kind() == ceps::ast::Ast_node_kind::structdef)
			 	  s+=ceps::ast::name(ceps::ast::as_struct_ref(pp));
			  	 }
			  	}
			  }
			 return new ceps::ast::String(s,nullptr,nullptr,nullptr);
         } else if ( name(id) == "reg_as_symbol_with_global_scope" ){
             std::vector<ceps::ast::Nodebase_ptr> args;
             if (params.children().size()) flatten_args(params.children()[0], args);
             if (args.size() != 2 || args[0]->kind() != ceps::ast::Ast_node_kind::string_literal || args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
                 throw semantic_exception{root_node,"reg_as_symbol_with_global_scope(): expect two string arguments."};
             std::string kind = ceps::ast::value(ceps::ast::as_string_ref(args[1]));
             std::string sym = ceps::ast::value(ceps::ast::as_string_ref(args[0]));

             auto kind_symbol = sym_table.lookup(kind,false,false,false);
             if (kind_symbol == nullptr) throw semantic_exception{root_node,"reg_as_symbol_with_global_scope(): No kind "+kind+" defined globally."};
             auto new_symbol = sym_table.lookup_globally(sym,true,false);
             new_symbol->payload = kind_symbol;
             new_symbol->category = ceps::parser_env::Symbol::SYMBOL;
             return nullptr;
         } else if (name(id) == "predecessor") {
			 if(predecessor == nullptr) throw semantic_exception{root_node,"predecessor(): undefined in current context."};
			 return ceps::ast::create_ast_nodeset("",std::vector<ceps::ast::Nodebase_ptr>{predecessor});
		 }else if (name(id) == "meta") {
			 std::vector<ceps::ast::Nodebase_ptr> args;
			 flatten_args(params.children()[0], args);
			 if (args.size() != 2)
			 	throw semantic_exception{root_node,"meta(): Wrong number of arguments"};
			 if (args[0]->kind() != ceps::ast::Ast_node_kind::nodeset)
				 throw semantic_exception{root_node,"meta(): first argument has to be a node set."};
			 if (args[1]->kind() != ceps::ast::Ast_node_kind::string_literal)
				 throw semantic_exception{root_node,"meta(): second argument has to be a node string."};
			 auto& an = ceps::ast::as_ast_nodeset_ref(args[0]);
			 if (an.children().size() == 0)
				 return new ceps::ast::String("",nullptr,nullptr,nullptr);
			 return new ceps::ast::String(get_meta_info(env.associated_universe(),an.children()[0],ceps::ast::value(ceps::ast::as_string_ref(args[1]))),nullptr,nullptr,nullptr);

		 } else if (name(id) == "system") {
			 std::vector<ceps::ast::Nodebase_ptr> args;
			 flatten_args(params.children()[0], args);
			 if (args.size() != 1)
				 throw semantic_exception{root_node,"system: Wrong number of arguments"};
			 if (args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
				 throw semantic_exception{root_node,"system: Expect first parameter to be a string."};
			 std::string cmd;
			 cmd = value(ceps::ast::as_string_ref(args[0]));
			 auto r = system(cmd.c_str());
			 return new ceps::ast::Int(r,ceps::ast::all_zero_unit(),nullptr,nullptr,nullptr);
		 } else if (name(id) == "dump"){

			 std::vector<ceps::ast::Nodebase_ptr> args;
			 flatten_args(params.children()[0], args);

			 if (args.size() <= 1)
				 throw semantic_exception{root_node,"dump: Wrong number of arguments"};
			 if (args[0]->kind() != ceps::ast::Ast_node_kind::string_literal)
				 throw semantic_exception{root_node,"dump: Expect first parameter to be a string."};
			 std::string filename;
			 std::ofstream fout(filename =ceps::ast::value(ceps::ast::as_string_ref(args[0])));
			 if (!fout)throw semantic_exception{root_node,"dump: Could not open file '"+filename+"' for output."};
			 for(size_t i = 1; i < args.size();++i){
				 if (args[i]->kind() != ceps::ast::Ast_node_kind::nodeset) continue;
				 auto& nodeset = ceps::ast::as_ast_nodeset_ref(args[i]);
				 for(auto ns_el : nodeset.children()){
					 if (ns_el->kind() != ceps::ast::Ast_node_kind::structdef) continue;
					 auto& container = ceps::ast::as_struct_ref(ns_el);
					 std::string docid = name(container);
					 if (docid == "smgraph")
						 dump_smgraph(fout,container);
					 else if (docid == "digraph" || docid == "graph" || docid == "strict_digraph" || docid == "strict_graph")
						 dump_digraph(fout,container);
					 else if (docid == "html")
						 dump_html(fout,container);

				 }
			 }
			 return nullptr;
		 } else if (name(id) == "include_xml")	 {
			 node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};

			 if (args.size() != 1 &&  args.size() != 2)
				 throw semantic_exception{root_node,"include_xml: Wrong number of arguments"};

			 if (args.size() == 1){
			  auto arg = args[0];			  
			  if (arg->kind() != Kind::string_literal)
				 throw semantic_exception{root_node,"include_xml: Illformed argument"};
			  return include_xml_file(ceps::ast::value(ceps::ast::as_string_ref(arg)));
			 } else {
				  if (args[0]->kind() != Kind::string_literal || args[1]->kind() != Kind::string_literal)
					 throw semantic_exception{root_node,"include_xml: Illformed argument"};

				  return include_xml_file(ceps::ast::value(ceps::ast::as_string_ref(args[0])),ceps::ast::value(ceps::ast::as_string_ref(args[1])));
			 }

         } else if (name(id) == "env") {
             std::vector<ceps::ast::Nodebase_ptr> args;
             flatten_args(params.children()[0], args);
             if (args.size() == 0){
                 auto p = new ceps::ast::Struct{"env"};
                 for(auto e = environ;*e;++e){
                     p->children().push_back(new ceps::ast::String{*e});
                 }
                 return p;
             } else {
                 if (args[0]->kind() != ceps::ast::Ast_node_kind::string_literal) return new ceps::ast::String{""};
                 auto s = ceps::ast::value(ceps::ast::as_string_ref(args[0]));
                 auto p = getenv(s.c_str());
                 if (p) return new ceps::ast::String{p};
             }
             if(args.size() > 1 && args[1]->kind() == ceps::ast::Ast_node_kind::string_literal)
                 return args[1];
             return new ceps::ast::String{""};
         } else if (name(id) == "__uniform_dist") {
             std::vector<ceps::ast::Nodebase_ptr> args;
             flatten_args(params.children()[0], args);

             if (args.size() != 2)
                 throw semantic_exception{root_node,"__random: Wrong number of arguments"};

             if (args[0]->kind() != ceps::ast::Ast_node_kind::int_literal || args[1]->kind() != ceps::ast::Ast_node_kind::int_literal){
                 func_call.children()[1] = params_;
                 return root_node;
             }

             std::uniform_int_distribution<int> uniform_dist(ceps::ast::value(ceps::ast::as_int_ref(args[0])),ceps::ast::value(ceps::ast::as_int_ref(args[1])));
             return new ceps::ast::Int(uniform_dist(e1), ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);

         } else if (name(id) == "sin") {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				 throw semantic_exception{root_node,"sin: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::sin(value(as_double_ref(arg))),ceps::ast::all_zero_unit(),nullptr,nullptr,nullptr);
			else if (arg->kind() == Kind::int_literal)
				return new ceps::ast::Double(std::sin((double)value(as_int_ref(arg))), ceps::ast::all_zero_unit(), nullptr, nullptr, nullptr);
		 	else
			 {
				 if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
				 auto fp = new ceps::ast::Call_parameters();
				 fp->children().push_back(arg);
				 ceps::ast::Func_call* f = new ceps::ast::Func_call();
				 f->children_.push_back(new ceps::ast::Identifier(name(id),nullptr,nullptr,nullptr));
				 f->children_.push_back(fp);
				 return f;
			 }
		 } else 	if (name(id) == "abs")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"abs: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::abs(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
			else if (arg->kind() == Kind::int_literal)
				return new ceps::ast::Double(std::abs((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
		 	else
		 	{
				 if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
				 auto fp = new ceps::ast::Call_parameters();
				 fp->children().push_back(arg);
				 ceps::ast::Func_call* f = new ceps::ast::Func_call();
				 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				 f->children_.push_back(fp);
				 return f;
			}
		 } else	 if (name(id) == "cos")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"cos: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::cos(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
		 	else if (arg->kind() == Kind::int_literal)
			 	return new ceps::ast::Double(std::cos((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
			else
			{
				if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
				auto fp = new ceps::ast::Call_parameters();
				fp->children().push_back(arg);
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
				f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				f->children_.push_back(fp);
				return f;
			}
		 }
		 if (name(id) == "tan")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"tan: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::tan(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
			else if (arg->kind() == Kind::int_literal)
				return new ceps::ast::Double(std::tan((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
			else
			{
				if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
				auto fp = new ceps::ast::Call_parameters();
				fp->children().push_back(arg);
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
				f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				f->children_.push_back(fp);
				return f;
			}
		 }
		 if (name(id) == "atan")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"atan: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::atan(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
			 else if (arg->kind() == Kind::int_literal)
				return new ceps::ast::Double(std::atan((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
			 else
			 {
				if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
			 	auto fp = new ceps::ast::Call_parameters();
				fp->children().push_back(arg);
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
				f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				f->children_.push_back(fp);
				return f;
			 }
		 }

		 if (name(id) == "acos")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"atan: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				 return new ceps::ast::Double(std::acos(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
			else if (arg->kind() == Kind::int_literal)
				 return new ceps::ast::Double(std::acos((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
			else
			{
				if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
			 	auto fp = new ceps::ast::Call_parameters();
				fp->children().push_back(arg);
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
				f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				f->children_.push_back(fp);
				return f;
			}
		 }

		 if (name(id) == "asin")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"asin: Expecting 1 argument"};
			auto arg = args[0];

			 if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::asin(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
			 else if (arg->kind() == Kind::int_literal)
				return new ceps::ast::Double(std::asin((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
			 else
			 {
				if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
				auto fp = new ceps::ast::Call_parameters();
				fp->children().push_back(arg);
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
				f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				f->children_.push_back(fp);
				return f;
			 }
		 }

		 if (name(id) == "ln" || name(id) == "log")
		 {
			node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
			if (args.size() != 1)
				throw semantic_exception{root_node,"ln/log: Expecting 1 argument"};
			auto arg = args[0];

			if (arg->kind() == Kind::float_literal)
				return new ceps::ast::Double(std::log(value(as_double_ref(arg))),unit(as_double_ref(arg)),nullptr,nullptr,nullptr);
			else if (arg->kind() == Kind::int_literal)
				 return new ceps::ast::Double(std::log((double)value(as_int_ref(arg))), unit(as_int_ref(arg)), nullptr, nullptr, nullptr);
			else
		 	{
				if (ceps::ast::is<Ast_node_kind::symbol>(arg)) symbols_found = true;
				auto fp = new ceps::ast::Call_parameters();
				fp->children().push_back(arg);
				ceps::ast::Func_call* f = new ceps::ast::Func_call();
				f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
				f->children_.push_back(fp);
				return f;
			}
		 }
		 if (name(id) == "value" )
		 {
			 std::vector<ceps::ast::Nodebase_ptr> result;
			 for ( auto & param : params.children() )
			 {
				 if (!ceps::ast::is_a_struct(param)) continue;
				 for(auto & child: as_struct_ref(param).children())
					 result.push_back(child);
			 }
			 return create_ast_nodeset( "", result);
		 }

		 ceps::ast::Func_call* f = new ceps::ast::Func_call();
		 f->children_.push_back(new ceps::ast::Identifier(name(id), nullptr, nullptr, nullptr));
		 f->children_.push_back(params_->clone());
		 node_vec_t args{get_args(*static_cast<ceps::ast::Call_parameters*>(params_))};
		 for(auto e : args) if (is<Ast_node_kind::symbol>(e)){ symbols_found = true;break;}
		 return f;
	 }
	 return nullptr;
}

