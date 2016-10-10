// A Bison parser, made by GNU Bison 3.0.2.

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

#line 37 "ceps.tab.c" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "ceps.tab.h"

// User implementation prologue.

#line 51 "ceps.tab.c" // lalr1.cc:407
// Unqualified %code blocks.
#line 46 "../src/grammar/ceps.y" // lalr1.cc:408

#include "cepsparserdriver.hh"
#include "cepslexer.hh"
#include "ceps_ast.hh"
#include <valarray>

#line 60 "ceps.tab.c" // lalr1.cc:408


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

#line 31 "../src/grammar/ceps.y" // lalr1.cc:474
namespace ceps {
#line 146 "ceps.tab.c" // lalr1.cc:474

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
  Cepsparser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
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
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

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
    yypush_ (YY_NULLPTR, 0, yyla);

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
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
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
#line 142 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::Root_ptr root_node = new ceps::ast::Root((yystack_[0].value.ast_node));
  driver.set_parsetree(ceps::ast::Parsetree{root_node});
  (yylhs.value.ast_node) = root_node;
 }
#line 561 "ceps.tab.c" // lalr1.cc:847
    break;

  case 3:
#line 151 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 571 "ceps.tab.c" // lalr1.cc:847
    break;

  case 4:
#line 158 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Stmts{}; //create empty Statements-Node
}
#line 579 "ceps.tab.c" // lalr1.cc:847
    break;

  case 5:
#line 165 "../src/grammar/ceps.y" // lalr1.cc:847
    { (yylhs.value.ast_node) = new ceps::ast::Scope{(yystack_[1].value.ast_node)};}
#line 585 "ceps.tab.c" // lalr1.cc:847
    break;

  case 6:
#line 167 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 (yylhs.value.ast_node) = new ceps::ast::Return((yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 593 "ceps.tab.c" // lalr1.cc:847
    break;

  case 7:
#line 171 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
}
#line 601 "ceps.tab.c" // lalr1.cc:847
    break;

  case 8:
#line 177 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result of decl
 }
#line 609 "ceps.tab.c" // lalr1.cc:847
    break;

  case 9:
#line 182 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of for_loop
 }
#line 617 "ceps.tab.c" // lalr1.cc:847
    break;

  case 10:
#line 188 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 625 "ceps.tab.c" // lalr1.cc:847
    break;

  case 11:
#line 193 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 633 "ceps.tab.c" // lalr1.cc:847
    break;

  case 12:
#line 197 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node) = nullptr;
 }
#line 641 "ceps.tab.c" // lalr1.cc:847
    break;

  case 13:
#line 204 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 (yylhs.value.ast_node) = new ceps::ast::Ifelse((yystack_[2].value.ast_node),(yystack_[0].value.ast_node));
}
#line 649 "ceps.tab.c" // lalr1.cc:847
    break;

  case 14:
#line 208 "../src/grammar/ceps.y" // lalr1.cc:847
    {
(yylhs.value.ast_node) = new ceps::ast::Ifelse((yystack_[4].value.ast_node),(yystack_[2].value.ast_node),(yystack_[0].value.ast_node));
}
#line 657 "ceps.tab.c" // lalr1.cc:847
    break;

  case 18:
#line 222 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.str_list) = new std::vector<std::string> {*(yystack_[0].value.sval)};
 }
#line 665 "ceps.tab.c" // lalr1.cc:847
    break;

  case 19:
#line 226 "../src/grammar/ceps.y" // lalr1.cc:847
    {
   (yystack_[2].value.str_list)->push_back(*(yystack_[0].value.sval));
   (yylhs.value.str_list) = (yystack_[2].value.str_list);
 }
#line 674 "ceps.tab.c" // lalr1.cc:847
    break;

  case 20:
#line 234 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result 
 }
#line 682 "ceps.tab.c" // lalr1.cc:847
    break;

  case 21:
#line 238 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result
}
#line 690 "ceps.tab.c" // lalr1.cc:847
    break;

  case 22:
#line 242 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 699 "ceps.tab.c" // lalr1.cc:847
    break;

  case 23:
#line 247 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 708 "ceps.tab.c" // lalr1.cc:847
    break;

  case 24:
#line 253 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
	delete (yystack_[0].value.sval); 
	(yylhs.value.ast_node)=nullptr;
}
#line 718 "ceps.tab.c" // lalr1.cc:847
    break;

  case 25:
#line 259 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	delete (yystack_[0].value.sval);
	(yylhs.value.ast_node)=nullptr;
}
#line 727 "ceps.tab.c" // lalr1.cc:847
    break;

  case 26:
#line 264 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 739 "ceps.tab.c" // lalr1.cc:847
    break;

  case 27:
#line 272 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 751 "ceps.tab.c" // lalr1.cc:847
    break;

  case 28:
#line 280 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 763 "ceps.tab.c" // lalr1.cc:847
    break;

  case 29:
#line 288 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 auto r = new ceps::ast::Kinddef(*(yystack_[1].value.sval),nullptr,nullptr,nullptr);
 for (auto s : *(yystack_[0].value.str_list))
 {
  r->children().push_back(new ceps::ast::Identifier(s,nullptr,nullptr,nullptr)); 
 }
 delete (yystack_[0].value.str_list); 
 (yylhs.value.ast_node) = r;
}
#line 777 "ceps.tab.c" // lalr1.cc:847
    break;

  case 30:
#line 298 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 785 "ceps.tab.c" // lalr1.cc:847
    break;

  case 31:
#line 302 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 793 "ceps.tab.c" // lalr1.cc:847
    break;

  case 32:
#line 308 "../src/grammar/ceps.y" // lalr1.cc:847
    {driver.symboltable().push_scope();}
#line 799 "ceps.tab.c" // lalr1.cc:847
    break;

  case 33:
#line 308 "../src/grammar/ceps.y" // lalr1.cc:847
    {driver.symboltable().pop_scope();}
#line 805 "ceps.tab.c" // lalr1.cc:847
    break;

  case 34:
#line 309 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[3].value.sval),(yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 813 "ceps.tab.c" // lalr1.cc:847
    break;

  case 35:
#line 313 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[2].value.sval),new ceps::ast::Identifier(*(yystack_[1].value.sval),nullptr,nullptr,nullptr),(yystack_[0].value.ast_node),nullptr);
}
#line 821 "ceps.tab.c" // lalr1.cc:847
    break;

  case 36:
#line 320 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 829 "ceps.tab.c" // lalr1.cc:847
    break;

  case 37:
#line 328 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node) = new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
 	delete (yystack_[0].value.sval);
 }
#line 838 "ceps.tab.c" // lalr1.cc:847
    break;

  case 38:
#line 334 "../src/grammar/ceps.y" // lalr1.cc:847
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
    delete (yystack_[0].value.sval);
  }
#line 847 "ceps.tab.c" // lalr1.cc:847
    break;

  case 39:
#line 340 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 856 "ceps.tab.c" // lalr1.cc:847
    break;

  case 40:
#line 345 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 //TODO:Handle kindid
}
#line 864 "ceps.tab.c" // lalr1.cc:847
    break;

  case 41:
#line 351 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}
#line 872 "ceps.tab.c" // lalr1.cc:847
    break;

  case 42:
#line 357 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}
#line 880 "ceps.tab.c" // lalr1.cc:847
    break;

  case 43:
#line 363 "../src/grammar/ceps.y" // lalr1.cc:847
    {  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('-',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 890 "ceps.tab.c" // lalr1.cc:847
    break;

  case 44:
#line 370 "../src/grammar/ceps.y" // lalr1.cc:847
    {  
	
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('!',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 899 "ceps.tab.c" // lalr1.cc:847
    break;

  case 45:
#line 376 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 (yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::DOTDOT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 907 "ceps.tab.c" // lalr1.cc:847
    break;

  case 46:
#line 381 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(',',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 915 "ceps.tab.c" // lalr1.cc:847
    break;

  case 47:
#line 386 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('|',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 923 "ceps.tab.c" // lalr1.cc:847
    break;

  case 48:
#line 390 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('&',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 931 "ceps.tab.c" // lalr1.cc:847
    break;

  case 49:
#line 396 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('+',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 939 "ceps.tab.c" // lalr1.cc:847
    break;

  case 50:
#line 401 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('-',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 947 "ceps.tab.c" // lalr1.cc:847
    break;

  case 51:
#line 407 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('*',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);			
}
#line 955 "ceps.tab.c" // lalr1.cc:847
    break;

  case 52:
#line 412 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('/',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 963 "ceps.tab.c" // lalr1.cc:847
    break;

  case 53:
#line 417 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('^',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 971 "ceps.tab.c" // lalr1.cc:847
    break;

  case 54:
#line 421 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('.',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 979 "ceps.tab.c" // lalr1.cc:847
    break;

  case 55:
#line 426 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('=',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 987 "ceps.tab.c" // lalr1.cc:847
    break;

  case 56:
#line 430 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 995 "ceps.tab.c" // lalr1.cc:847
    break;

  case 57:
#line 434 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1003 "ceps.tab.c" // lalr1.cc:847
    break;

  case 58:
#line 438 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1011 "ceps.tab.c" // lalr1.cc:847
    break;

  case 59:
#line 442 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1019 "ceps.tab.c" // lalr1.cc:847
    break;

  case 60:
#line 446 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1027 "ceps.tab.c" // lalr1.cc:847
    break;

  case 61:
#line 450 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_NEQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1035 "ceps.tab.c" // lalr1.cc:847
    break;

  case 62:
#line 454 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1043 "ceps.tab.c" // lalr1.cc:847
    break;

  case 63:
#line 458 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1051 "ceps.tab.c" // lalr1.cc:847
    break;

  case 64:
#line 463 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call{(yystack_[3].value.ast_node),(yystack_[1].value.ast_node)};
}
#line 1060 "ceps.tab.c" // lalr1.cc:847
    break;

  case 65:
#line 468 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call((yystack_[2].value.ast_node),new  ceps::ast::Call_parameters(), nullptr);
}
#line 1069 "ceps.tab.c" // lalr1.cc:847
    break;

  case 66:
#line 474 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 //TODO: Function Definitions
}
#line 1077 "ceps.tab.c" // lalr1.cc:847
    break;

  case 78:
#line 504 "../src/grammar/ceps.y" // lalr1.cc:847
    {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
 }
#line 1086 "ceps.tab.c" // lalr1.cc:847
    break;

  case 79:
#line 509 "../src/grammar/ceps.y" // lalr1.cc:847
    {
   (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 1095 "ceps.tab.c" // lalr1.cc:847
    break;

  case 80:
#line 517 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1106 "ceps.tab.c" // lalr1.cc:847
    break;

  case 81:
#line 524 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr((yystack_[3].value.ast_node));
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1117 "ceps.tab.c" // lalr1.cc:847
    break;

  case 82:
#line 535 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back((yystack_[4].value.ast_node));
  temp->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = temp;
 }
#line 1129 "ceps.tab.c" // lalr1.cc:847
    break;

  case 83:
#line 546 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  auto temp = new ceps::ast::Template_defintion(*(yystack_[6].value.sval),*(yystack_[4].value.str_list),nullptr,nullptr,nullptr);
  
}
#line 1138 "ceps.tab.c" // lalr1.cc:847
    break;

  case 84:
#line 552 "../src/grammar/ceps.y" // lalr1.cc:847
    {
}
#line 1145 "ceps.tab.c" // lalr1.cc:847
    break;

  case 85:
#line 556 "../src/grammar/ceps.y" // lalr1.cc:847
    {
}
#line 1152 "ceps.tab.c" // lalr1.cc:847
    break;

  case 86:
#line 562 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  auto symbol = driver.symboltable().lookup(*(yystack_[3].value.sval),true,true,false);
  symbol->category = ceps::parser_env::Symbol::MACRO;
  symbol->payload = (yystack_[1].value.ast_node);
  delete (yystack_[3].value.sval);
  (yylhs.value.ast_node) = nullptr; 	  
 }
#line 1164 "ceps.tab.c" // lalr1.cc:847
    break;

  case 91:
#line 584 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = new ceps::ast::Call_parameters((yystack_[0].value.ast_node),nullptr,nullptr);
 }
#line 1172 "ceps.tab.c" // lalr1.cc:847
    break;

  case 92:
#line 589 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
    
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 1183 "ceps.tab.c" // lalr1.cc:847
    break;

  case 93:
#line 599 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
 }
#line 1191 "ceps.tab.c" // lalr1.cc:847
    break;

  case 94:
#line 610 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node)=(yystack_[0].value.ast_node);
 }
#line 1199 "ceps.tab.c" // lalr1.cc:847
    break;

  case 95:
#line 617 "../src/grammar/ceps.y" // lalr1.cc:847
    {
 	(yylhs.value.ast_node) = new ceps::ast::Rawmap();
 }
#line 1207 "ceps.tab.c" // lalr1.cc:847
    break;

  case 96:
#line 621 "../src/grammar/ceps.y" // lalr1.cc:847
    { 
 	if (!ceps::ast::nlf_ptr((yystack_[0].value.ast_node))->empty()) 
 	 ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node)); 
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 1217 "ceps.tab.c" // lalr1.cc:847
    break;

  case 97:
#line 630 "../src/grammar/ceps.y" // lalr1.cc:847
    {
	(yylhs.value.ast_node) = new ceps::ast::Atoms();
}
#line 1225 "ceps.tab.c" // lalr1.cc:847
    break;

  case 98:
#line 634 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1234 "ceps.tab.c" // lalr1.cc:847
    break;

  case 99:
#line 639 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int(-1 * (yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1243 "ceps.tab.c" // lalr1.cc:847
    break;

  case 100:
#line 645 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1253 "ceps.tab.c" // lalr1.cc:847
    break;

  case 101:
#line 653 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1262 "ceps.tab.c" // lalr1.cc:847
    break;

  case 102:
#line 658 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double(-1.0*(yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1271 "ceps.tab.c" // lalr1.cc:847
    break;

  case 103:
#line 664 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1281 "ceps.tab.c" // lalr1.cc:847
    break;

  case 104:
#line 672 "../src/grammar/ceps.y" // lalr1.cc:847
    {
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr));
  delete (yystack_[0].value.sval);
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1291 "ceps.tab.c" // lalr1.cc:847
    break;


#line 1295 "ceps.tab.c" // lalr1.cc:847
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
      yypush_ (YY_NULLPTR, yylhs);
    }
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
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
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


  const short int Cepsparser::yypact_ninf_ = -185;

  const signed char Cepsparser::yytable_ninf_ = -33;

  const short int
  Cepsparser::yypact_[] =
  {
    -185,    26,   545,  -185,  -185,  -185,    46,    83,    -4,   187,
       1,     7,    16,    51,    59,    59,   187,   -27,  -185,   119,
    -185,  -185,  -185,   187,   187,   187,  -185,  -185,    -8,  -185,
     879,  -185,    29,    45,  -185,  -185,    74,    91,    79,  -185,
    -185,   647,   187,   129,    16,    89,  -185,  -185,  -185,  -185,
    -185,  -185,    38,    95,   -21,   100,   100,    99,  -185,   107,
     202,   681,   -21,   579,  -185,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   135,   187,  -185,  -185,   187,   187,  -185,    80,
     116,  -185,   713,  -185,  -185,   110,   -15,  -185,  -185,  -185,
     -18,  -185,  -185,  -185,  -185,   187,    59,  -185,  -185,  -185,
     941,   910,   941,   970,   982,  1009,    75,  1018,  1042,   121,
    1048,     3,     3,   113,   113,    82,   -21,  -185,   879,    -3,
    -185,   613,   879,   879,   134,    79,   545,   187,   136,   142,
     141,   176,   251,   144,    -1,   300,   349,  -185,   879,  -185,
     187,  -185,  -185,     2,  -185,   186,   879,  -185,   187,  -185,
    -185,    84,  -185,  -185,   152,  -185,  -185,  -185,   159,   187,
     151,     2,   745,   156,  -185,  -185,  -185,   545,   398,   879,
    -185,  -185,   167,   174,  -185,   447,  -185,   182,   779,   187,
     166,     2,  -185,  -185,  -185,  -185,  -185,  -185,  -185,  -185,
     496,   187,     2,   813,  -185,  -185,  -185,   845,  -185,     2,
       2,   204,  -185,     2,  -185
  };

  const unsigned char
  Cepsparser::yydefact_[] =
  {
       4,     0,     2,     1,    41,    42,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,    39,    38,    40,
      37,     4,    12,     0,     0,     0,     3,     7,     0,    20,
      31,     9,     0,     0,    25,    24,     0,     0,    87,    39,
      40,     0,     0,     0,   105,     0,    27,    26,    28,    16,
      15,    17,     0,     0,    43,     0,     0,    15,    18,    29,
       0,     0,    44,     0,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,     0,     0,    90,     0,
      88,     6,     0,    79,    78,     0,     0,   106,    95,     4,
       0,     4,     4,    35,    33,     0,     0,     5,    62,    63,
      45,    46,    55,    47,    48,    58,    57,    56,    61,    59,
      60,    50,    49,    51,    52,    53,    54,    65,    93,     0,
      91,     0,    23,    22,     0,    87,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,    34,    30,    19,
       0,    64,    67,    68,    89,    13,    80,     4,     0,    21,
      97,    96,    84,     4,     0,    86,    36,    92,     0,     0,
       0,    68,     0,     0,    69,    73,    74,     0,     0,    81,
      98,   101,     0,     0,   104,     0,     4,     0,     0,     0,
       0,    68,    66,    14,    82,    99,   102,   100,   103,    85,
       0,     0,    68,     0,    75,    70,    83,     0,    72,     0,
      68,    76,    71,     0,    77
  };

  const short int
  Cepsparser::yypgoto_[] =
  {
    -185,  -185,   -20,  -132,  -185,   -12,   120,  -185,  -185,  -185,
    -185,   163,    -9,   -91,  -184,  -185,   126,  -185,   -78,  -185,
    -185,    88,  -185,  -185,    81,  -185,  -185,  -185,   180
  };

  const short int
  Cepsparser::yydefgoto_[] =
  {
      -1,     1,     2,    26,    27,    58,    59,    28,    29,    56,
     147,   103,    30,   173,   174,   175,    95,    96,    31,    32,
      33,    89,    90,   129,   130,   140,   141,   161,    45
  };

  const short int
  Cepsparser::yytable_[] =
  {
      41,    60,    52,    53,   155,     4,     5,    54,   168,     8,
     169,   170,    55,    11,    61,    62,    63,   150,   -32,   106,
      49,    50,    51,    93,    94,   211,     3,    82,    44,   214,
      83,   143,    16,    92,   138,    78,    79,    80,    81,    64,
      39,    18,    40,    20,    38,   193,   151,   171,   164,    42,
      23,    82,    24,    25,    83,    43,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   128,   131,   176,    84,   132,   133,   142,
     190,   145,   146,    99,    34,    35,   100,   180,   181,    46,
      47,    48,    85,   176,   149,    86,   148,    49,    50,    51,
     205,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,   208,    87,   176,   182,   183,    80,    81,    88,   212,
     105,    36,    37,    82,   176,   184,    83,   106,   156,   134,
      82,   176,   176,    83,    98,   176,   135,   178,     4,     5,
     101,   128,     8,   185,   172,   102,    79,    80,    81,   179,
      75,    76,    77,    78,    79,    80,    81,    49,    57,    51,
     188,    82,   172,   137,    83,    16,   200,    93,    94,    82,
     195,   196,    83,    39,    18,    40,    20,   197,   198,   153,
     203,   157,   172,    23,   127,    24,    25,   159,   160,   163,
       4,     5,   207,   172,     8,   158,   177,   186,   187,   189,
     172,   172,   192,   201,   172,     4,     5,     6,     7,     8,
       9,    10,   204,    11,   213,    12,    13,    16,    14,   104,
     144,    15,   139,   154,    97,    39,    18,    40,    20,     0,
       0,   167,    16,     0,     0,    23,     0,    24,    25,     0,
      17,    18,    19,    20,     0,     0,     0,    21,   107,    22,
      23,     0,    24,    25,     4,     5,     6,     7,     8,     9,
      10,     0,    11,     0,    12,    13,     0,    14,     0,     0,
      15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,     0,    17,
      18,    19,    20,     0,     0,     0,    21,   162,    22,    23,
       0,    24,    25,     4,     5,     6,     7,     8,     9,    10,
       0,    11,     0,    12,    13,     0,    14,     0,     0,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,    17,    18,
      19,    20,     0,     0,     0,    21,   165,    22,    23,     0,
      24,    25,     4,     5,     6,     7,     8,     9,    10,     0,
      11,     0,    12,    13,     0,    14,     0,     0,    15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,    17,    18,    19,
      20,     0,     0,     0,    21,   166,    22,    23,     0,    24,
      25,     4,     5,     6,     7,     8,     9,    10,     0,    11,
       0,    12,    13,     0,    14,     0,     0,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,    17,    18,    19,    20,
       0,     0,     0,    21,   194,    22,    23,     0,    24,    25,
       4,     5,     6,     7,     8,     9,    10,     0,    11,     0,
      12,    13,     0,    14,     0,     0,    15,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     0,     0,     0,    17,    18,    19,    20,     0,
       0,     0,    21,   199,    22,    23,     0,    24,    25,     4,
       5,     6,     7,     8,     9,    10,     0,    11,     0,    12,
      13,     0,    14,     0,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,    17,    18,    19,    20,     0,     0,
       0,    21,   206,    22,    23,     0,    24,    25,     4,     5,
       6,     7,     8,     9,    10,     0,    11,     0,    12,    13,
       0,    14,     0,     0,    15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,     0,
       0,     0,     0,    17,    18,    19,    20,     0,     0,     0,
      21,     0,    22,    23,    65,    24,    25,     0,     0,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,    65,     0,
      83,   109,     0,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,    65,     0,    83,   152,     0,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    82,    65,     0,    83,     0,
       0,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,    82,
     108,     0,    83,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    82,   136,     0,    83,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   191,    82,    65,     0,    83,     0,     0,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   202,    82,    65,     0,
      83,     0,     0,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,    82,   209,     0,    83,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   210,    82,    65,     0,    83,     0,     0,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    82,     0,     0,
      83,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,    83,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,    83,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    83,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     0,     0,     0,    82,     0,     0,
      83,     0,     0,     0,     0,     0,    82,     0,     0,    83,
      74,    75,    76,    77,    78,    79,    80,    81,    76,    77,
      78,    79,    80,    81,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    83,     0,     0,    82,     0,     0,    83
  };

  const short int
  Cepsparser::yycheck_[] =
  {
       9,    21,    14,    15,   136,     3,     4,    16,     6,     7,
       8,     9,    39,    11,    23,    24,    25,    20,    45,    20,
      38,    39,    40,    38,    39,   209,     0,    48,    12,   213,
      51,    49,    30,    42,    49,    32,    33,    34,    35,    47,
      38,    39,    40,    41,    48,   177,    49,    45,    49,    48,
      48,    48,    50,    51,    51,    48,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,   153,    47,    86,    87,    99,
     171,   101,   102,    45,    38,    39,    48,     3,     4,    38,
      39,    40,    47,   171,   106,    21,   105,    38,    39,    40,
     191,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,   202,    21,   191,    30,    31,    34,    35,    39,   210,
      21,    38,    39,    48,   202,    41,    51,    20,   137,    49,
      48,   209,   210,    51,    45,   213,    20,   157,     3,     4,
      45,   150,     7,   163,   153,    45,    33,    34,    35,   158,
      29,    30,    31,    32,    33,    34,    35,    38,    39,    40,
     169,    48,   171,    53,    51,    30,   186,    38,    39,    48,
       3,     4,    51,    38,    39,    40,    41,     3,     4,    45,
     189,    45,   191,    48,    49,    50,    51,    46,    12,    45,
       3,     4,   201,   202,     7,    53,    10,    45,    39,    48,
     209,   210,    46,    21,   213,     3,     4,     5,     6,     7,
       8,     9,    46,    11,    10,    13,    14,    30,    16,    56,
     100,    19,    96,   135,    44,    38,    39,    40,    41,    -1,
      -1,   150,    30,    -1,    -1,    48,    -1,    50,    51,    -1,
      38,    39,    40,    41,    -1,    -1,    -1,    45,    46,    47,
      48,    -1,    50,    51,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    -1,    13,    14,    -1,    16,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    41,    -1,    -1,    -1,    45,    46,    47,    48,
      -1,    50,    51,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    13,    14,    -1,    16,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,
      40,    41,    -1,    -1,    -1,    45,    46,    47,    48,    -1,
      50,    51,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    13,    14,    -1,    16,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      41,    -1,    -1,    -1,    45,    46,    47,    48,    -1,    50,
      51,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    13,    14,    -1,    16,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,
      -1,    -1,    -1,    45,    46,    47,    48,    -1,    50,    51,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      13,    14,    -1,    16,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    41,    -1,
      -1,    -1,    45,    46,    47,    48,    -1,    50,    51,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    13,
      14,    -1,    16,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    41,    -1,    -1,
      -1,    45,    46,    47,    48,    -1,    50,    51,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    13,    14,
      -1,    16,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    40,    41,    -1,    -1,    -1,
      45,    -1,    47,    48,    15,    50,    51,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    15,    -1,
      51,    52,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    15,    -1,    51,    52,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    15,    -1,    51,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    48,
      49,    -1,    51,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    48,    49,    -1,    51,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    48,    15,    -1,    51,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    15,    -1,
      51,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    48,    49,    -1,    51,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    48,    15,    -1,    51,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    48,    -1,    -1,
      51,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    51,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    51,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    48,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    51,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,
      28,    29,    30,    31,    32,    33,    34,    35,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    48,    -1,    -1,    51
  };

  const unsigned char
  Cepsparser::yystos_[] =
  {
       0,    55,    56,     0,     3,     4,     5,     6,     7,     8,
       9,    11,    13,    14,    16,    19,    30,    38,    39,    40,
      41,    45,    47,    48,    50,    51,    57,    58,    61,    62,
      66,    72,    73,    74,    38,    39,    38,    39,    48,    38,
      40,    66,    48,    48,    12,    82,    38,    39,    40,    38,
      39,    40,    59,    59,    66,    39,    63,    39,    59,    60,
      56,    66,    66,    66,    47,    15,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    48,    51,    47,    47,    21,    21,    39,    75,
      76,    47,    66,    38,    39,    70,    71,    82,    45,    45,
      48,    45,    45,    65,    65,    21,    20,    46,    49,    52,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    49,    66,    77,
      78,    66,    66,    66,    49,    20,    49,    53,    49,    70,
      79,    80,    56,    49,    60,    56,    56,    64,    66,    59,
      20,    49,    52,    45,    75,    57,    66,    45,    53,    46,
      12,    81,    46,    45,    49,    46,    46,    78,     6,     8,
       9,    45,    66,    67,    68,    69,    72,    10,    56,    66,
       3,     4,    30,    31,    41,    56,    45,    39,    66,    48,
      67,    47,    46,    57,    46,     3,     4,     3,     4,    46,
      56,    21,    47,    66,    46,    67,    46,    66,    67,    49,
      47,    68,    67,    10,    68
  };

  const unsigned char
  Cepsparser::yyr1_[] =
  {
       0,    54,    55,    56,    56,    57,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    59,    59,    59,    60,    60,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    63,    64,    62,    62,    65,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    67,    67,
      68,    68,    68,    68,    68,    68,    69,    69,    70,    70,
      71,    71,    72,    73,    73,    73,    74,    75,    75,    75,
      76,    77,    77,    78,    79,    80,    80,    81,    81,    81,
      81,    81,    81,    81,    81,    82,    82
  };

  const unsigned char
  Cepsparser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     3,     3,     1,     2,     1,
       2,     2,     1,     5,     7,     1,     1,     1,     1,     3,
       1,     5,     4,     4,     2,     2,     2,     2,     2,     2,
       4,     1,     0,     0,     4,     3,     3,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     3,     7,     4,     0,     1,
       3,     6,     4,     1,     1,     3,     5,     7,     1,     1,
       3,     4,     7,     8,     5,     7,     5,     0,     1,     3,
       1,     1,     3,     1,     1,     0,     2,     1,     2,     3,
       3,     2,     3,     3,     2,     0,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Cepsparser::yytname_[] =
  {
  "END", "error", "$undefined", "INTNUM", "FLOATNUM", "STRUCT", "VAL",
  "FUN", "RET", "IF", "ELSE", "FOR", "EOL", "RAWMAP", "KIND", "DOTDOT",
  "TEMPLATE", "TEMPLATE_ID", "TEMPLATE_PARAM", "MACRO", "','", "'='",
  "'|'", "'&'", "REL_OP_GT", "REL_OP_LT", "REL_OP_EQ", "REL_OP_NEQ",
  "REL_OP_LT_EQ", "REL_OP_GT_EQ", "'-'", "'+'", "'*'", "'/'", "'^'", "'.'",
  "NEG", "NOT", "STRUCTID", "ID", "KINDID", "LITERAL", "METRIC_UNIT",
  "FUNCCALL", "INDEXOP", "'{'", "'}'", "';'", "'('", "')'", "'!'", "'['",
  "']'", "':'", "$accept", "cepsscript", "stmts", "stmt", "ifthenelse",
  "general_id", "id_list", "decl", "struct_decl", "$@1", "$@2",
  "struct_initialization", "expr", "func_body", "func_stmts",
  "if_then_else", "id_or_struct_id", "for_loop_head", "for_loop",
  "template", "macro_definition", "parameter_list", "parameter",
  "argument_list", "argument", "raw_map", "raw_lines", "raw_line", "eols", YY_NULLPTR
  };


  const unsigned short int
  Cepsparser::yyrline_[] =
  {
       0,   141,   141,   150,   158,   165,   166,   170,   176,   181,
     187,   192,   196,   203,   207,   215,   216,   217,   221,   225,
     233,   237,   241,   246,   252,   258,   263,   271,   279,   287,
     297,   301,   308,   308,   308,   312,   319,   327,   333,   338,
     344,   349,   355,   361,   368,   375,   380,   385,   389,   394,
     399,   405,   410,   415,   420,   425,   429,   433,   437,   441,
     445,   449,   453,   457,   462,   467,   473,   477,   483,   485,
     489,   490,   491,   492,   493,   494,   498,   499,   503,   508,
     516,   523,   534,   545,   551,   555,   561,   572,   574,   575,
     579,   583,   588,   598,   609,   617,   620,   629,   633,   638,
     644,   652,   657,   663,   671,   681,   683
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
       2,     2,     2,    50,     2,     2,     2,     2,    23,     2,
      48,    49,    32,    31,    20,    30,    35,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    53,    47,
       2,    21,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    51,     2,    52,    34,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    45,    22,    46,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    24,    25,    26,    27,    28,
      29,    36,    37,    38,    39,    40,    41,    42,    43,    44
    };
    const unsigned int user_token_number_max_ = 289;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 31 "../src/grammar/ceps.y" // lalr1.cc:1155
} // ceps
#line 1945 "ceps.tab.c" // lalr1.cc:1155
#line 686 "../src/grammar/ceps.y" // lalr1.cc:1156


void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
