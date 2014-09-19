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
#line 123 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Root_ptr root_node = new ceps::ast::Root((yystack_[0].value.ast_node));
  driver.set_parsetree(ceps::ast::Parsetree{root_node});
  (yylhs.value.ast_node) = root_node;
 }
#line 561 "ceps.tab.c" // lalr1.cc:846
    break;

  case 3:
#line 132 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[1].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 571 "ceps.tab.c" // lalr1.cc:846
    break;

  case 4:
#line 139 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Stmts{}; //create empty Statements-Node
}
#line 579 "ceps.tab.c" // lalr1.cc:846
    break;

  case 5:
#line 146 "../src/grammar/ceps.y" // lalr1.cc:846
    { (yylhs.value.ast_node) = new ceps::ast::Scope{(yystack_[1].value.ast_node)};}
#line 585 "ceps.tab.c" // lalr1.cc:846
    break;

  case 6:
#line 151 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of decl
 }
#line 593 "ceps.tab.c" // lalr1.cc:846
    break;

  case 7:
#line 156 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of for_loop
 }
#line 601 "ceps.tab.c" // lalr1.cc:846
    break;

  case 8:
#line 161 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = nullptr;
 }
#line 609 "ceps.tab.c" // lalr1.cc:846
    break;

  case 12:
#line 174 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.str_list) = new std::vector<std::string> {*(yystack_[0].value.sval)};
 }
#line 617 "ceps.tab.c" // lalr1.cc:846
    break;

  case 13:
#line 178 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yystack_[2].value.str_list)->push_back(*(yystack_[0].value.sval));
   (yylhs.value.str_list) = (yystack_[2].value.str_list);
 }
#line 626 "ceps.tab.c" // lalr1.cc:846
    break;

  case 14:
#line 186 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result 
 }
#line 634 "ceps.tab.c" // lalr1.cc:846
    break;

  case 15:
#line 190 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result
}
#line 642 "ceps.tab.c" // lalr1.cc:846
    break;

  case 16:
#line 194 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 651 "ceps.tab.c" // lalr1.cc:846
    break;

  case 17:
#line 199 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 660 "ceps.tab.c" // lalr1.cc:846
    break;

  case 18:
#line 205 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
	delete (yystack_[0].value.sval); 
	(yylhs.value.ast_node)=nullptr;
}
#line 670 "ceps.tab.c" // lalr1.cc:846
    break;

  case 19:
#line 211 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	delete (yystack_[0].value.sval);
	(yylhs.value.ast_node)=nullptr;
}
#line 679 "ceps.tab.c" // lalr1.cc:846
    break;

  case 20:
#line 216 "../src/grammar/ceps.y" // lalr1.cc:846
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
#line 224 "../src/grammar/ceps.y" // lalr1.cc:846
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
#line 232 "../src/grammar/ceps.y" // lalr1.cc:846
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
#line 242 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 725 "ceps.tab.c" // lalr1.cc:846
    break;

  case 24:
#line 248 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().push_scope();}
#line 731 "ceps.tab.c" // lalr1.cc:846
    break;

  case 25:
#line 248 "../src/grammar/ceps.y" // lalr1.cc:846
    {driver.symboltable().pop_scope();}
#line 737 "ceps.tab.c" // lalr1.cc:846
    break;

  case 26:
#line 249 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[3].value.sval),(yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 745 "ceps.tab.c" // lalr1.cc:846
    break;

  case 27:
#line 253 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[2].value.sval),new ceps::ast::Identifier(*(yystack_[1].value.sval),nullptr,nullptr,nullptr),(yystack_[0].value.ast_node),nullptr);
}
#line 753 "ceps.tab.c" // lalr1.cc:846
    break;

  case 28:
#line 260 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 761 "ceps.tab.c" // lalr1.cc:846
    break;

  case 29:
#line 268 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
 	delete (yystack_[0].value.sval);
 }
#line 770 "ceps.tab.c" // lalr1.cc:846
    break;

  case 30:
#line 274 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
    delete (yystack_[0].value.sval);
  }
#line 779 "ceps.tab.c" // lalr1.cc:846
    break;

  case 31:
#line 280 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 788 "ceps.tab.c" // lalr1.cc:846
    break;

  case 32:
#line 285 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO:Handle kindid
}
#line 796 "ceps.tab.c" // lalr1.cc:846
    break;

  case 33:
#line 291 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}
#line 804 "ceps.tab.c" // lalr1.cc:846
    break;

  case 34:
#line 297 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}
#line 812 "ceps.tab.c" // lalr1.cc:846
    break;

  case 35:
#line 303 "../src/grammar/ceps.y" // lalr1.cc:846
    {  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('-',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 822 "ceps.tab.c" // lalr1.cc:846
    break;

  case 36:
#line 310 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 (yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::DOTDOT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 830 "ceps.tab.c" // lalr1.cc:846
    break;

  case 37:
#line 315 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(',',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 838 "ceps.tab.c" // lalr1.cc:846
    break;

  case 38:
#line 321 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('+',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 846 "ceps.tab.c" // lalr1.cc:846
    break;

  case 39:
#line 326 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('-',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 854 "ceps.tab.c" // lalr1.cc:846
    break;

  case 40:
#line 332 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('*',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);			
}
#line 862 "ceps.tab.c" // lalr1.cc:846
    break;

  case 41:
#line 337 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('/',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 870 "ceps.tab.c" // lalr1.cc:846
    break;

  case 42:
#line 342 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('^',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 878 "ceps.tab.c" // lalr1.cc:846
    break;

  case 43:
#line 346 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('.',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 886 "ceps.tab.c" // lalr1.cc:846
    break;

  case 44:
#line 351 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('=',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 894 "ceps.tab.c" // lalr1.cc:846
    break;

  case 45:
#line 355 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 902 "ceps.tab.c" // lalr1.cc:846
    break;

  case 46:
#line 359 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 910 "ceps.tab.c" // lalr1.cc:846
    break;

  case 47:
#line 364 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call{(yystack_[3].value.ast_node),(yystack_[1].value.ast_node)};
}
#line 919 "ceps.tab.c" // lalr1.cc:846
    break;

  case 48:
#line 369 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call((yystack_[2].value.ast_node),new  ceps::ast::Call_parameters(), nullptr);
}
#line 928 "ceps.tab.c" // lalr1.cc:846
    break;

  case 49:
#line 375 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 //TODO: Function Definitions
}
#line 936 "ceps.tab.c" // lalr1.cc:846
    break;

  case 61:
#line 404 "../src/grammar/ceps.y" // lalr1.cc:846
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
 }
#line 945 "ceps.tab.c" // lalr1.cc:846
    break;

  case 62:
#line 409 "../src/grammar/ceps.y" // lalr1.cc:846
    {
   (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 954 "ceps.tab.c" // lalr1.cc:846
    break;

  case 63:
#line 417 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 965 "ceps.tab.c" // lalr1.cc:846
    break;

  case 64:
#line 424 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr((yystack_[3].value.ast_node));
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 976 "ceps.tab.c" // lalr1.cc:846
    break;

  case 65:
#line 435 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back((yystack_[4].value.ast_node));
  temp->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = temp;
 }
#line 988 "ceps.tab.c" // lalr1.cc:846
    break;

  case 70:
#line 457 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = new ceps::ast::Call_parameters((yystack_[0].value.ast_node),nullptr,nullptr);
 }
#line 996 "ceps.tab.c" // lalr1.cc:846
    break;

  case 71:
#line 462 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
    
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 1007 "ceps.tab.c" // lalr1.cc:846
    break;

  case 72:
#line 472 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
 }
#line 1015 "ceps.tab.c" // lalr1.cc:846
    break;

  case 73:
#line 483 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node)=(yystack_[0].value.ast_node);
 }
#line 1023 "ceps.tab.c" // lalr1.cc:846
    break;

  case 74:
#line 490 "../src/grammar/ceps.y" // lalr1.cc:846
    {
 	(yylhs.value.ast_node) = new ceps::ast::Rawmap();
 }
#line 1031 "ceps.tab.c" // lalr1.cc:846
    break;

  case 75:
#line 494 "../src/grammar/ceps.y" // lalr1.cc:846
    { 
 	if (!ceps::ast::nlf_ptr((yystack_[0].value.ast_node))->empty()) 
 	 ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node)); 
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 1041 "ceps.tab.c" // lalr1.cc:846
    break;

  case 76:
#line 503 "../src/grammar/ceps.y" // lalr1.cc:846
    {
	(yylhs.value.ast_node) = new ceps::ast::Atoms();
}
#line 1049 "ceps.tab.c" // lalr1.cc:846
    break;

  case 77:
#line 507 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1058 "ceps.tab.c" // lalr1.cc:846
    break;

  case 78:
#line 512 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int(-1 * (yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1067 "ceps.tab.c" // lalr1.cc:846
    break;

  case 79:
#line 518 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1077 "ceps.tab.c" // lalr1.cc:846
    break;

  case 80:
#line 526 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1086 "ceps.tab.c" // lalr1.cc:846
    break;

  case 81:
#line 531 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double(-1.0*(yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1095 "ceps.tab.c" // lalr1.cc:846
    break;

  case 82:
#line 537 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1105 "ceps.tab.c" // lalr1.cc:846
    break;

  case 83:
#line 545 "../src/grammar/ceps.y" // lalr1.cc:846
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr));
  delete (yystack_[0].value.sval);
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1115 "ceps.tab.c" // lalr1.cc:846
    break;


#line 1119 "ceps.tab.c" // lalr1.cc:846
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


  const signed char Cepsparser::yypact_ninf_ = -86;

  const signed char Cepsparser::yytable_ninf_ = -25;

  const short int
  Cepsparser::yypact_[] =
  {
     -86,     9,     0,   -86,   -86,   -86,    35,    60,   -23,   -20,
      17,    65,   211,     8,   -86,    42,   -86,   -86,   211,   211,
     -11,   -86,   -86,   354,   -86,   -86,   -86,    14,    22,    19,
      67,    17,    39,   -86,   -86,   -86,   -86,     5,    49,    49,
     -86,   -86,   -86,   -86,    71,    70,   267,   234,   -86,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   198,   211,
     211,   211,   -86,    72,    90,   -86,   -86,    75,    -6,   -86,
     -86,   -86,   -86,   -86,    42,   -86,   -86,   -86,   384,   363,
     384,   373,   373,   117,   117,     1,     5,   -86,   354,   -14,
     -86,   243,   354,   354,    82,    19,   211,    84,    79,    85,
     108,   123,   -86,   -86,   211,   -86,   -86,   171,   -86,   354,
     -86,   211,   -86,   -86,    61,   -86,   -86,    95,   211,    87,
     171,   276,    89,   -86,   -86,   -86,   158,   354,   -86,   -86,
     107,   109,   -86,   114,   299,   211,    98,   171,   -86,   -86,
     -86,   -86,   -86,   -86,   211,   171,   322,   -86,   -86,   331,
     -86,   171,   171,   125,   -86,   171,   -86
  };

  const unsigned char
  Cepsparser::yydefact_[] =
  {
       4,     0,     2,     1,    33,    34,     0,     0,     0,     0,
      84,     0,     0,    31,    30,    32,    29,     4,     0,     0,
       0,     6,    14,    23,     7,    19,    18,     0,     0,    66,
       0,    84,     0,    21,    20,    31,    32,    35,     0,     0,
      10,     9,    11,    12,    22,     8,     0,     0,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,    67,    62,    61,     0,     0,    85,
      74,     4,    27,    25,     0,     5,    45,    46,    36,    37,
      44,    39,    38,    40,    41,    42,    43,    48,    72,     0,
      70,     0,    17,    16,     0,    66,     0,     0,     0,     0,
      73,     8,    26,    13,     0,    47,    50,    51,    68,    63,
       4,     0,    15,    76,    75,    28,    71,     0,     0,     0,
      51,     0,     0,    52,    56,    57,     8,    64,    77,    80,
       0,     0,    83,     0,     0,     0,     0,    51,    49,    65,
      78,    81,    79,    82,     0,    51,     0,    58,    53,     0,
      55,     0,    51,    59,    54,     0,    60
  };

  const signed char
  Cepsparser::yypgoto_[] =
  {
     -86,   -86,    -9,   -86,    69,   -86,   -86,   -86,   -86,   -86,
     105,    -2,   -74,   -85,   -86,    77,   -86,    -1,    51,   -86,
     -86,    43,   -86,   -86,   -86,   122
  };

  const signed char
  Cepsparser::yydefgoto_[] =
  {
      -1,     1,     2,    20,    43,    44,    21,    22,    39,   102,
      72,   121,   122,   123,   124,    67,    68,   125,    63,    64,
      89,    90,    99,   100,   114,    32
  };

  const short int
  Cepsparser::yytable_[] =
  {
      23,    24,   104,     4,     5,     6,     7,     8,    45,     3,
      37,     9,    29,    10,    11,    30,    46,    47,    12,    65,
      66,    48,   105,    56,    57,    13,    14,    15,    16,    31,
      97,    60,    -8,    17,    38,    18,    58,    19,    59,    61,
      58,   -24,    59,    23,    24,    62,   136,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    88,    91,    92,    93,
      25,    26,   101,   148,   128,   129,   153,    40,    41,    42,
     156,   150,    70,     4,     5,     6,     7,     8,   154,   130,
     131,     9,    71,    10,    11,    27,    28,    74,    12,   132,
      33,    34,    65,    66,   109,    13,    14,    15,    16,    23,
      24,   126,    88,    17,    75,    18,    95,    19,    94,   127,
     140,   141,   142,   143,    96,   107,   134,   110,   111,   112,
     113,   133,   135,   138,    23,    24,     4,     5,     6,     7,
       8,   144,   147,   146,     9,   155,    10,    11,    55,    56,
      57,    12,   149,   103,    73,    98,   108,   116,    13,    14,
      15,    16,    58,    69,    59,     0,    17,   115,    18,     0,
      19,     4,     5,     6,     7,     8,     0,     0,     0,     9,
       0,    10,    11,     0,     4,     5,    12,   117,     8,   118,
     119,     0,     9,    13,    14,    15,    16,     0,     0,    12,
       0,    17,   139,    18,     0,    19,    35,    14,    36,    16,
       0,     4,     5,     0,   120,     8,    18,     0,    19,     0,
       0,     0,     0,     0,     4,     5,    12,     0,     8,     0,
       0,     0,     0,    35,    14,    36,    16,     0,     0,    12,
       0,     0,     0,    18,    87,    19,    35,    14,    36,    16,
       0,     0,     0,     0,     0,     0,    18,     0,    19,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    49,    50,
      51,    52,    53,    54,    55,    56,    57,     0,     0,    58,
       0,    59,    77,     0,     0,     0,     0,     0,    58,     0,
      59,   106,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    49,    50,    51,    52,    53,    54,    55,    56,    57,
       0,     0,    58,    76,    59,     0,     0,     0,   137,     0,
       0,    58,     0,    59,    49,    50,    51,    52,    53,    54,
      55,    56,    57,     0,     0,     0,     0,     0,     0,     0,
       0,   145,     0,     0,    58,     0,    59,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,     0,    58,   151,    59,
       0,     0,     0,   152,     0,     0,    58,     0,    59,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    49,     0,
      51,    52,    53,    54,    55,    56,    57,     0,     0,    58,
       0,    59,     0,    54,    55,    56,    57,     0,    58,     0,
      59,    51,    52,    53,    54,    55,    56,    57,    58,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    59
  };

  const short int
  Cepsparser::yycheck_[] =
  {
       2,     2,    16,     3,     4,     5,     6,     7,    17,     0,
      12,    11,    35,    13,    14,    35,    18,    19,    18,    25,
      26,    32,    36,    22,    23,    25,    26,    27,    28,    12,
      36,    17,    32,    33,    26,    35,    35,    37,    37,    17,
      35,    33,    37,    45,    45,    26,   120,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      25,    26,    71,   137,     3,     4,   151,    25,    26,    27,
     155,   145,    33,     3,     4,     5,     6,     7,   152,    18,
      19,    11,    33,    13,    14,    25,    26,    16,    18,    28,
      25,    26,    25,    26,    96,    25,    26,    27,    28,   101,
     101,   110,   104,    33,    34,    35,    16,    37,    36,   111,
       3,     4,     3,     4,    39,    33,   118,    33,    39,    34,
      12,    26,    35,    34,   126,   126,     3,     4,     5,     6,
       7,    17,    34,   135,    11,    10,    13,    14,    21,    22,
      23,    18,   144,    74,    39,    68,    95,   104,    25,    26,
      27,    28,    35,    31,    37,    -1,    33,    34,    35,    -1,
      37,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      -1,    13,    14,    -1,     3,     4,    18,     6,     7,     8,
       9,    -1,    11,    25,    26,    27,    28,    -1,    -1,    18,
      -1,    33,    34,    35,    -1,    37,    25,    26,    27,    28,
      -1,     3,     4,    -1,    33,     7,    35,    -1,    37,    -1,
      -1,    -1,    -1,    -1,     3,     4,    18,    -1,     7,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    18,
      -1,    -1,    -1,    35,    36,    37,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    35,    36,    37,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    37,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    37,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,    35,    36,    37,
      -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    37,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    15,    -1,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    35,
      -1,    37,    -1,    20,    21,    22,    23,    -1,    35,    -1,
      37,    17,    18,    19,    20,    21,    22,    23,    35,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37
  };

  const unsigned char
  Cepsparser::yystos_[] =
  {
       0,    41,    42,     0,     3,     4,     5,     6,     7,    11,
      13,    14,    18,    25,    26,    27,    28,    33,    35,    37,
      43,    46,    47,    51,    57,    25,    26,    25,    26,    35,
      35,    12,    65,    25,    26,    25,    27,    51,    26,    48,
      25,    26,    27,    44,    45,    42,    51,    51,    32,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    35,    37,
      17,    17,    26,    58,    59,    25,    26,    55,    56,    65,
      33,    33,    50,    50,    16,    34,    36,    38,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    36,    51,    60,
      61,    51,    51,    51,    36,    16,    39,    36,    55,    62,
      63,    42,    49,    44,    16,    36,    38,    33,    58,    51,
      33,    39,    34,    12,    64,    34,    61,     6,     8,     9,
      33,    51,    52,    53,    54,    57,    42,    51,     3,     4,
      18,    19,    28,    26,    51,    35,    52,    32,    34,    34,
       3,     4,     3,     4,    17,    32,    51,    34,    52,    51,
      52,    36,    32,    53,    52,    10,    53
  };

  const unsigned char
  Cepsparser::yyr1_[] =
  {
       0,    40,    41,    42,    42,    43,    43,    43,    43,    44,
      44,    44,    45,    45,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    48,    49,    47,    47,    50,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    52,    52,    53,    53,    53,    53,    53,    53,    54,
      54,    55,    55,    56,    56,    57,    58,    58,    58,    59,
      60,    60,    61,    62,    63,    63,    64,    64,    64,    64,
      64,    64,    64,    64,    65,    65
  };

  const unsigned char
  Cepsparser::yyr2_[] =
  {
       0,     2,     1,     3,     0,     3,     1,     1,     0,     1,
       1,     1,     1,     3,     1,     5,     4,     4,     2,     2,
       2,     2,     2,     1,     0,     0,     4,     3,     3,     1,
       1,     1,     1,     1,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     3,     7,
       4,     0,     1,     3,     6,     4,     1,     1,     3,     5,
       7,     1,     1,     3,     4,     7,     0,     1,     3,     1,
       1,     3,     1,     1,     0,     2,     1,     2,     3,     3,
       2,     3,     3,     2,     0,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Cepsparser::yytname_[] =
  {
  "END", "error", "$undefined", "INTNUM", "FLOATNUM", "STRUCT", "VAL",
  "FUN", "RET", "IF", "ELSE", "FOR", "EOL", "RAWMAP", "KIND", "DOTDOT",
  "','", "'='", "'-'", "'+'", "'*'", "'/'", "'^'", "'.'", "NEG",
  "STRUCTID", "ID", "KINDID", "LITERAL", "METRIC_UNIT", "FUNCCALL",
  "INDEXOP", "';'", "'{'", "'}'", "'('", "')'", "'['", "']'", "':'",
  "$accept", "cepsscript", "stmts", "stmt", "general_id", "id_list",
  "decl", "struct_decl", "$@1", "$@2", "struct_initialization", "expr",
  "func_body", "func_stmts", "if_then_else", "id_or_struct_id",
  "for_loop_head", "for_loop", "parameter_list", "parameter",
  "argument_list", "argument", "raw_map", "raw_lines", "raw_line", "eols", YY_NULL
  };


  const unsigned short int
  Cepsparser::yyrline_[] =
  {
       0,   122,   122,   131,   139,   146,   150,   155,   161,   167,
     168,   169,   173,   177,   185,   189,   193,   198,   204,   210,
     215,   223,   231,   241,   248,   248,   248,   252,   259,   267,
     273,   278,   284,   289,   295,   301,   309,   314,   319,   324,
     330,   335,   340,   345,   350,   354,   358,   363,   368,   374,
     378,   383,   385,   389,   390,   391,   392,   393,   394,   398,
     399,   403,   408,   416,   423,   434,   445,   447,   448,   452,
     456,   461,   471,   482,   490,   493,   502,   506,   511,   517,
     525,   530,   536,   544,   554,   556
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
      35,    36,    20,    19,    16,    18,    23,    21,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    32,
       2,    17,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,    22,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,     2,    34,     2,     2,     2,     2,
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
      15,    24,    25,    26,    27,    28,    29,    30,    31
    };
    const unsigned int user_token_number_max_ = 278;
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
#line 1606 "ceps.tab.c" // lalr1.cc:1156
#line 559 "../src/grammar/ceps.y" // lalr1.cc:1157


void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
