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

  
%verbose
%skeleton "lalr1.cc"
%require "2.7"
%defines
%define parser_class_name "Cepsparser"
%define namespace ceps



%code requires {
 class Ceps_parser_driver;
 namespace ceps{namespace parsetree{class Node;}namespace ast{class Nodebase;}}
}

%parse-param {Ceps_parser_driver& driver}
%lex-param   {Ceps_parser_driver& driver}
%locations


//Includes
%code {
#include "cepsparserdriver.hh"
#include "cepslexer.hh"
#include "ceps_ast.hh"
#include <valarray>
}

%union
{
	int ival;
	double fval;
	std::string* sval;
	ceps::parsetree::Node* node;//deprecated
	ceps::ast::Nodebase* ast_node;
	std::vector<std::string>* str_list;
};


%token END 0
%token <ival> INTNUM
%token <fval> FLOATNUM
%token STRUCT
%token VAL
%token FUN
%token RET
%token IF
%token ELSE
%token FOR
%token EOL
%token RAWMAP
%token KIND
%token DOTDOT
%token TEMPLATE
%token TEMPLATE_ID
%token TEMPLATE_PARAM


//%token KINDID


%left ','

%left DOTDOT
%right '='
%left '|'
%left '&'
%left REL_OP_GT
%left REL_OP_LT
%left REL_OP_EQ
%left REL_OP_NEQ
%left REL_OP_LT_EQ
%left REL_OP_GT_EQ
%left '-' '+'
%left '*'
%right '/'
%right '^'
%left '.'

%left NEG
%left NOT
%token <sval> STRUCTID
%token <sval> ID
%token <sval> KINDID
%token <sval> LITERAL
%token <sval> METRIC_UNIT // meter or m, second or s or sec,kilogram or kg
%right FUNCCALL
%left INDEXOP

%type <ast_node> cepsscript;
%type <ast_node> expr
%type <ast_node> struct_initialization
%type <ast_node> stmts;
%type <ast_node> stmt;
%type <ast_node> decl;
%type <ast_node> struct_decl;
%type <ast_node> raw_line;
%type <ast_node> raw_lines;
%type <ast_node> raw_map;
%type <ast_node> argument_list;
%type <ast_node> argument;
%type <str_list> id_list;
%type <sval> general_id;
%type <ast_node> for_loop;
%type <ast_node> template;
%type <ast_node> id_or_struct_id;
%type <ast_node> for_loop_head;
%type <ast_node> ifthenelse;

%expect 41

%%
 
cepsscript: 
 stmts 
 {
  ceps::ast::Root_ptr root_node = new ceps::ast::Root($1);
  driver.set_parsetree(ceps::ast::Parsetree{root_node});
  $$ = root_node;
 }
;
 
stmts :
 stmts stmt 
 {
  if($2 != nullptr) 
  	ceps::ast::nlf_ptr($1)->children().push_back($2);
  $$ = $1;
 }

| /*empty*/ 
{
	$$ = new ceps::ast::Stmts{}; //create empty Statements-Node
}
;

stmt :

 '{' stmts '}' { $$ = new ceps::ast::Scope{$2};}
| RET expr ';'
{
 $$ = new ceps::ast::Return($2,nullptr,nullptr);
}
| ifthenelse
{
  $$ = $1;
}
|
 
 decl ';'
 {
 	$$ = $1; //pass over result of decl
 }
| 
 for_loop
 {
  $$ = $1; //pass over result of for_loop
 }
 
| 
 template ';'
 {
  $$ = $1;
 }
| ';' 
{
 	$$ = nullptr;
 }
;

ifthenelse:
 IF '(' expr ')' stmt
{
 $$ = new ceps::ast::Ifelse($3,$5);
}
| IF '('expr')' stmt ELSE stmt
{
$$ = new ceps::ast::Ifelse($3,$5,$7);
}
;


general_id :
 ID
 |STRUCTID
 |KINDID
;

id_list:
 general_id
 {
  $$ = new std::vector<std::string> {*$1};
 }
 |  id_list ',' general_id
 {
   $1->push_back(*$3);
   $$ = $1;
 }
;

decl:
 struct_decl 
 {
 	$$ = $1; //pass over result 
 }
| RAWMAP eols '{' raw_map '}' 
{
	$$ = $4; //pass over result
}
| VAL ID '=' expr 
{
	$$ = new ceps::ast::Valdef(*$2,$4);
	delete $2;
}
| VAL STRUCTID '=' expr 
{
	$$ = new ceps::ast::Valdef(*$2,$4);
	delete $2;
}

|STRUCT ID 
{
	driver.symboltable().lookup(*$2,true,true,false);
	delete $2; 
	$$=nullptr;
}
|STRUCT STRUCTID 
{
	delete $2;
	$$=nullptr;
}
| KIND ID
{
 auto symbol = driver.symboltable().lookup(*$2,true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete $2;
 $$ = nullptr; 	
}
| KIND STRUCTID
{
 auto symbol = driver.symboltable().lookup(*$2,true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete $2;
 $$ = nullptr; 	
}
| KIND KINDID
{
 auto symbol = driver.symboltable().lookup(*$2,true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete $2;
 $$ = nullptr; 	
}
|KINDID id_list
{
 auto r = new ceps::ast::Kinddef(*$1,nullptr,nullptr,nullptr);
 for (auto s : *$2)
 {
  r->children().push_back(new ceps::ast::Identifier(s,nullptr,nullptr,nullptr)); 
 }
 delete $2; 
 $$ = r;
} 
|expr 
{
	$$ = new ceps::ast::Expression($1,nullptr,nullptr);
}
;

struct_decl:
STRUCTID {driver.symboltable().push_scope();} struct_initialization {driver.symboltable().pop_scope();}
{
 $$ = new ceps::ast::Struct(*$1,$3,nullptr,nullptr);
}
|STRUCTID ID struct_initialization  
{
 $$ = new ceps::ast::Struct(*$1,new ceps::ast::Identifier(*$2,nullptr,nullptr,nullptr),$3,nullptr);
}
;

struct_initialization:
 '{' stmts '}' 
 {
 	$$ = $2;
 }
;


expr:
 LITERAL 
 {
 	$$ = new ceps::ast::String(*$1,nullptr,nullptr,nullptr);
 	delete $1;
 }

|ID 
 {
    $$ = new ceps::ast::Identifier(*$1,nullptr,nullptr,nullptr);
    delete $1;
  }
|STRUCTID 

{
	$$ = new ceps::ast::Identifier(*$1,nullptr,nullptr,nullptr); 
	delete $1;
}
|KINDID
{
 //TODO:Handle kindid
}

|INTNUM 

{
	$$ = new ceps::ast::Int($1,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}

|FLOATNUM 

{
	$$ = new ceps::ast::Double($1,ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}

|'-' expr %prec NEG

{  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	$$ = new ceps::ast::Unary_operator('-',$2,nullptr,nullptr); 
}
|'!' expr %prec NEG

{  
	
	$$ = new ceps::ast::Unary_operator('!',$2,nullptr,nullptr); 
}

|expr DOTDOT expr
{
 $$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::DOTDOT,$1,$3,nullptr); 
}

|expr ',' expr
{
	$$ = new ceps::ast::Binary_operator(',',$1,$3,nullptr); 
}

|expr '|' expr
{
	$$ = new ceps::ast::Binary_operator('|',$1,$3,nullptr); 
}
|expr '&' expr
{
	$$ = new ceps::ast::Binary_operator('&',$1,$3,nullptr); 
}

|expr '+' expr 

{
	$$ = new ceps::ast::Binary_operator('+',$1,$3,nullptr); 
}
|expr '-' expr

{
	$$ = new ceps::ast::Binary_operator('-',$1,$3,nullptr);
}

|expr '*' expr

{
	$$ = new ceps::ast::Binary_operator('*',$1,$3,nullptr);			
}
|expr '/' expr

{
	$$ = new ceps::ast::Binary_operator('/',$1,$3,nullptr); 
}
|expr '^' expr

{
	$$ = new ceps::ast::Binary_operator('^',$1,$3,nullptr); 
}
|expr '.' expr
{
	$$ = new ceps::ast::Binary_operator('.',$1,$3,nullptr); 
}

| expr '=' expr 
{
	$$ = new ceps::ast::Binary_operator('=',$1,$3,nullptr);
} 
| expr REL_OP_EQ expr 
{
	$$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_EQ,$1,$3,nullptr);
}
| expr REL_OP_LT expr 
{
	$$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT,$1,$3,nullptr);
}
| expr REL_OP_GT expr 
{
	$$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT,$1,$3,nullptr);
} 
| expr REL_OP_LT_EQ expr 
{
	$$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT_EQ,$1,$3,nullptr);
}
| expr REL_OP_GT_EQ expr 
{
	$$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT_EQ,$1,$3,nullptr);
}
| expr REL_OP_NEQ expr 
{
	$$ = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_NEQ,$1,$3,nullptr);
}
|'('expr')' 
{
	$$ = $2;
}
| '[' expr ']' 
{
	$$ = $2;
}

| expr '(' argument_list ')'  %prec FUNCCALL
{
 //std::cout << "FUNCTION!!" << std::endl;
 $$ = new ceps::ast::Func_call{$1,$3};
}
| expr '('  ')'  %prec FUNCCALL
{
 //std::cout << "FUNCTION!!" << std::endl;
 $$ = new ceps::ast::Func_call($1,new  ceps::ast::Call_parameters(), nullptr);
}

| FUN '('  parameter_list ')' '{' func_body '}'
{
 //TODO: Function Definitions
}
| expr '[' expr ']' %prec INDEXOP
/*
| TEMPLATE_ID '(' stmts ')'
| TEMPLATE_PARAM*/
;

func_body :
 /*epsilon*/
 | func_stmts
 ;

func_stmts:
 expr ';' func_body
 | VAL ID '=' expr ';' func_body
 | RET expr ';' func_body
 | if_then_else
 | for_loop
 | '{' func_body '}'
;

if_then_else:
 IF '(' expr ')' func_stmts
 | IF '('expr')' func_stmts ELSE func_stmts
;

id_or_struct_id:
 ID
 {
    $$ = new ceps::ast::Identifier(*$1,nullptr,nullptr,nullptr); 
	delete $1;
 }
| STRUCTID
{
   $$ = new ceps::ast::Identifier(*$1,nullptr,nullptr,nullptr); 
	delete $1;
}
;

for_loop_head :
 id_or_struct_id ':' expr
 {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back($1);
  head->children().push_back($3);
  $$ = head;
 }
| for_loop_head id_or_struct_id ':' expr
 {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr($1);
  head->children().push_back($2);
  head->children().push_back($4);
  $$ = head;
 }
;

  
for_loop:
 FOR '('  for_loop_head  ')' '{' stmts '}'
 {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back($3);
  temp->children().push_back($6);
  $$ = temp;
 }
;

template:
 TEMPLATE general_id '(' id_list ')' '{' stmts '}'
{
  auto temp = new ceps::ast::Template_defintion(*$2,*$4,nullptr,nullptr,nullptr);
  
}
|
 TEMPLATE general_id  '{' stmts '}'
{
}
|
TEMPLATE general_id '(' ')' '{' stmts '}'
{
}
;


parameter_list:
 /*empty*/
 | parameter 
 | parameter ',' parameter_list
;

parameter:
 ID
;

argument_list:
 argument
 {
  $$ = new ceps::ast::Call_parameters($1,nullptr,nullptr);
 }
   
 | argument_list ',' argument
 {
  if($3 != nullptr) 
  	ceps::ast::nlf_ptr($1)->children().push_back($3);
    
  $$ = $1;
 }
 ;
 
 argument : 
  expr 
 {
  $$ = $1;
 }
 
 ;


//Raw Maps

raw_map:
 raw_lines 
 {
 	$$=$1;
 }
;

raw_lines:
 /*empty*/ 
 {
 	$$ = new ceps::ast::Rawmap();
 }
 | raw_lines raw_line 
 { 
 	if (!ceps::ast::nlf_ptr($2)->empty()) 
 	 ceps::ast::nlf_ptr($1)->children().push_back($2); 
 	$$ = $1;
 }
;

raw_line:
EOL 
{
	$$ = new ceps::ast::Atoms();
}
| raw_line INTNUM  
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::Int($2,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  $$ = $1;
}
| raw_line '-' INTNUM  
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::Int(-1 * $3,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  $$ = $1;
}

| raw_line '+' INTNUM  
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::Int($3,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  $$ = $1;
}


| raw_line FLOATNUM 
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::Double($2,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  $$ = $1;
}
| raw_line '-' FLOATNUM 
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::Double(-1.0*$3,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  $$ = $1;
}

| raw_line '+' FLOATNUM  
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::Double($3,ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  $$ = $1;
}


| raw_line LITERAL
{
  ceps::ast::nlf_ptr($1)->children().push_back(new ceps::ast::String(*$2,nullptr,nullptr,nullptr));
  delete $2;
  $$ = $1;
}


;

eols :
/*empty*/
| EOL eols
;

%%

void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
