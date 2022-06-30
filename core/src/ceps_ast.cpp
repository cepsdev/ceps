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


#include "ceps_ast.hh"

using namespace ceps::ast;

const char * ceps::ast::ast_node_kind_to_text[] = {	"ROOT",
													"STRUCT",
													"ID",
													"STRING",
													"INT",
													"FLOAT",
													"EXPR",
													"SI_UNIT",
													"OPERATOR",
													"STMTS","STMT","VAL","LAMBDA","PARAMS",
													"PARAM","LAMBDA_BODY","RAWMAP","ATOMS",
													"VEC","UNARY_OPERATOR","SCOPE","FUNC_CALL",
													"CALL_PARAMETERS","INT64","UINT64",
													"KINDDEF","KIND","SYMBOL","FOR","FOR_HEAD",
													"NODESET","NODESET_PATH_EXPR","TEMPLATE",
													"TEMPLATE_ID","IF_ELSE","RETURN", "BYTE_ARRAY",
													"ERROR","UNDEFINED","NONE","MACRO","ALGORITHM",
													"LABEL","LET","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??"
													,"??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??"
													,"??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??"
													,"??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??","??"};


const int ceps::ast::Nodebase::iword_index = std::ios_base::xalloc();


inline const char * ceps::ast::c_str(ceps::ast::Ast_node_kind k)
{
	if ( (sizeof(ceps::ast::ast_node_kind_to_text) / sizeof(char *)) <= (unsigned int) k) return "UNDEFINED";
	return ast_node_kind_to_text[(int)k];
}


//////////////////////////////////

//////////////////////////////////  ceps::ast::Nodebase

std::ostream& ceps::ast::Nodebase::pretty_print(std::ostream& os)
{
	os.iword(iword_index) = true;
	return os;
}

 std::ostream& ceps::ast::Nodebase::pretty_print_off(std::ostream& os)
{
	os.iword(iword_index) = false;
	return os;
}

void ceps::ast::Nodebase::print_ws(std::ostream& out,int n) const
	{
		if (n < 0) return;
		for(int i = 0; i++ != n;) out << ' ';
	}




void ceps::ast::Nodebase::print(std::ostream& out,bool pretty_print,int indent) const
{
	if (kind() == ceps::ast::Ast_node_kind::string_literal) print_content(out,pretty_print,indent);
	else {
		out << "("<<ceps::ast::c_str(kind()) << ' '; print_content(out,pretty_print,indent); out << ")";
	}
}

std::ostream& ceps::ast::operator << (std::ostream & out, Nodebase const & n)
{
	n.print(out,out.iword(Nodebase::iword_index) != 0,0);
	return out;
}

ceps::ast::Nodebase_ptr ceps::ast::box(int j)
 {
	return  new Int(j, Unit_rep(), nullptr, nullptr, nullptr);
 }

ceps::ast::Nodebase_ptr ceps::ast::box(unsigned int j)
 {
	return  new Int((int)j, Unit_rep(), nullptr, nullptr, nullptr);
 }


ceps::ast::Nodebase_ptr ceps::ast::box(long j)
 {
	return  new Int64(j, Unit_rep(), nullptr, nullptr, nullptr);
 }

ceps::ast::Nodebase_ptr ceps::ast::box(unsigned long j)
 {
	return  new Int64((long)j, Unit_rep(), nullptr, nullptr, nullptr);
 }

ceps::ast::Nodebase_ptr ceps::ast::box(double d)
  {
	return  new Double(d, Unit_rep(), nullptr, nullptr, nullptr);
  }

ceps::ast::Nodebase_ptr ceps::ast::box(std::string const & s)
   {
	return  new String(s, nullptr, nullptr, nullptr);
   }

ceps::ast::Nodebase_ptr ceps::ast::box(ceps::ast::ident const & s){
	return new Identifier(s.v, nullptr, nullptr, nullptr);
}
Struct_ptr ceps::ast::make_struct(std::string const & name)
{
	return new Struct(name, nullptr, nullptr, nullptr);
}

std::ostream& ceps::ast::operator << (std::ostream& out, strct & s)
{
	out << *s.get_root();
	return out;
}


void ceps::ast::flatten_func_args(ceps::ast::Nodebase_ptr r, std::vector<ceps::ast::Nodebase_ptr>& v)
{
	if (r == nullptr) return;
	if (r->kind() == ceps::ast::Ast_node_kind::binary_operator && op(as_binop_ref(r)) ==  ',')
	{
	 auto& t = as_binop_ref(r);
	 flatten_func_args(t.left(),v);
	 flatten_func_args(t.right(),v);
	 return;
	}
	v.push_back(r);
}

ceps::ast::node_symbol_t ceps::ast::mk_symbol(std::string name, std::string kind){
	return new ceps::ast::Symbol(name,kind,nullptr, nullptr, nullptr);
}

ceps::ast::node_t ceps::ast::mk_string(std::string v){
	return new ceps::ast::String(v,nullptr,nullptr,nullptr);
}

ceps::ast::node_root_t ceps::ast::mk_root(){
	return new Root{};
}
ceps::ast::node_stmts_t ceps::ast::mk_stmts(){
	return new Stmts{};
}
ceps::ast::node_stmt_t ceps::ast::mk_stmt(){
	return new Stmt{};
}
ceps::ast::node_struct_t ceps::ast::mk_struct(std::string struct_name){
	return new Struct{struct_name};
}
ceps::ast::node_struct_t ceps::ast::mk_struct(std::string struct_name, std::vector<node_t> const & v){
	auto r = new Struct{struct_name};
	children(*r) = v;
	return r;
}


ceps::ast::node_scope_t ceps::ast::mk_scope(){
	return new Scope{};
}
ceps::ast::node_callparameters_t ceps::ast::mk_callparameters(){
	return new Call_parameters{};
}

ceps::ast::node_nodeset_t ceps::ast::mk_nodeset(){
	return new Ast_nodeset{""};
}

ceps::ast::node_t ceps::ast::mk_ifelse(ceps::ast::node_t c,ceps::ast::node_t l,ceps::ast::node_t r){
	return new ceps::ast::Ifelse(c,l,r);
}

ceps::ast::node_t ceps::ast::mk_none(){
	return new ceps::ast::None{};
}

std::vector<ceps::ast::node_t> ceps::ast::extract_functioncall_arguments_from_param_block(ceps::ast::Call_parameters& params){
	std::vector<ceps::ast::node_t> r;
	auto & args = params.children();
	if (args.size()){
		for(size_t i = 0; i != args.size(); ++i){
			std::vector<ceps::ast::node_t> t;
		   	flatten_func_args(args[i],t);
			if (t.size() == 0) continue;
			auto insert_pos = r.size();
			r.resize(insert_pos + t.size());
			for(size_t j = 0; j != t.size(); ++j)
			 r[insert_pos + j] = t[j];
		}		
	}
	return r;
}


ceps::ast::node_t ceps::ast::get_node_by_path(std::vector<std::string> v, std::vector<ceps::ast::node_t> const & ns){
	if (v.size() == 0) return nullptr;
	std::vector<node_t> const * nsp = &ns;
	Struct* r = nullptr;
	for(size_t i = 0; i < v.size(); ++i){
		r = nullptr;
		shallow_traverse(*nsp, [&](node_t n)->bool{
			if (is<Ast_node_kind::structdef>(n)){
			 auto&  strct{as_struct_ref(n)};
			 if (name(strct) != v[i]) return true;
			 r = as_struct_ptr(n);return false;
			}	
			return true;		
		});
		if (r == nullptr) return nullptr;
		nsp = &r->children();
	}
	return r;
} 


template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::string_literal>::clone(){
	return new This_type(*this);
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::int_literal>::clone(){
	return new This_type(*this);
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::float_literal>::clone(){
	return new This_type(*this);
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::long_literal>::clone(){
	return new This_type(*this);
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::unsigned_long_literal>::clone(){
	return new This_type(*this);
}




template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::root>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::structdef>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::identifier>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::expr>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::si_unit_or_derived>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::binary_operator>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::stmts>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::stmt>::clone(){	
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::lambda>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::formal_parameters>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::formal_parameter>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::lambda_body>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::atoms>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::vector>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::unary_operator>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::func_call>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::call_parameters>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::kind_def>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::kind>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::symbol>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::loop>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::for_loop_head>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::nodeset>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::nodeset_path_expr>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::template_definition>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::template_id>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::ifelse>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::ret>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::byte_array>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::error>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::undef>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::none>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::macro_definition>::clone(){
	auto r =  new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::algorithm_definition>::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::label>::clone(){
	auto r  = new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::let>::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}
template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::rawmap>::clone(){
	auto r  = new This_type(*this);
	r->owns_children() = true;
	return r;
}


template<>
ceps::ast::Nodebase* ceps::ast::Error::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Binary_operator::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Struct::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Scope::clone(){
	auto r = new Scope(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Identifier::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Valdef::clone(){
	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Kind::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Kinddef::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Ast_nodeset::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Symbol::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Template_id::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}


template<>
ceps::ast::Nodebase* ceps::ast::Let::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}


template<>
ceps::ast::Nodebase* ceps::ast::Macrodef::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	return r;
}


template<>
ceps::ast::Nodebase* ceps::ast::String::clone(){

	auto r = new This_type(*this);
	
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::Template_defintion::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	
	return r;
}


template<>
ceps::ast::Nodebase* ceps::ast::Label::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::User_defined::clone(){

	auto r = new This_type(*this);
	r->owns_children() = true;
	
	return r;
}

template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::user_defined>::clone(){

	auto r = new This_type(*this);
		
	return r;
}


template<>
 ceps::ast::Nodebase*  ast_node<Ast_node_kind::symbol,std::string>::clone() {std::cerr << "** Undefined." << std::endl; return nullptr;}
template<>
 ceps::ast::Nodebase*  ast_node<Ast_node_kind::binary_operator,std::string>::clone() {std::cerr << "** Undefined." << std::endl; return nullptr;}

 template<>
ceps::ast::Nodebase* ceps::ast::ast_node<ceps::ast::Ast_node_kind::valdef>::clone(){std::cerr << "** Undefined." << std::endl;return nullptr;}



void ceps::ast::gc(ceps::ast::node_t){}

ceps::ast::Nodebase_ptr ceps::ast::get_first_child(ceps::ast::Nodebase_ptr node){
	auto nlf = nlf_ptr(node);
	if (!nlf) return nullptr;
	return nlf->children()[0];
}




