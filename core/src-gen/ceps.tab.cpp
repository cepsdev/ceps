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
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 755 "ceps.tab.c" // lalr1.cc:846
    break;

  case 28:
#line 281 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto r = new ceps::ast::Kinddef(*(yystack_[1].value.sval),nullptr,nullptr,nullptr);
 for (auto s : *(yystack_[0].value.str_list))
 {
  r->children().push_back(new ceps::ast::Identifier(s,nullptr,nullptr,nullptr)); 
 }
 delete (yystack_[0].value.str_list); 
 (yylhs.value.ast_node) = r;
}
#line 769 "ceps.tab.c" // lalr1.cc:846
    break;

  case 29:
#line 291 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 777 "ceps.tab.c" // lalr1.cc:846
    break;

  case 30:
#line 297 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().push_scope();}
#line 783 "ceps.tab.c" // lalr1.cc:846
    break;

  case 31:
#line 297 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().pop_scope();}
#line 789 "ceps.tab.c" // lalr1.cc:846
    break;

  case 32:
#line 298 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[3].value.sval),(yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 797 "ceps.tab.c" // lalr1.cc:846
    break;

  case 33:
#line 302 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[2].value.sval),new ceps::ast::Identifier(*(yystack_[1].value.sval),nullptr,nullptr,nullptr),(yystack_[0].value.ast_node),nullptr);
}
#line 805 "ceps.tab.c" // lalr1.cc:846
    break;

  case 34:
#line 309 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 813 "ceps.tab.c" // lalr1.cc:846
    break;

  case 35:
#line 317 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
 	delete (yystack_[0].value.sval);
 }
#line 822 "ceps.tab.c" // lalr1.cc:846
    break;

  case 36:
#line 323 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
    delete (yystack_[0].value.sval);
  }
#line 831 "ceps.tab.c" // lalr1.cc:846
    break;

  case 37:
#line 329 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 840 "ceps.tab.c" // lalr1.cc:846
    break;

  case 38:
#line 334 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO:Handle kindid
}
#line 848 "ceps.tab.c" // lalr1.cc:846
    break;

  case 39:
#line 340 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}
#line 856 "ceps.tab.c" // lalr1.cc:846
    break;

  case 40:
#line 346 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}
#line 864 "ceps.tab.c" // lalr1.cc:846
    break;

  case 41:
#line 352 "../src/grammar/ceps.y" // lalr1.cc:846
    {  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('-',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 874 "ceps.tab.c" // lalr1.cc:846
    break;

  case 42:
#line 359 "../src/grammar/ceps.y" // lalr1.cc:846
    {  
	
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('!',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 883 "ceps.tab.c" // lalr1.cc:846
    break;

  case 43:
#line 365 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::DOTDOT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 891 "ceps.tab.c" // lalr1.cc:846
    break;

  case 44:
#line 370 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(',',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 899 "ceps.tab.c" // lalr1.cc:846
    break;

  case 45:
#line 375 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('|',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 907 "ceps.tab.c" // lalr1.cc:846
    break;

  case 46:
#line 379 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('&',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 915 "ceps.tab.c" // lalr1.cc:846
    break;

  case 47:
#line 385 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('+',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 923 "ceps.tab.c" // lalr1.cc:846
    break;

  case 48:
#line 390 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('-',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 931 "ceps.tab.c" // lalr1.cc:846
    break;

  case 49:
#line 396 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('*',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);			
}
#line 939 "ceps.tab.c" // lalr1.cc:846
    break;

  case 50:
#line 401 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('/',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 947 "ceps.tab.c" // lalr1.cc:846
    break;

  case 51:
#line 406 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('^',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 955 "ceps.tab.c" // lalr1.cc:846
    break;

  case 52:
#line 410 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('.',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 963 "ceps.tab.c" // lalr1.cc:846
    break;

  case 53:
#line 415 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('=',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 971 "ceps.tab.c" // lalr1.cc:846
    break;

  case 54:
#line 419 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 979 "ceps.tab.c" // lalr1.cc:846
    break;

  case 55:
#line 423 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 987 "ceps.tab.c" // lalr1.cc:846
    break;

  case 56:
#line 427 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 995 "ceps.tab.c" // lalr1.cc:846
    break;

  case 57:
#line 431 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1003 "ceps.tab.c" // lalr1.cc:846
    break;

  case 58:
#line 435 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1011 "ceps.tab.c" // lalr1.cc:846
    break;

  case 59:
#line 439 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_NEQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1019 "ceps.tab.c" // lalr1.cc:846
    break;

  case 60:
#line 443 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1027 "ceps.tab.c" // lalr1.cc:846
    break;

  case 61:
#line 447 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1035 "ceps.tab.c" // lalr1.cc:846
    break;

  case 62:
#line 452 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call{(yystack_[3].value.ast_node),(yystack_[1].value.ast_node)};
}
#line 1044 "ceps.tab.c" // lalr1.cc:846
    break;

  case 63:
#line 457 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call((yystack_[2].value.ast_node),new  ceps::ast::Call_parameters(), nullptr);
}
#line 1053 "ceps.tab.c" // lalr1.cc:846
    break;

  case 64:
#line 463 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO: Function Definitions
}
#line 1061 "ceps.tab.c" // lalr1.cc:846
    break;

  case 76:
#line 493 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
 }
#line 1070 "ceps.tab.c" // lalr1.cc:846
    break;

  case 77:
#line 498 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 1079 "ceps.tab.c" // lalr1.cc:846
    break;

  case 78:
#line 506 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1090 "ceps.tab.c" // lalr1.cc:846
    break;

  case 79:
#line 513 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr((yystack_[3].value.ast_node));
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1101 "ceps.tab.c" // lalr1.cc:846
    break;

  case 80:
#line 524 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back((yystack_[4].value.ast_node));
  temp->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = temp;
 }
#line 1113 "ceps.tab.c" // lalr1.cc:846
    break;

  case 81:
#line 535 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto temp = new ceps::ast::Template_defintion(*(yystack_[6].value.sval),*(yystack_[4].value.str_list),nullptr,nullptr,nullptr);
  
}
#line 1122 "ceps.tab.c" // lalr1.cc:846
    break;

  case 82:
#line 541 "../src/grammar/ceps.y" // lalr1.cc:846
    {
}
#line 1129 "ceps.tab.c" // lalr1.cc:846
    break;

  case 83:
#line 545 "../src/grammar/ceps.y" // lalr1.cc:846
    {
}
#line 1136 "ceps.tab.c" // lalr1.cc:846
    break;

  case 88:
#line 562 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = new ceps::ast::Call_parameters((yystack_[0].value.ast_node),nullptr,nullptr);
 }
#line 1144 "ceps.tab.c" // lalr1.cc:846
    break;

  case 89:
#line 567 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
    
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 1155 "ceps.tab.c" // lalr1.cc:846
    break;

  case 90:
#line 577 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
 }
#line 1163 "ceps.tab.c" // lalr1.cc:846
    break;

  case 91:
#line 588 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node)=(yystack_[0].value.ast_node);
 }
#line 1171 "ceps.tab.c" // lalr1.cc:846
    break;

  case 92:
#line 595 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::Rawmap();
 }
#line 1179 "ceps.tab.c" // lalr1.cc:846
    break;

  case 93:
#line 599 "../src/grammar/ceps.y" // lalr1.cc:846
    { 
 	if (!ceps::ast::nlf_ptr((yystack_[0].value.ast_node))->empty()) 
 	 ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node)); 
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 1189 "ceps.tab.c" // lalr1.cc:846
    break;

  case 94:
#line 608 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Atoms();
}
#line 1197 "ceps.tab.c" // lalr1.cc:846
    break;

  case 95:
#line 612 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1206 "ceps.tab.c" // lalr1.cc:846
    break;

  case 96:
#line 617 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int(-1 * (yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1215 "ceps.tab.c" // lalr1.cc:846
    break;

  case 97:
#line 623 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1225 "ceps.tab.c" // lalr1.cc:846
    break;

  case 98:
#line 631 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1234 "ceps.tab.c" // lalr1.cc:846
    break;

  case 99:
#line 636 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double(-1.0*(yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1243 "ceps.tab.c" // lalr1.cc:846
    break;

  case 100:
#line 642 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1253 "ceps.tab.c" // lalr1.cc:846
    break;

  case 101:
#line 650 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr));
  delete (yystack_[0].value.sval);
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1263 "ceps.tab.c" // lalr1.cc:846
    break;


#line 1267 "ceps.tab.c" // lalr1.cc:846
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


  const short int Cepsparser::yypact_ninf_ = -178;

  const signed char Cepsparser::yytable_ninf_ = -31;

  const short int
  Cepsparser::yypact_[] =
  {
    -178,    28,   346,  -178,  -178,  -178,   -17,     5,    30,   360,
      36,    62,   102,    89,   100,   360,   -21,  -178,   100,  -178,
    -178,  -178,   360,   360,   360,  -178,  -178,    83,  -178,   693,
    -178,    85,  -178,  -178,   125,   126,    84,  -178,  -178,   459,
     360,    50,   102,   103,  -178,  -178,  -178,  -178,  -178,  -178,
     -14,    34,   105,   105,  -178,   136,   160,   492,    34,   393,
    -178,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   360,   360,   360,   360,     0,   360,
    -178,   360,   360,  -178,   108,   138,  -178,   526,  -178,  -178,
     127,    37,  -178,  -178,  -178,    86,  -178,  -178,  -178,   100,
    -178,  -178,  -178,   755,   724,   755,   784,   796,    70,   822,
     831,   855,   862,   884,   128,   128,   -23,   -23,   -15,    34,
    -178,   693,    -7,  -178,   426,   693,   693,   133,    84,   346,
     360,   137,   132,   135,   173,   208,   142,    -3,   222,  -178,
    -178,   360,  -178,  -178,   104,  -178,   177,   693,  -178,   360,
    -178,  -178,    76,  -178,  -178,   144,  -178,  -178,   154,   360,
     146,   104,   560,   150,  -178,  -178,  -178,   346,   270,   693,
    -178,  -178,    88,   115,  -178,   284,  -178,   176,   593,   360,
     156,   104,  -178,  -178,  -178,  -178,  -178,  -178,  -178,  -178,
     332,   360,   104,   626,  -178,  -178,  -178,   660,  -178,   104,
     104,   192,  -178,   104,  -178
  };

  const unsigned char
  Cepsparser::yydefact_[] =
  {
       4,     0,     2,     1,    39,    40,     0,     0,     0,     0,
       0,     0,   102,     0,     0,     0,    37,    36,    38,    35,
       4,    11,     0,     0,     0,     3,     7,     0,    19,    29,
       9,     0,    24,    23,     0,     0,    84,    37,    38,     0,
       0,     0,   102,     0,    26,    25,    27,    15,    14,    16,
       0,    41,     0,     0,    17,    28,     0,     0,    42,     0,
       8,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      10,     0,     0,    87,     0,    85,     6,     0,    77,    76,
       0,     0,   103,    92,     4,     0,     4,    33,    31,     0,
       5,    60,    61,    43,    44,    53,    45,    46,    56,    55,
      54,    59,    57,    58,    48,    47,    49,    50,    51,    52,
      63,    90,     0,    88,     0,    22,    21,     0,    84,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,    32,
      18,     0,    62,    65,    66,    86,    12,    78,     4,     0,
      20,    94,    93,    82,     4,     0,    34,    89,     0,     0,
       0,    66,     0,     0,    67,    71,    72,     0,     0,    79,
      95,    98,     0,     0,   101,     0,     4,     0,     0,     0,
       0,    66,    64,    13,    80,    96,    99,    97,   100,    83,
       0,     0,    66,     0,    73,    68,    81,     0,    70,     0,
      66,    74,    69,     0,    75
  };

  const short int
  Cepsparser::yypgoto_[] =
  {
    -178,  -178,   -18,  -121,  -178,   -13,   113,  -178,  -178,  -178,
    -178,   165,    -9,  -156,  -177,  -178,   112,  -178,  -110,  -178,
      92,  -178,  -178,    82,  -178,  -178,  -178,   190
  };

  const short int
  Cepsparser::yydefgoto_[] =
  {
      -1,     1,     2,    25,    26,    54,    55,    27,    28,    53,
     139,    97,    29,   163,   164,   165,    90,    91,    30,    31,
      84,    85,   122,   123,   133,   134,   152,    43
  };

  const short int
  Cepsparser::yytable_[] =
  {
      39,    50,    56,     4,     5,   180,    51,     8,   146,    75,
      76,    77,   141,    57,    58,    59,    99,    52,    76,    77,
      32,    33,   201,   -30,    78,   195,   204,    79,     3,    15,
      94,    87,    78,    95,   166,    79,   198,    37,    17,    38,
      19,   142,    34,    35,   202,   155,   183,    22,   120,    23,
      24,   166,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   121,
     124,   166,   125,   126,    88,    89,   135,    36,   138,   170,
     171,    78,   166,    40,    79,   131,   140,    88,    89,   166,
     166,   185,   186,   166,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,   172,   173,     4,     5,    41,
     158,     8,   159,   160,    42,    11,   174,    78,   187,   188,
      79,   147,    83,    47,    48,    49,    44,    45,    46,    60,
     168,    80,   121,    15,   136,   162,   175,    47,    48,    49,
     169,    37,    17,    38,    19,    81,    82,    93,   161,    96,
     178,    22,   162,    23,    24,    99,   127,   128,   190,    74,
      75,    76,    77,     4,     5,     6,     7,     8,     9,    10,
     193,    11,   162,    12,    13,    78,    14,   144,    79,   130,
     150,   148,   197,   162,   149,   151,   154,   167,   176,    15,
     162,   162,   177,   179,   162,   182,   191,    16,    17,    18,
      19,   194,   203,   132,    20,   100,    21,    22,   137,    23,
      24,     4,     5,     6,     7,     8,     9,    10,    98,    11,
     145,    12,    13,   157,    14,     4,     5,     6,     7,     8,
       9,    10,    92,    11,     0,    12,    13,    15,    14,     0,
       0,     0,     0,     0,     0,    16,    17,    18,    19,     0,
       0,    15,    20,   153,    21,    22,     0,    23,    24,    16,
      17,    18,    19,     0,     0,     0,    20,   156,    21,    22,
       0,    23,    24,     4,     5,     6,     7,     8,     9,    10,
       0,    11,     0,    12,    13,     0,    14,     4,     5,     6,
       7,     8,     9,    10,     0,    11,     0,    12,    13,    15,
      14,     0,     0,     0,     0,     0,     0,    16,    17,    18,
      19,     0,     0,    15,    20,   184,    21,    22,     0,    23,
      24,    16,    17,    18,    19,     0,     0,     0,    20,   189,
      21,    22,     0,    23,    24,     4,     5,     6,     7,     8,
       9,    10,     0,    11,     0,    12,    13,     0,    14,     4,
       5,     6,     7,     8,     9,    10,     0,    11,     0,    12,
      13,    15,    14,     4,     5,     0,     0,     8,     0,    16,
      17,    18,    19,     0,     0,    15,    20,   196,    21,    22,
       0,    23,    24,    16,    17,    18,    19,     0,     0,    15,
      20,     0,    21,    22,     0,    23,    24,    37,    17,    38,
      19,     0,     0,     0,     0,     0,     0,    22,    61,    23,
      24,     0,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,    61,     0,    79,   102,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,    61,     0,    79,   143,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    78,    61,     0,    79,
       0,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
     101,    61,    79,     0,     0,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,   129,    61,    79,     0,     0,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,    78,    61,     0,
      79,     0,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   192,
      78,    61,     0,    79,     0,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,   199,    61,    79,     0,     0,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   200,    78,    61,     0,
      79,     0,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      78,     0,     0,    79,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,    79,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,    79,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,    79,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    69,    70,    71,
      72,    73,    74,    75,    76,    77,     0,     0,     0,    78,
       0,     0,    79,     0,     0,     0,     0,     0,    78,     0,
       0,    79,    70,    71,    72,    73,    74,    75,    76,    77,
      71,    72,    73,    74,    75,    76,    77,     0,     0,     0,
       0,     0,    78,     0,     0,    79,     0,     0,     0,    78,
       0,     0,    79,    72,    73,    74,    75,    76,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,    79
  };

  const short int
  Cepsparser::yycheck_[] =
  {
       9,    14,    20,     3,     4,   161,    15,     7,   129,    32,
      33,    34,    19,    22,    23,    24,    19,    38,    33,    34,
      37,    38,   199,    44,    47,   181,   203,    50,     0,    29,
      44,    40,    47,    47,   144,    50,   192,    37,    38,    39,
      40,    48,    37,    38,   200,    48,   167,    47,    48,    49,
      50,   161,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,   181,    81,    82,    37,    38,    94,    47,    96,     3,
       4,    47,   192,    47,    50,    48,    99,    37,    38,   199,
     200,     3,     4,   203,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    29,    30,     3,     4,    47,
       6,     7,     8,     9,    12,    11,    40,    47,     3,     4,
      50,   130,    38,    37,    38,    39,    37,    38,    39,    46,
     148,    46,   141,    29,    48,   144,   154,    37,    38,    39,
     149,    37,    38,    39,    40,    20,    20,    44,    44,    44,
     159,    47,   161,    49,    50,    19,    48,    19,   176,    31,
      32,    33,    34,     3,     4,     5,     6,     7,     8,     9,
     179,    11,   181,    13,    14,    47,    16,    44,    50,    52,
      45,    44,   191,   192,    52,    12,    44,    10,    44,    29,
     199,   200,    38,    47,   203,    45,    20,    37,    38,    39,
      40,    45,    10,    91,    44,    45,    46,    47,    95,    49,
      50,     3,     4,     5,     6,     7,     8,     9,    53,    11,
     128,    13,    14,   141,    16,     3,     4,     5,     6,     7,
       8,     9,    42,    11,    -1,    13,    14,    29,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,    -1,
      -1,    29,    44,    45,    46,    47,    -1,    49,    50,    37,
      38,    39,    40,    -1,    -1,    -1,    44,    45,    46,    47,
      -1,    49,    50,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    13,    14,    -1,    16,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    -1,    13,    14,    29,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,
      40,    -1,    -1,    29,    44,    45,    46,    47,    -1,    49,
      50,    37,    38,    39,    40,    -1,    -1,    -1,    44,    45,
      46,    47,    -1,    49,    50,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    13,    14,    -1,    16,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    13,
      14,    29,    16,     3,     4,    -1,    -1,     7,    -1,    37,
      38,    39,    40,    -1,    -1,    29,    44,    45,    46,    47,
      -1,    49,    50,    37,    38,    39,    40,    -1,    -1,    29,
      44,    -1,    46,    47,    -1,    49,    50,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    47,    15,    49,
      50,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    15,    -1,    50,    51,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    15,    -1,    50,    51,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    15,    -1,    50,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    15,    50,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    15,    50,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    15,    -1,
      50,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    15,    -1,    50,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    48,    15,    50,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    15,    -1,
      50,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      47,    -1,    -1,    50,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    50,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    50,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    47,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    -1,    50,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    50,    27,    28,    29,    30,    31,    32,    33,    34,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,    -1,    47,
      -1,    -1,    50,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    50
  };

  const unsigned char
  Cepsparser::yystos_[] =
  {
       0,    54,    55,     0,     3,     4,     5,     6,     7,     8,
       9,    11,    13,    14,    16,    29,    37,    38,    39,    40,
      44,    46,    47,    49,    50,    56,    57,    60,    61,    65,
      71,    72,    37,    38,    37,    38,    47,    37,    39,    65,
      47,    47,    12,    80,    37,    38,    39,    37,    38,    39,
      58,    65,    38,    62,    58,    59,    55,    65,    65,    65,
      46,    15,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    47,    50,
      46,    20,    20,    38,    73,    74,    46,    65,    37,    38,
      69,    70,    80,    44,    44,    47,    44,    64,    64,    19,
      45,    48,    51,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      48,    65,    75,    76,    65,    65,    65,    48,    19,    48,
      52,    48,    69,    77,    78,    55,    48,    59,    55,    63,
      58,    19,    48,    51,    44,    73,    56,    65,    44,    52,
      45,    12,    79,    45,    44,    48,    45,    76,     6,     8,
       9,    44,    65,    66,    67,    68,    71,    10,    55,    65,
       3,     4,    29,    30,    40,    55,    44,    38,    65,    47,
      66,    46,    45,    56,    45,     3,     4,     3,     4,    45,
      55,    20,    46,    65,    45,    66,    45,    65,    66,    48,
      46,    67,    66,    10,    67
  };

  const unsigned char
  Cepsparser::yyr1_[] =
  {
       0,    53,    54,    55,    55,    56,    56,    56,    56,    56,
      56,    56,    57,    57,    58,    58,    58,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      62,    63,    61,    61,    64,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    66,    66,    67,    67,
      67,    67,    67,    67,    68,    68,    69,    69,    70,    70,
      71,    72,    72,    72,    73,    73,    73,    74,    75,    75,
      76,    77,    78,    78,    79,    79,    79,    79,    79,    79,
      79,    79,    80,    80
  };

  const unsigned char
  Cepsparser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     3,     3,     1,     2,     1,
       2,     1,     5,     7,     1,     1,     1,     1,     3,     1,
       5,     4,     4,     2,     2,     2,     2,     2,     2,     1,
       0,     0,     4,     3,     3,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     3,     7,     4,     0,     1,     3,     6,
       4,     1,     1,     3,     5,     7,     1,     1,     3,     4,
       7,     8,     5,     7,     0,     1,     3,     1,     1,     3,
       1,     1,     0,     2,     1,     2,     3,     3,     2,     3,
       3,     2,     0,     2
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
     230,   234,   239,   245,   251,   256,   264,   272,   280,   290,
     297,   297,   297,   301,   308,   316,   322,   327,   333,   338,
     344,   350,   357,   364,   369,   374,   378,   383,   388,   394,
     399,   404,   409,   414,   418,   422,   426,   430,   434,   438,
     442,   446,   451,   456,   462,   466,   472,   474,   478,   479,
     480,   481,   482,   483,   487,   488,   492,   497,   505,   512,
     523,   534,   540,   544,   550,   552,   553,   557,   561,   566,
     576,   587,   595,   598,   607,   611,   616,   622,   630,   635,
     641,   649,   659,   661
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
#line 1880 "ceps.tab.c" // lalr1.cc:1156
#line 664 "../src/grammar/ceps.y" // lalr1.cc:1157


void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
