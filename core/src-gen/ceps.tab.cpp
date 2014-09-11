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
#line 120 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Root_ptr root_node = new ceps::ast::Root((yystack_[0].value.ast_node));
  driver.set_parsetree(ceps::ast::Parsetree{root_node});
  (yylhs.value.ast_node) = root_node;
 }
#line 561 "ceps.tab.c" // lalr1.cc:846
    break;

  case 3:
#line 129 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[1].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 571 "ceps.tab.c" // lalr1.cc:846
    break;

  case 4:
#line 136 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Stmts{}; //create empty Statements-Node
}
#line 579 "ceps.tab.c" // lalr1.cc:846
    break;

  case 5:
#line 143 "../src/grammar/ceps.y" // lalr1.cc:846
    { (yylhs.value.ast_node) = new ceps::ast::Scope{(yystack_[1].value.ast_node)};}
#line 585 "ceps.tab.c" // lalr1.cc:846
    break;

  case 6:
#line 148 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of decl
 }
#line 593 "ceps.tab.c" // lalr1.cc:846
    break;

  case 7:
#line 153 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of for_loop
 }
#line 601 "ceps.tab.c" // lalr1.cc:846
    break;

  case 8:
#line 158 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = nullptr;
 }
#line 609 "ceps.tab.c" // lalr1.cc:846
    break;

  case 12:
#line 171 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.str_list) = new std::vector<std::string> {*(yystack_[0].value.sval)};
 }
#line 617 "ceps.tab.c" // lalr1.cc:846
    break;

  case 13:
#line 175 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yystack_[2].value.str_list)->push_back(*(yystack_[0].value.sval));
   (yylhs.value.str_list) = (yystack_[2].value.str_list);
 }
#line 626 "ceps.tab.c" // lalr1.cc:846
    break;

  case 14:
#line 183 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result 
 }
#line 634 "ceps.tab.c" // lalr1.cc:846
    break;

  case 15:
#line 187 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result
}
#line 642 "ceps.tab.c" // lalr1.cc:846
    break;

  case 16:
#line 191 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 651 "ceps.tab.c" // lalr1.cc:846
    break;

  case 17:
#line 196 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 660 "ceps.tab.c" // lalr1.cc:846
    break;

  case 18:
#line 202 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
	delete (yystack_[0].value.sval); 
	(yylhs.value.ast_node)=nullptr;
}
#line 670 "ceps.tab.c" // lalr1.cc:846
    break;

  case 19:
#line 208 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	delete (yystack_[0].value.sval);
	(yylhs.value.ast_node)=nullptr;
}
#line 679 "ceps.tab.c" // lalr1.cc:846
    break;

  case 20:
#line 213 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 691 "ceps.tab.c" // lalr1.cc:846
    break;

  case 21:
#line 221 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 703 "ceps.tab.c" // lalr1.cc:846
    break;

  case 22:
#line 229 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 auto r = new ceps::ast::Kinddef(*(yystack_[1].value.sval),nullptr,nullptr,nullptr);
 for (auto s : *(yystack_[0].value.str_list))
 {
  r->children().push_back(new ceps::ast::Identifier(s,nullptr,nullptr,nullptr)); 
 }
 delete (yystack_[0].value.str_list); 
 (yylhs.value.ast_node) = r;
}
#line 717 "ceps.tab.c" // lalr1.cc:846
    break;

  case 23:
#line 239 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 725 "ceps.tab.c" // lalr1.cc:846
    break;

  case 24:
#line 245 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().push_scope();}
#line 731 "ceps.tab.c" // lalr1.cc:846
    break;

  case 25:
#line 245 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().pop_scope();}
#line 737 "ceps.tab.c" // lalr1.cc:846
    break;

  case 26:
#line 246 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[3].value.sval),(yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 745 "ceps.tab.c" // lalr1.cc:846
    break;

  case 27:
#line 250 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[2].value.sval),new ceps::ast::Identifier(*(yystack_[1].value.sval),nullptr,nullptr,nullptr),(yystack_[0].value.ast_node),nullptr);
}
#line 753 "ceps.tab.c" // lalr1.cc:846
    break;

  case 28:
#line 257 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 761 "ceps.tab.c" // lalr1.cc:846
    break;

  case 29:
#line 265 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
 	delete (yystack_[0].value.sval);
 }
#line 770 "ceps.tab.c" // lalr1.cc:846
    break;

  case 30:
#line 271 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
    delete (yystack_[0].value.sval);
  }
#line 779 "ceps.tab.c" // lalr1.cc:846
    break;

  case 31:
#line 277 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 788 "ceps.tab.c" // lalr1.cc:846
    break;

  case 32:
#line 282 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO:Handle kindid
}
#line 796 "ceps.tab.c" // lalr1.cc:846
    break;

  case 33:
#line 288 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}
#line 804 "ceps.tab.c" // lalr1.cc:846
    break;

  case 34:
#line 294 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}
#line 812 "ceps.tab.c" // lalr1.cc:846
    break;

  case 35:
#line 300 "../src/grammar/ceps.y" // lalr1.cc:846
    {  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('-',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 822 "ceps.tab.c" // lalr1.cc:846
    break;

  case 36:
#line 308 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(',',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 830 "ceps.tab.c" // lalr1.cc:846
    break;

  case 37:
#line 314 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('+',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 838 "ceps.tab.c" // lalr1.cc:846
    break;

  case 38:
#line 319 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('-',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 846 "ceps.tab.c" // lalr1.cc:846
    break;

  case 39:
#line 325 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('*',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);			
}
#line 854 "ceps.tab.c" // lalr1.cc:846
    break;

  case 40:
#line 330 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('/',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 862 "ceps.tab.c" // lalr1.cc:846
    break;

  case 41:
#line 335 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('^',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 870 "ceps.tab.c" // lalr1.cc:846
    break;

  case 42:
#line 339 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('.',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 878 "ceps.tab.c" // lalr1.cc:846
    break;

  case 43:
#line 344 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('=',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 886 "ceps.tab.c" // lalr1.cc:846
    break;

  case 44:
#line 348 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 894 "ceps.tab.c" // lalr1.cc:846
    break;

  case 45:
#line 352 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 902 "ceps.tab.c" // lalr1.cc:846
    break;

  case 46:
#line 357 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call{(yystack_[3].value.ast_node),(yystack_[1].value.ast_node)};
}
#line 911 "ceps.tab.c" // lalr1.cc:846
    break;

  case 47:
#line 362 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call((yystack_[2].value.ast_node),new  ceps::ast::Call_parameters(), nullptr);
}
#line 920 "ceps.tab.c" // lalr1.cc:846
    break;

  case 48:
#line 368 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO: Function Definitions
}
#line 928 "ceps.tab.c" // lalr1.cc:846
    break;

  case 60:
#line 397 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
 }
#line 937 "ceps.tab.c" // lalr1.cc:846
    break;

  case 61:
#line 402 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 946 "ceps.tab.c" // lalr1.cc:846
    break;

  case 62:
#line 410 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 957 "ceps.tab.c" // lalr1.cc:846
    break;

  case 63:
#line 417 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr((yystack_[3].value.ast_node));
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 968 "ceps.tab.c" // lalr1.cc:846
    break;

  case 64:
#line 428 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back((yystack_[4].value.ast_node));
  temp->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = temp;
 }
#line 980 "ceps.tab.c" // lalr1.cc:846
    break;

  case 69:
#line 450 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = new ceps::ast::Call_parameters((yystack_[0].value.ast_node),nullptr,nullptr);
 }
#line 988 "ceps.tab.c" // lalr1.cc:846
    break;

  case 70:
#line 455 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
    
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 999 "ceps.tab.c" // lalr1.cc:846
    break;

  case 71:
#line 465 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
 }
#line 1007 "ceps.tab.c" // lalr1.cc:846
    break;

  case 72:
#line 476 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node)=(yystack_[0].value.ast_node);
 }
#line 1015 "ceps.tab.c" // lalr1.cc:846
    break;

  case 73:
#line 483 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::Rawmap();
 }
#line 1023 "ceps.tab.c" // lalr1.cc:846
    break;

  case 74:
#line 487 "../src/grammar/ceps.y" // lalr1.cc:846
    { 
 	if (!ceps::ast::nlf_ptr((yystack_[0].value.ast_node))->empty()) 
 	 ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node)); 
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 1033 "ceps.tab.c" // lalr1.cc:846
    break;

  case 75:
#line 496 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Atoms();
}
#line 1041 "ceps.tab.c" // lalr1.cc:846
    break;

  case 76:
#line 500 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1050 "ceps.tab.c" // lalr1.cc:846
    break;

  case 77:
#line 505 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int(-1 * (yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1059 "ceps.tab.c" // lalr1.cc:846
    break;

  case 78:
#line 511 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1069 "ceps.tab.c" // lalr1.cc:846
    break;

  case 79:
#line 519 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1078 "ceps.tab.c" // lalr1.cc:846
    break;

  case 80:
#line 524 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double(-1.0*(yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1087 "ceps.tab.c" // lalr1.cc:846
    break;

  case 81:
#line 530 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1097 "ceps.tab.c" // lalr1.cc:846
    break;

  case 82:
#line 538 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr));
  delete (yystack_[0].value.sval);
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1107 "ceps.tab.c" // lalr1.cc:846
    break;


#line 1111 "ceps.tab.c" // lalr1.cc:846
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


  const short int Cepsparser::yypact_ninf_ = -129;

  const signed char Cepsparser::yytable_ninf_ = -25;

  const short int
  Cepsparser::yypact_[] =
  {
    -129,    15,    59,  -129,  -129,  -129,   -12,    43,   -11,     7,
       8,    50,   239,   -14,  -129,     9,  -129,  -129,   239,   239,
      40,  -129,  -129,   360,  -129,  -129,  -129,    86,    90,    85,
      53,     8,    79,  -129,  -129,  -129,  -129,     6,    80,    80,
    -129,  -129,  -129,  -129,    98,   121,   292,   261,  -129,   239,
     239,   239,   239,   239,   239,   239,   239,     2,   239,   239,
     239,  -129,    81,   100,  -129,  -129,    82,   -21,  -129,  -129,
    -129,  -129,  -129,     9,  -129,  -129,  -129,   381,   381,    60,
      60,    67,    67,    83,     6,  -129,   360,   -13,  -129,   269,
     360,   360,    89,    85,   239,    97,    92,   103,   106,   155,
    -129,  -129,   239,  -129,  -129,   223,  -129,   360,  -129,   239,
    -129,  -129,    42,  -129,  -129,   108,   239,   107,   223,   300,
     104,  -129,  -129,  -129,   189,   360,  -129,  -129,    27,   105,
    -129,   123,   322,   239,   109,   223,  -129,  -129,  -129,  -129,
    -129,  -129,   239,   223,   330,  -129,  -129,   352,  -129,   223,
     223,   133,  -129,   223,  -129
  };

  const unsigned char
  Cepsparser::yydefact_[] =
  {
       4,     0,     2,     1,    33,    34,     0,     0,     0,     0,
      83,     0,     0,    31,    30,    32,    29,     4,     0,     0,
       0,     6,    14,    23,     7,    19,    18,     0,     0,    65,
       0,    83,     0,    21,    20,    31,    32,    35,     0,     0,
      10,     9,    11,    12,    22,     8,     0,     0,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    66,    61,    60,     0,     0,    84,    73,
       4,    27,    25,     0,     5,    44,    45,    36,    43,    38,
      37,    39,    40,    41,    42,    47,    71,     0,    69,     0,
      17,    16,     0,    65,     0,     0,     0,     0,    72,     8,
      26,    13,     0,    46,    49,    50,    67,    62,     4,     0,
      15,    75,    74,    28,    70,     0,     0,     0,    50,     0,
       0,    51,    55,    56,     8,    63,    76,    79,     0,     0,
      82,     0,     0,     0,     0,    50,    48,    64,    77,    80,
      78,    81,     0,    50,     0,    57,    52,     0,    54,     0,
      50,    58,    53,     0,    59
  };

  const short int
  Cepsparser::yypgoto_[] =
  {
    -129,  -129,    -9,  -129,    71,  -129,  -129,  -129,  -129,  -129,
     110,    -2,  -111,  -128,  -129,    84,  -129,    -1,    57,  -129,
    -129,    54,  -129,  -129,  -129,   132
  };

  const signed char
  Cepsparser::yydefgoto_[] =
  {
      -1,     1,     2,    20,    43,    44,    21,    22,    39,   100,
      71,   119,   120,   121,   122,    66,    67,   123,    62,    63,
      87,    88,    97,    98,   112,    32
  };

  const short int
  Cepsparser::yytable_[] =
  {
      23,    24,   102,    64,    65,     4,     5,   134,    45,     8,
      37,    38,    25,    26,    95,     3,    46,    47,   -24,    12,
      31,   151,   103,    29,   146,   154,    35,    14,    36,    16,
     138,   139,   148,    40,    41,    42,    18,    85,    19,   152,
      57,    30,    58,    23,    24,   126,   127,    77,    78,    79,
      80,    81,    82,    83,    84,    86,    89,    90,    91,   128,
     129,    99,     4,     5,     6,     7,     8,    27,    28,   130,
       9,    48,    10,    11,    33,    34,    12,    64,    65,    53,
      54,    55,    56,    13,    14,    15,    16,    54,    55,    56,
      -8,    17,   107,    18,    57,    19,    58,    23,    24,   124,
      86,    57,    59,    58,    55,    56,    60,   125,   140,   141,
      61,    69,    70,    73,   132,    93,    92,    57,   111,    58,
      94,   105,    23,    24,     4,     5,     6,     7,     8,   108,
     109,   144,     9,   131,    10,    11,   110,   136,    12,   142,
     147,   133,   145,   153,   101,    13,    14,    15,    16,    72,
     106,    96,     0,    17,    74,    18,   114,    19,     4,     5,
       6,     7,     8,    68,     0,     0,     9,     0,    10,    11,
       0,     0,    12,     0,     0,     0,     0,     0,     0,    13,
      14,    15,    16,     0,     0,     0,     0,    17,   113,    18,
       0,    19,     4,     5,     6,     7,     8,     0,     0,     0,
       9,     0,    10,    11,     0,     0,    12,     0,     0,     0,
       0,     0,     0,    13,    14,    15,    16,     0,     0,     0,
       0,    17,   137,    18,     0,    19,     4,     5,     0,   115,
       8,   116,   117,     0,     9,     0,     0,     0,     0,     0,
      12,     0,     4,     5,     0,     0,     8,    35,    14,    36,
      16,     0,     0,     0,     0,   118,    12,    18,     0,    19,
       0,     0,     0,    35,    14,    36,    16,     0,     0,     0,
       0,     0,     0,    18,     0,    19,    49,    50,    51,    52,
      53,    54,    55,    56,    49,    50,    51,    52,    53,    54,
      55,    56,     0,     0,     0,    57,     0,    58,    76,     0,
       0,     0,     0,    57,     0,    58,   104,    49,    50,    51,
      52,    53,    54,    55,    56,    49,    50,    51,    52,    53,
      54,    55,    56,     0,     0,     0,    57,    75,    58,     0,
       0,   135,     0,     0,    57,     0,    58,    49,    50,    51,
      52,    53,    54,    55,    56,    49,    50,    51,    52,    53,
      54,    55,    56,   143,     0,     0,    57,     0,    58,     0,
       0,     0,     0,     0,    57,   149,    58,    49,    50,    51,
      52,    53,    54,    55,    56,    49,    50,    51,    52,    53,
      54,    55,    56,   150,     0,     0,    57,     0,    58,     0,
       0,     0,     0,     0,    57,     0,    58,    50,    51,    52,
      53,    54,    55,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    58
  };

  const short int
  Cepsparser::yycheck_[] =
  {
       2,     2,    15,    24,    25,     3,     4,   118,    17,     7,
      12,    25,    24,    25,    35,     0,    18,    19,    32,    17,
      12,   149,    35,    34,   135,   153,    24,    25,    26,    27,
       3,     4,   143,    24,    25,    26,    34,    35,    36,   150,
      34,    34,    36,    45,    45,     3,     4,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    17,
      18,    70,     3,     4,     5,     6,     7,    24,    25,    27,
      11,    31,    13,    14,    24,    25,    17,    24,    25,    19,
      20,    21,    22,    24,    25,    26,    27,    20,    21,    22,
      31,    32,    94,    34,    34,    36,    36,    99,    99,   108,
     102,    34,    16,    36,    21,    22,    16,   109,     3,     4,
      25,    32,    32,    15,   116,    15,    35,    34,    12,    36,
      38,    32,   124,   124,     3,     4,     5,     6,     7,    32,
      38,   133,    11,    25,    13,    14,    33,    33,    17,    16,
     142,    34,    33,    10,    73,    24,    25,    26,    27,    39,
      93,    67,    -1,    32,    33,    34,   102,    36,     3,     4,
       5,     6,     7,    31,    -1,    -1,    11,    -1,    13,    14,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    24,
      25,    26,    27,    -1,    -1,    -1,    -1,    32,    33,    34,
      -1,    36,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    -1,    13,    14,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    26,    27,    -1,    -1,    -1,
      -1,    32,    33,    34,    -1,    36,     3,     4,    -1,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      17,    -1,     3,     4,    -1,    -1,     7,    24,    25,    26,
      27,    -1,    -1,    -1,    -1,    32,    17,    34,    -1,    36,
      -1,    -1,    -1,    24,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    15,    16,    17,    18,
      19,    20,    21,    22,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    34,    -1,    36,    37,    -1,
      -1,    -1,    -1,    34,    -1,    36,    37,    15,    16,    17,
      18,    19,    20,    21,    22,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    34,    35,    36,    -1,
      -1,    31,    -1,    -1,    34,    -1,    36,    15,    16,    17,
      18,    19,    20,    21,    22,    15,    16,    17,    18,    19,
      20,    21,    22,    31,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    34,    35,    36,    15,    16,    17,
      18,    19,    20,    21,    22,    15,    16,    17,    18,    19,
      20,    21,    22,    31,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36
  };

  const unsigned char
  Cepsparser::yystos_[] =
  {
       0,    40,    41,     0,     3,     4,     5,     6,     7,    11,
      13,    14,    17,    24,    25,    26,    27,    32,    34,    36,
      42,    45,    46,    50,    56,    24,    25,    24,    25,    34,
      34,    12,    64,    24,    25,    24,    26,    50,    25,    47,
      24,    25,    26,    43,    44,    41,    50,    50,    31,    15,
      16,    17,    18,    19,    20,    21,    22,    34,    36,    16,
      16,    25,    57,    58,    24,    25,    54,    55,    64,    32,
      32,    49,    49,    15,    33,    35,    37,    50,    50,    50,
      50,    50,    50,    50,    50,    35,    50,    59,    60,    50,
      50,    50,    35,    15,    38,    35,    54,    61,    62,    41,
      48,    43,    15,    35,    37,    32,    57,    50,    32,    38,
      33,    12,    63,    33,    60,     6,     8,     9,    32,    50,
      51,    52,    53,    56,    41,    50,     3,     4,    17,    18,
      27,    25,    50,    34,    51,    31,    33,    33,     3,     4,
       3,     4,    16,    31,    50,    33,    51,    50,    51,    35,
      31,    52,    51,    10,    52
  };

  const unsigned char
  Cepsparser::yyr1_[] =
  {
       0,    39,    40,    41,    41,    42,    42,    42,    42,    43,
      43,    43,    44,    44,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    47,    48,    46,    46,    49,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      51,    51,    52,    52,    52,    52,    52,    52,    53,    53,
      54,    54,    55,    55,    56,    57,    57,    57,    58,    59,
      59,    60,    61,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    64,    64
  };

  const unsigned char
  Cepsparser::yyr2_[] =
  {
       0,     2,     1,     3,     0,     3,     1,     1,     0,     1,
       1,     1,     1,     3,     1,     5,     4,     4,     2,     2,
       2,     2,     2,     1,     0,     0,     4,     3,     3,     1,
       1,     1,     1,     1,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     3,     7,     4,
       0,     1,     3,     6,     4,     1,     1,     3,     5,     7,
       1,     1,     3,     4,     7,     0,     1,     3,     1,     1,
       3,     1,     1,     0,     2,     1,     2,     3,     3,     2,
       3,     3,     2,     0,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Cepsparser::yytname_[] =
  {
  "END", "error", "$undefined", "INTNUM", "FLOATNUM", "STRUCT", "VAL",
  "FUN", "RET", "IF", "ELSE", "FOR", "EOL", "RAWMAP", "KIND", "','", "'='",
  "'-'", "'+'", "'*'", "'/'", "'^'", "'.'", "NEG", "STRUCTID", "ID",
  "KINDID", "LITERAL", "METRIC_UNIT", "FUNCCALL", "INDEXOP", "';'", "'{'",
  "'}'", "'('", "')'", "'['", "']'", "':'", "$accept", "cepsscript",
  "stmts", "stmt", "general_id", "id_list", "decl", "struct_decl", "$@1",
  "$@2", "struct_initialization", "expr", "func_body", "func_stmts",
  "if_then_else", "id_or_struct_id", "for_loop_head", "for_loop",
  "parameter_list", "parameter", "argument_list", "argument", "raw_map",
  "raw_lines", "raw_line", "eols", YY_NULL
  };


  const unsigned short int
  Cepsparser::yyrline_[] =
  {
       0,   119,   119,   128,   136,   143,   147,   152,   158,   164,
     165,   166,   170,   174,   182,   186,   190,   195,   201,   207,
     212,   220,   228,   238,   245,   245,   245,   249,   256,   264,
     270,   275,   281,   286,   292,   298,   307,   312,   317,   323,
     328,   333,   338,   343,   347,   351,   356,   361,   367,   371,
     376,   378,   382,   383,   384,   385,   386,   387,   391,   392,
     396,   401,   409,   416,   427,   438,   440,   441,   445,   449,
     454,   464,   475,   483,   486,   495,   499,   504,   510,   518,
     523,   529,   537,   547,   549
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    19,    18,    15,    17,    22,    20,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,    31,
       2,    16,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    37,    21,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,    33,     2,     2,     2,     2,
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
      23,    24,    25,    26,    27,    28,    29,    30
    };
    const unsigned int user_token_number_max_ = 277;
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
#line 1596 "ceps.tab.c" // lalr1.cc:1156
#line 552 "../src/grammar/ceps.y" // lalr1.cc:1157


void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
