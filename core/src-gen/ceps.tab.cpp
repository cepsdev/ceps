// A Bison parser, made by GNU Bison 3.0.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "ceps.tab.c" // lalr1.cc:398

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "ceps.tab.h"

// User implementation prologue.

#line 51 "ceps.tab.c" // lalr1.cc:406
// Unqualified %code blocks.
#line 46 "../src/grammar/ceps.y" // lalr1.cc:407

#include "cepsparserdriver.hh"
#include "cepslexer.hh"
#include "ceps_ast.hh"
#include <valarray>

#line 60 "ceps.tab.c" // lalr1.cc:407


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 31 "../src/grammar/ceps.y" // lalr1.cc:473
namespace ceps {
#line 146 "ceps.tab.c" // lalr1.cc:473

  /// Build a parser object.
  Cepsparser::Cepsparser (Ceps_parser_driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  Cepsparser::~Cepsparser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Cepsparser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Cepsparser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Cepsparser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Cepsparser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Cepsparser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  Cepsparser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  Cepsparser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  Cepsparser::by_type::by_type ()
     : type (empty)
  {}

  inline
  Cepsparser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Cepsparser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Cepsparser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  Cepsparser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Cepsparser::by_state::by_state ()
    : state (empty)
  {}

  inline
  Cepsparser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Cepsparser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  Cepsparser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Cepsparser::symbol_number_type
  Cepsparser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  Cepsparser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Cepsparser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  Cepsparser::stack_symbol_type&
  Cepsparser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Cepsparser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Cepsparser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Cepsparser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Cepsparser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Cepsparser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Cepsparser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Cepsparser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Cepsparser::debug_level_type
  Cepsparser::debug_level () const
  {
    return yydebug_;
  }

  void
  Cepsparser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Cepsparser::state_type
  Cepsparser::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
  Cepsparser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Cepsparser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Cepsparser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* If YYLEN is nonzero, implement the default value of the action:
       '$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYLHS.VALUE to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yylhs.value = yystack_[yylen - 1].value;
    else
      yylhs.value = yystack_[0].value;

    // Compute the default @$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 2:
#line 140 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Root_ptr root_node = new ceps::ast::Root((yystack_[0].value.ast_node));
  driver.set_parsetree(ceps::ast::Parsetree{root_node});
  (yylhs.value.ast_node) = root_node;
 }
#line 561 "ceps.tab.c" // lalr1.cc:846
    break;

  case 3:
#line 149 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 571 "ceps.tab.c" // lalr1.cc:846
    break;

  case 4:
#line 156 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Stmts{}; //create empty Statements-Node
}
#line 579 "ceps.tab.c" // lalr1.cc:846
    break;

  case 5:
#line 163 "../src/grammar/ceps.y" // lalr1.cc:846
    { (yylhs.value.ast_node) = new ceps::ast::Scope{(yystack_[1].value.ast_node)};}
#line 585 "ceps.tab.c" // lalr1.cc:846
    break;

  case 6:
#line 165 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Return((yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 593 "ceps.tab.c" // lalr1.cc:846
    break;

  case 7:
#line 169 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
}
#line 601 "ceps.tab.c" // lalr1.cc:846
    break;

  case 8:
#line 175 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result of decl
 }
#line 609 "ceps.tab.c" // lalr1.cc:846
    break;

  case 9:
#line 180 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of for_loop
 }
#line 617 "ceps.tab.c" // lalr1.cc:846
    break;

  case 10:
#line 186 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 625 "ceps.tab.c" // lalr1.cc:846
    break;

  case 11:
#line 190 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = nullptr;
 }
#line 633 "ceps.tab.c" // lalr1.cc:846
    break;

  case 12:
#line 197 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Ifelse((yystack_[2].value.ast_node),(yystack_[0].value.ast_node));
}
#line 641 "ceps.tab.c" // lalr1.cc:846
    break;

  case 13:
#line 201 "../src/grammar/ceps.y" // lalr1.cc:846
    {
(yylhs.value.ast_node) = new ceps::ast::Ifelse((yystack_[4].value.ast_node),(yystack_[2].value.ast_node),(yystack_[0].value.ast_node));
}
#line 649 "ceps.tab.c" // lalr1.cc:846
    break;

  case 17:
#line 215 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.str_list) = new std::vector<std::string> {*(yystack_[0].value.sval)};
 }
#line 657 "ceps.tab.c" // lalr1.cc:846
    break;

  case 18:
#line 219 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yystack_[2].value.str_list)->push_back(*(yystack_[0].value.sval));
   (yylhs.value.str_list) = (yystack_[2].value.str_list);
 }
#line 666 "ceps.tab.c" // lalr1.cc:846
    break;

  case 19:
#line 227 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result 
 }
#line 674 "ceps.tab.c" // lalr1.cc:846
    break;

  case 20:
#line 231 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result
}
#line 682 "ceps.tab.c" // lalr1.cc:846
    break;

  case 21:
#line 235 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 691 "ceps.tab.c" // lalr1.cc:846
    break;

  case 22:
#line 240 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 700 "ceps.tab.c" // lalr1.cc:846
    break;

  case 23:
#line 246 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
	delete (yystack_[0].value.sval); 
	(yylhs.value.ast_node)=nullptr;
}
#line 710 "ceps.tab.c" // lalr1.cc:846
    break;

  case 24:
#line 252 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	delete (yystack_[0].value.sval);
	(yylhs.value.ast_node)=nullptr;
}
#line 719 "ceps.tab.c" // lalr1.cc:846
    break;

  case 25:
#line 257 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 731 "ceps.tab.c" // lalr1.cc:846
    break;

  case 26:
#line 265 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 743 "ceps.tab.c" // lalr1.cc:846
    break;

  case 27:
#line 273 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto r = new ceps::ast::Kinddef(*(yystack_[1].value.sval),nullptr,nullptr,nullptr);
 for (auto s : *(yystack_[0].value.str_list))
 {
  r->children().push_back(new ceps::ast::Identifier(s,nullptr,nullptr,nullptr)); 
 }
 delete (yystack_[0].value.str_list); 
 (yylhs.value.ast_node) = r;
}
#line 757 "ceps.tab.c" // lalr1.cc:846
    break;

  case 28:
#line 283 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 765 "ceps.tab.c" // lalr1.cc:846
    break;

  case 29:
#line 289 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().push_scope();}
#line 771 "ceps.tab.c" // lalr1.cc:846
    break;

  case 30:
#line 289 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().pop_scope();}
#line 777 "ceps.tab.c" // lalr1.cc:846
    break;

  case 31:
#line 290 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[3].value.sval),(yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 785 "ceps.tab.c" // lalr1.cc:846
    break;

  case 32:
#line 294 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[2].value.sval),new ceps::ast::Identifier(*(yystack_[1].value.sval),nullptr,nullptr,nullptr),(yystack_[0].value.ast_node),nullptr);
}
#line 793 "ceps.tab.c" // lalr1.cc:846
    break;

  case 33:
#line 301 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 801 "ceps.tab.c" // lalr1.cc:846
    break;

  case 34:
#line 309 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
 	delete (yystack_[0].value.sval);
 }
#line 810 "ceps.tab.c" // lalr1.cc:846
    break;

  case 35:
#line 315 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
    delete (yystack_[0].value.sval);
  }
#line 819 "ceps.tab.c" // lalr1.cc:846
    break;

  case 36:
#line 321 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 828 "ceps.tab.c" // lalr1.cc:846
    break;

  case 37:
#line 326 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO:Handle kindid
}
#line 836 "ceps.tab.c" // lalr1.cc:846
    break;

  case 38:
#line 332 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}
#line 844 "ceps.tab.c" // lalr1.cc:846
    break;

  case 39:
#line 338 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}
#line 852 "ceps.tab.c" // lalr1.cc:846
    break;

  case 40:
#line 344 "../src/grammar/ceps.y" // lalr1.cc:846
    {  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('-',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 862 "ceps.tab.c" // lalr1.cc:846
    break;

  case 41:
#line 351 "../src/grammar/ceps.y" // lalr1.cc:846
    {  
	
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('!',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 871 "ceps.tab.c" // lalr1.cc:846
    break;

  case 42:
#line 357 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::DOTDOT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 879 "ceps.tab.c" // lalr1.cc:846
    break;

  case 43:
#line 362 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(',',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 887 "ceps.tab.c" // lalr1.cc:846
    break;

  case 44:
#line 367 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('|',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 895 "ceps.tab.c" // lalr1.cc:846
    break;

  case 45:
#line 371 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('&',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 903 "ceps.tab.c" // lalr1.cc:846
    break;

  case 46:
#line 377 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('+',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 911 "ceps.tab.c" // lalr1.cc:846
    break;

  case 47:
#line 382 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('-',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 919 "ceps.tab.c" // lalr1.cc:846
    break;

  case 48:
#line 388 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('*',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);			
}
#line 927 "ceps.tab.c" // lalr1.cc:846
    break;

  case 49:
#line 393 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('/',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 935 "ceps.tab.c" // lalr1.cc:846
    break;

  case 50:
#line 398 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('^',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 943 "ceps.tab.c" // lalr1.cc:846
    break;

  case 51:
#line 402 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('.',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 951 "ceps.tab.c" // lalr1.cc:846
    break;

  case 52:
#line 407 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('=',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 959 "ceps.tab.c" // lalr1.cc:846
    break;

  case 53:
#line 411 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 967 "ceps.tab.c" // lalr1.cc:846
    break;

  case 54:
#line 415 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 975 "ceps.tab.c" // lalr1.cc:846
    break;

  case 55:
#line 419 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 983 "ceps.tab.c" // lalr1.cc:846
    break;

  case 56:
#line 423 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 991 "ceps.tab.c" // lalr1.cc:846
    break;

  case 57:
#line 427 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 999 "ceps.tab.c" // lalr1.cc:846
    break;

  case 58:
#line 431 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_NEQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1007 "ceps.tab.c" // lalr1.cc:846
    break;

  case 59:
#line 435 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1015 "ceps.tab.c" // lalr1.cc:846
    break;

  case 60:
#line 439 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1023 "ceps.tab.c" // lalr1.cc:846
    break;

  case 61:
#line 444 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call{(yystack_[3].value.ast_node),(yystack_[1].value.ast_node)};
}
#line 1032 "ceps.tab.c" // lalr1.cc:846
    break;

  case 62:
#line 449 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call((yystack_[2].value.ast_node),new  ceps::ast::Call_parameters(), nullptr);
}
#line 1041 "ceps.tab.c" // lalr1.cc:846
    break;

  case 63:
#line 455 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO: Function Definitions
}
#line 1049 "ceps.tab.c" // lalr1.cc:846
    break;

  case 75:
#line 485 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
 }
#line 1058 "ceps.tab.c" // lalr1.cc:846
    break;

  case 76:
#line 490 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 1067 "ceps.tab.c" // lalr1.cc:846
    break;

  case 77:
#line 498 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1078 "ceps.tab.c" // lalr1.cc:846
    break;

  case 78:
#line 505 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr((yystack_[3].value.ast_node));
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1089 "ceps.tab.c" // lalr1.cc:846
    break;

  case 79:
#line 516 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back((yystack_[4].value.ast_node));
  temp->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = temp;
 }
#line 1101 "ceps.tab.c" // lalr1.cc:846
    break;

  case 80:
#line 527 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto temp = new ceps::ast::Template_defintion(*(yystack_[6].value.sval),*(yystack_[4].value.str_list),nullptr,nullptr,nullptr);
  
}
#line 1110 "ceps.tab.c" // lalr1.cc:846
    break;

  case 81:
#line 533 "../src/grammar/ceps.y" // lalr1.cc:846
    {
}
#line 1117 "ceps.tab.c" // lalr1.cc:846
    break;

  case 82:
#line 537 "../src/grammar/ceps.y" // lalr1.cc:846
    {
}
#line 1124 "ceps.tab.c" // lalr1.cc:846
    break;

  case 87:
#line 554 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = new ceps::ast::Call_parameters((yystack_[0].value.ast_node),nullptr,nullptr);
 }
#line 1132 "ceps.tab.c" // lalr1.cc:846
    break;

  case 88:
#line 559 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
    
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 1143 "ceps.tab.c" // lalr1.cc:846
    break;

  case 89:
#line 569 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
 }
#line 1151 "ceps.tab.c" // lalr1.cc:846
    break;

  case 90:
#line 580 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node)=(yystack_[0].value.ast_node);
 }
#line 1159 "ceps.tab.c" // lalr1.cc:846
    break;

  case 91:
#line 587 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::Rawmap();
 }
#line 1167 "ceps.tab.c" // lalr1.cc:846
    break;

  case 92:
#line 591 "../src/grammar/ceps.y" // lalr1.cc:846
    { 
 	if (!ceps::ast::nlf_ptr((yystack_[0].value.ast_node))->empty()) 
 	 ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node)); 
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 1177 "ceps.tab.c" // lalr1.cc:846
    break;

  case 93:
#line 600 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Atoms();
}
#line 1185 "ceps.tab.c" // lalr1.cc:846
    break;

  case 94:
#line 604 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1194 "ceps.tab.c" // lalr1.cc:846
    break;

  case 95:
#line 609 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int(-1 * (yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1203 "ceps.tab.c" // lalr1.cc:846
    break;

  case 96:
#line 615 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1213 "ceps.tab.c" // lalr1.cc:846
    break;

  case 97:
#line 623 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1222 "ceps.tab.c" // lalr1.cc:846
    break;

  case 98:
#line 628 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double(-1.0*(yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1231 "ceps.tab.c" // lalr1.cc:846
    break;

  case 99:
#line 634 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1241 "ceps.tab.c" // lalr1.cc:846
    break;

  case 100:
#line 642 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr));
  delete (yystack_[0].value.sval);
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1251 "ceps.tab.c" // lalr1.cc:846
    break;


#line 1255 "ceps.tab.c" // lalr1.cc:846
          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Cepsparser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Cepsparser::yysyntax_error_ (state_type, symbol_number_type) const
  {
    return YY_("syntax error");
  }


  const short int Cepsparser::yypact_ninf_ = -187;

  const signed char Cepsparser::yytable_ninf_ = -30;

  const short int
  Cepsparser::yypact_[] =
  {
    -187,    17,   345,  -187,  -187,  -187,     1,    45,     0,   383,
       2,    29,    72,    85,    41,   383,   -14,  -187,    41,  -187,
    -187,  -187,   383,   383,   383,  -187,  -187,    62,  -187,   716,
    -187,    67,  -187,  -187,    66,    99,    53,  -187,  -187,   482,
     383,    87,    72,    84,  -187,  -187,  -187,  -187,  -187,   -40,
      -2,    94,    94,  -187,   125,   159,   515,    -2,   416,  -187,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   359,   383,  -187,
     383,   383,  -187,    97,   127,  -187,   549,  -187,  -187,    96,
     -16,  -187,  -187,  -187,   -19,  -187,  -187,  -187,    41,  -187,
    -187,  -187,   778,   747,   778,    71,   790,   817,   827,   852,
     860,   883,   889,    -6,    -6,    83,    83,    40,    -2,  -187,
     716,   -11,  -187,   449,   716,   716,   110,    53,   345,   383,
     111,   104,   113,   147,   207,   116,    -8,   221,  -187,  -187,
     383,  -187,  -187,   103,  -187,   151,   716,  -187,   383,  -187,
    -187,     6,  -187,  -187,   130,  -187,  -187,   138,   383,   131,
     103,   583,   132,  -187,  -187,  -187,   345,   269,   716,  -187,
    -187,   123,   133,  -187,   283,  -187,   160,   616,   383,   134,
     103,  -187,  -187,  -187,  -187,  -187,  -187,  -187,  -187,   331,
     383,   103,   649,  -187,  -187,  -187,   683,  -187,   103,   103,
     173,  -187,   103,  -187
  };

  const unsigned char
  Cepsparser::yydefact_[] =
  {
       4,     0,     2,     1,    38,    39,     0,     0,     0,     0,
       0,     0,   101,     0,     0,     0,    36,    35,    37,    34,
       4,    11,     0,     0,     0,     3,     7,     0,    19,    28,
       9,     0,    24,    23,     0,     0,    83,    36,    37,     0,
       0,     0,   101,     0,    26,    25,    15,    14,    16,     0,
      40,     0,     0,    17,    27,     0,     0,    41,     0,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
       0,     0,    86,     0,    84,     6,     0,    76,    75,     0,
       0,   102,    91,     4,     0,     4,    32,    30,     0,     5,
      59,    60,    42,    43,    52,    44,    45,    55,    54,    53,
      58,    56,    57,    47,    46,    48,    49,    50,    51,    62,
      89,     0,    87,     0,    22,    21,     0,    83,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,    31,    18,
       0,    61,    64,    65,    85,    12,    77,     4,     0,    20,
      93,    92,    81,     4,     0,    33,    88,     0,     0,     0,
      65,     0,     0,    66,    70,    71,     0,     0,    78,    94,
      97,     0,     0,   100,     0,     4,     0,     0,     0,     0,
      65,    63,    13,    79,    95,    98,    96,    99,    82,     0,
       0,    65,     0,    72,    67,    80,     0,    69,     0,    65,
      73,    68,     0,    74
  };

  const short int
  Cepsparser::yypgoto_[] =
  {
    -187,  -187,   -18,  -123,  -187,   -13,    90,  -187,  -187,  -187,
    -187,   135,    -9,  -157,  -186,  -187,    95,  -187,  -110,  -187,
      59,  -187,  -187,    51,  -187,  -187,  -187,   150
  };

  const short int
  Cepsparser::yydefgoto_[] =
  {
      -1,     1,     2,    25,    26,    53,    54,    27,    28,    52,
     138,    96,    29,   162,   163,   164,    89,    90,    30,    31,
      83,    84,   121,   122,   132,   133,   151,    43
  };

  const short int
  Cepsparser::yytable_[] =
  {
      39,    49,    55,   179,    93,   145,    50,    94,   140,   169,
     170,    98,   200,    56,    57,    58,   203,     3,    46,    47,
      48,    87,    88,   194,    51,    73,    74,    75,    76,   135,
     -29,    86,   130,   165,   197,   171,   172,   141,    32,    33,
     154,    77,   201,   182,    78,    77,   173,    36,    78,    40,
     165,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   120,   123,
     165,   124,   125,    75,    76,   134,    41,   137,    46,    47,
      48,   165,    34,    35,    42,   139,    80,    77,   165,   165,
      78,    82,   165,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,     4,     5,    59,   157,
       8,   158,   159,    79,    11,    74,    75,    76,    77,    81,
     146,    78,    44,    45,    87,    88,   184,   185,    92,   167,
      77,   120,    15,    78,   161,   174,   186,   187,    95,   168,
      37,    17,    38,    19,    98,   126,   127,   160,   129,   177,
      22,   161,    23,    24,   143,   147,   148,   189,   149,   150,
     153,   166,     4,     5,     6,     7,     8,     9,    10,   192,
      11,   161,    12,    13,   175,    14,   176,   181,   178,   193,
     190,   196,   161,   202,   136,   131,   144,    97,    15,   161,
     161,   156,    91,   161,     0,     0,    16,    17,    18,    19,
       0,     0,     0,    20,    99,    21,    22,     0,    23,    24,
       4,     5,     6,     7,     8,     9,    10,     0,    11,     0,
      12,    13,     0,    14,     4,     5,     6,     7,     8,     9,
      10,     0,    11,     0,    12,    13,    15,    14,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,     0,     0,
      15,    20,   152,    21,    22,     0,    23,    24,    16,    17,
      18,    19,     0,     0,     0,    20,   155,    21,    22,     0,
      23,    24,     4,     5,     6,     7,     8,     9,    10,     0,
      11,     0,    12,    13,     0,    14,     4,     5,     6,     7,
       8,     9,    10,     0,    11,     0,    12,    13,    15,    14,
       0,     0,     0,     0,     0,     0,    16,    17,    18,    19,
       0,     0,    15,    20,   183,    21,    22,     0,    23,    24,
      16,    17,    18,    19,     0,     0,     0,    20,   188,    21,
      22,     0,    23,    24,     4,     5,     6,     7,     8,     9,
      10,     0,    11,     0,    12,    13,     0,    14,     4,     5,
       6,     7,     8,     9,    10,     0,    11,     0,    12,    13,
      15,    14,     4,     5,     0,     0,     8,     0,    16,    17,
      18,    19,     0,     0,    15,    20,   195,    21,    22,     0,
      23,    24,    16,    17,    18,    19,     4,     5,    15,    20,
       8,    21,    22,     0,    23,    24,    37,    17,    38,    19,
       0,     0,     0,     0,     0,     0,    22,   119,    23,    24,
       0,     0,    15,     0,     0,     0,     0,     0,     0,     0,
      37,    17,    38,    19,     0,     0,     0,     0,     0,     0,
      22,    60,    23,    24,     0,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,    60,     0,    78,   101,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,    60,     0,    78,
     142,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    85,    77,
      60,     0,    78,     0,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    77,   100,    60,    78,     0,     0,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,   128,    60,    78,
       0,     0,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
      77,    60,     0,    78,     0,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   191,    77,    60,     0,    78,     0,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,   198,    60,    78,
       0,     0,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   199,
      77,    60,     0,    78,     0,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    77,     0,     0,    78,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,     0,     0,    78,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,    77,     0,     0,
      78,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,     0,     0,    77,     0,     0,    78,     0,     0,
       0,     0,     0,     0,    77,     0,     0,    78,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    69,    70,    71,
      72,    73,    74,    75,    76,     0,     0,     0,     0,    77,
       0,     0,    78,     0,     0,     0,     0,    77,     0,     0,
      78,    70,    71,    72,    73,    74,    75,    76,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,     0,     0,
      77,     0,     0,    78,     0,     0,    77,     0,     0,    78
  };

  const short int
  Cepsparser::yycheck_[] =
  {
       9,    14,    20,   160,    44,   128,    15,    47,    19,     3,
       4,    19,   198,    22,    23,    24,   202,     0,    37,    38,
      39,    37,    38,   180,    38,    31,    32,    33,    34,    48,
      44,    40,    48,   143,   191,    29,    30,    48,    37,    38,
      48,    47,   199,   166,    50,    47,    40,    47,    50,    47,
     160,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
     180,    80,    81,    33,    34,    93,    47,    95,    37,    38,
      39,   191,    37,    38,    12,    98,    20,    47,   198,   199,
      50,    38,   202,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,     3,     4,    46,     6,
       7,     8,     9,    46,    11,    32,    33,    34,    47,    20,
     129,    50,    37,    38,    37,    38,     3,     4,    44,   147,
      47,   140,    29,    50,   143,   153,     3,     4,    44,   148,
      37,    38,    39,    40,    19,    48,    19,    44,    52,   158,
      47,   160,    49,    50,    44,    44,    52,   175,    45,    12,
      44,    10,     3,     4,     5,     6,     7,     8,     9,   178,
      11,   180,    13,    14,    44,    16,    38,    45,    47,    45,
      20,   190,   191,    10,    94,    90,   127,    52,    29,   198,
     199,   140,    42,   202,    -1,    -1,    37,    38,    39,    40,
      -1,    -1,    -1,    44,    45,    46,    47,    -1,    49,    50,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      13,    14,    -1,    16,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    13,    14,    29,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    40,    -1,    -1,
      29,    44,    45,    46,    47,    -1,    49,    50,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    45,    46,    47,    -1,
      49,    50,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    13,    14,    -1,    16,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    13,    14,    29,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      -1,    -1,    29,    44,    45,    46,    47,    -1,    49,    50,
      37,    38,    39,    40,    -1,    -1,    -1,    44,    45,    46,
      47,    -1,    49,    50,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    13,    14,    -1,    16,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    13,    14,
      29,    16,     3,     4,    -1,    -1,     7,    -1,    37,    38,
      39,    40,    -1,    -1,    29,    44,    45,    46,    47,    -1,
      49,    50,    37,    38,    39,    40,     3,     4,    29,    44,
       7,    46,    47,    -1,    49,    50,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,    50,
      -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    15,    49,    50,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    15,    -1,    50,    51,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    15,    -1,    50,
      51,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      15,    -1,    50,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    48,    15,    50,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    15,    50,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    15,    -1,    50,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    15,    -1,    50,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    15,    50,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    15,    -1,    50,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    47,    -1,    -1,    50,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    50,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    47,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      50,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    50,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      50,    28,    29,    30,    31,    32,    33,    34,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,    47,    -1,    -1,    50
  };

  const unsigned char
  Cepsparser::yystos_[] =
  {
       0,    54,    55,     0,     3,     4,     5,     6,     7,     8,
       9,    11,    13,    14,    16,    29,    37,    38,    39,    40,
      44,    46,    47,    49,    50,    56,    57,    60,    61,    65,
      71,    72,    37,    38,    37,    38,    47,    37,    39,    65,
      47,    47,    12,    80,    37,    38,    37,    38,    39,    58,
      65,    38,    62,    58,    59,    55,    65,    65,    65,    46,
      15,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    47,    50,    46,
      20,    20,    38,    73,    74,    46,    65,    37,    38,    69,
      70,    80,    44,    44,    47,    44,    64,    64,    19,    45,
      48,    51,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    48,
      65,    75,    76,    65,    65,    65,    48,    19,    48,    52,
      48,    69,    77,    78,    55,    48,    59,    55,    63,    58,
      19,    48,    51,    44,    73,    56,    65,    44,    52,    45,
      12,    79,    45,    44,    48,    45,    76,     6,     8,     9,
      44,    65,    66,    67,    68,    71,    10,    55,    65,     3,
       4,    29,    30,    40,    55,    44,    38,    65,    47,    66,
      46,    45,    56,    45,     3,     4,     3,     4,    45,    55,
      20,    46,    65,    45,    66,    45,    65,    66,    48,    46,
      67,    66,    10,    67
  };

  const unsigned char
  Cepsparser::yyr1_[] =
  {
       0,    53,    54,    55,    55,    56,    56,    56,    56,    56,
      56,    56,    57,    57,    58,    58,    58,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    62,
      63,    61,    61,    64,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    66,    66,    67,    67,    67,
      67,    67,    67,    68,    68,    69,    69,    70,    70,    71,
      72,    72,    72,    73,    73,    73,    74,    75,    75,    76,
      77,    78,    78,    79,    79,    79,    79,    79,    79,    79,
      79,    80,    80
  };

  const unsigned char
  Cepsparser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     3,     3,     1,     2,     1,
       2,     1,     5,     7,     1,     1,     1,     1,     3,     1,
       5,     4,     4,     2,     2,     2,     2,     2,     1,     0,
       0,     4,     3,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     3,     7,     4,     0,     1,     3,     6,     4,
       1,     1,     3,     5,     7,     1,     1,     3,     4,     7,
       8,     5,     7,     0,     1,     3,     1,     1,     3,     1,
       1,     0,     2,     1,     2,     3,     3,     2,     3,     3,
       2,     0,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Cepsparser::yytname_[] =
  {
  "END", "error", "$undefined", "INTNUM", "FLOATNUM", "STRUCT", "VAL",
  "FUN", "RET", "IF", "ELSE", "FOR", "EOL", "RAWMAP", "KIND", "DOTDOT",
  "TEMPLATE", "TEMPLATE_ID", "TEMPLATE_PARAM", "','", "'='", "'|'", "'&'",
  "REL_OP_GT", "REL_OP_LT", "REL_OP_EQ", "REL_OP_NEQ", "REL_OP_LT_EQ",
  "REL_OP_GT_EQ", "'-'", "'+'", "'*'", "'/'", "'^'", "'.'", "NEG", "NOT",
  "STRUCTID", "ID", "KINDID", "LITERAL", "METRIC_UNIT", "FUNCCALL",
  "INDEXOP", "'{'", "'}'", "';'", "'('", "')'", "'!'", "'['", "']'", "':'",
  "$accept", "cepsscript", "stmts", "stmt", "ifthenelse", "general_id",
  "id_list", "decl", "struct_decl", "$@1", "$@2", "struct_initialization",
  "expr", "func_body", "func_stmts", "if_then_else", "id_or_struct_id",
  "for_loop_head", "for_loop", "template", "parameter_list", "parameter",
  "argument_list", "argument", "raw_map", "raw_lines", "raw_line", "eols", YY_NULL
  };


  const unsigned short int
  Cepsparser::yyrline_[] =
  {
       0,   139,   139,   148,   156,   163,   164,   168,   174,   179,
     185,   189,   196,   200,   208,   209,   210,   214,   218,   226,
     230,   234,   239,   245,   251,   256,   264,   272,   282,   289,
     289,   289,   293,   300,   308,   314,   319,   325,   330,   336,
     342,   349,   356,   361,   366,   370,   375,   380,   386,   391,
     396,   401,   406,   410,   414,   418,   422,   426,   430,   434,
     438,   443,   448,   454,   458,   464,   466,   470,   471,   472,
     473,   474,   475,   479,   480,   484,   489,   497,   504,   515,
     526,   532,   536,   542,   544,   545,   549,   553,   558,   568,
     579,   587,   590,   599,   603,   608,   614,   622,   627,   633,
     641,   651,   653
  };

  // Print the state stack on the debug stream.
  void
  Cepsparser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Cepsparser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  Cepsparser::token_number_type
  Cepsparser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    49,     2,     2,     2,     2,    22,     2,
      47,    48,    31,    30,    19,    29,    34,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    46,
       2,    20,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,    33,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,    21,    45,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    23,    24,    25,    26,    27,    28,
      35,    36,    37,    38,    39,    40,    41,    42,    43
    };
    const unsigned int user_token_number_max_ = 288;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 31 "../src/grammar/ceps.y" // lalr1.cc:1156
} // ceps
#line 1868 "ceps.tab.c" // lalr1.cc:1156
#line 656 "../src/grammar/ceps.y" // lalr1.cc:1157


void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
