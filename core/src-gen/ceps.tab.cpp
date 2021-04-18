// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "ceps.tab.h"


// Unqualified %code blocks.
#line 37 "../src/grammar/ceps.y"

#include "cepsparserdriver.hh"
#include "cepslexer.hh"
#include "ceps_ast.hh"
#include <valarray>

#line 52 "ceps.tab.c"


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

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
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
    while (false)
# endif


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
      *yycdebug_ << '\n';                       \
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 22 "../src/grammar/ceps.y"
namespace ceps {
#line 144 "ceps.tab.c"


  /// Build a parser object.
  Cepsparser::Cepsparser (Ceps_parser_driver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      driver (driver_yyarg)
  {}

  Cepsparser::~Cepsparser ()
  {}

  Cepsparser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Cepsparser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
    , location (std::move (that.location))
  {}
#endif

  template <typename Base>
  Cepsparser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Cepsparser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  Cepsparser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}

  template <typename Base>
  bool
  Cepsparser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Cepsparser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_type.
  Cepsparser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  Cepsparser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  Cepsparser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  Cepsparser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  Cepsparser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  Cepsparser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  Cepsparser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  Cepsparser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Cepsparser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Cepsparser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Cepsparser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Cepsparser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Cepsparser::symbol_number_type
  Cepsparser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  Cepsparser::stack_symbol_type::stack_symbol_type ()
  {}

  Cepsparser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Cepsparser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Cepsparser::stack_symbol_type&
  Cepsparser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  Cepsparser::stack_symbol_type&
  Cepsparser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
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
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  Cepsparser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Cepsparser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Cepsparser::yypop_ (int n)
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

  Cepsparser::state_type
  Cepsparser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Cepsparser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Cepsparser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Cepsparser::operator() ()
  {
    return parse ();
  }

  int
  Cepsparser::parse ()
  {
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

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 140 "../src/grammar/ceps.y"
 {
  ceps::ast::Root_ptr root_node = new ceps::ast::Root((yystack_[0].value.ast_node));
  driver.set_parsetree(ceps::ast::Parsetree{root_node});
  (yylhs.value.ast_node) = root_node;
 }
#line 603 "ceps.tab.c"
    break;

  case 3:
#line 149 "../src/grammar/ceps.y"
 {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 613 "ceps.tab.c"
    break;

  case 4:
#line 156 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Stmts{}; //create empty Statements-Node
}
#line 621 "ceps.tab.c"
    break;

  case 5:
#line 163 "../src/grammar/ceps.y"
               { (yylhs.value.ast_node) = new ceps::ast::Scope{(yystack_[1].value.ast_node)};}
#line 627 "ceps.tab.c"
    break;

  case 6:
#line 165 "../src/grammar/ceps.y"
{
  (yylhs.value.ast_node) = new ceps::ast::Label(*(yystack_[2].value.sval),*(yystack_[1].value.ast_list),nullptr,nullptr,nullptr,nullptr);
  delete (yystack_[1].value.ast_list);
}
#line 636 "ceps.tab.c"
    break;

  case 7:
#line 170 "../src/grammar/ceps.y"
{
 (yylhs.value.ast_node) = new ceps::ast::Return((yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 644 "ceps.tab.c"
    break;

  case 8:
#line 174 "../src/grammar/ceps.y"
{
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
}
#line 652 "ceps.tab.c"
    break;

  case 9:
#line 180 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result of decl
 }
#line 660 "ceps.tab.c"
    break;

  case 10:
#line 185 "../src/grammar/ceps.y"
 {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result of for_loop
 }
#line 668 "ceps.tab.c"
    break;

  case 11:
#line 191 "../src/grammar/ceps.y"
 {
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 676 "ceps.tab.c"
    break;

  case 12:
#line 196 "../src/grammar/ceps.y"
 {
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 684 "ceps.tab.c"
    break;

  case 13:
#line 200 "../src/grammar/ceps.y"
 {
   (yylhs.value.ast_node) = new ceps::ast::Let(*(yystack_[3].value.sval),(yystack_[1].value.ast_node));//,nullptr,nullptr,nullptr,nullptr);
 }
#line 692 "ceps.tab.c"
    break;

  case 14:
#line 204 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node) = nullptr;
 }
#line 700 "ceps.tab.c"
    break;

  case 15:
#line 211 "../src/grammar/ceps.y"
{
 (yylhs.value.ast_node) = new ceps::ast::Ifelse((yystack_[2].value.ast_node),(yystack_[0].value.ast_node));
}
#line 708 "ceps.tab.c"
    break;

  case 16:
#line 215 "../src/grammar/ceps.y"
{
(yylhs.value.ast_node) = new ceps::ast::Ifelse((yystack_[4].value.ast_node),(yystack_[2].value.ast_node),(yystack_[0].value.ast_node));
}
#line 716 "ceps.tab.c"
    break;

  case 17:
#line 222 "../src/grammar/ceps.y"
 { (yylhs.value.sval) = (yystack_[0].value.sval); }
#line 722 "ceps.tab.c"
    break;

  case 18:
#line 223 "../src/grammar/ceps.y"
  { (yylhs.value.sval) = (yystack_[0].value.sval); }
#line 728 "ceps.tab.c"
    break;

  case 19:
#line 224 "../src/grammar/ceps.y"
  { (yylhs.value.sval) = (yystack_[0].value.sval); }
#line 734 "ceps.tab.c"
    break;

  case 20:
#line 229 "../src/grammar/ceps.y"
 {
  (yylhs.value.str_list) = new std::vector<std::string> {*(yystack_[0].value.sval)};
 }
#line 742 "ceps.tab.c"
    break;

  case 21:
#line 233 "../src/grammar/ceps.y"
 {
   (yystack_[2].value.str_list)->push_back(*(yystack_[0].value.sval));
   (yylhs.value.str_list) = (yystack_[2].value.str_list);
 }
#line 751 "ceps.tab.c"
    break;

  case 22:
#line 241 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node) = (yystack_[0].value.ast_node); //pass over result 
 }
#line 759 "ceps.tab.c"
    break;

  case 23:
#line 245 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node); //pass over result
}
#line 767 "ceps.tab.c"
    break;

  case 24:
#line 249 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 776 "ceps.tab.c"
    break;

  case 25:
#line 254 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Valdef(*(yystack_[2].value.sval),(yystack_[0].value.ast_node));
	delete (yystack_[2].value.sval);
}
#line 785 "ceps.tab.c"
    break;

  case 26:
#line 260 "../src/grammar/ceps.y"
{
	driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
	delete (yystack_[0].value.sval); 
	(yylhs.value.ast_node)=nullptr;
}
#line 795 "ceps.tab.c"
    break;

  case 27:
#line 266 "../src/grammar/ceps.y"
{
	delete (yystack_[0].value.sval);
	(yylhs.value.ast_node)=nullptr;
}
#line 804 "ceps.tab.c"
    break;

  case 28:
#line 271 "../src/grammar/ceps.y"
{
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 816 "ceps.tab.c"
    break;

  case 29:
#line 279 "../src/grammar/ceps.y"
{
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 828 "ceps.tab.c"
    break;

  case 30:
#line 287 "../src/grammar/ceps.y"
{
 auto symbol = driver.symboltable().lookup(*(yystack_[0].value.sval),true,true,false);
 symbol->category = ceps::parser_env::Symbol::KIND;
 //$$= new ceps::ast::Kind{*$2};
 delete (yystack_[0].value.sval);
 (yylhs.value.ast_node) = nullptr; 	
}
#line 840 "ceps.tab.c"
    break;

  case 31:
#line 295 "../src/grammar/ceps.y"
{
 auto r = new ceps::ast::Kinddef(*(yystack_[1].value.sval),nullptr,nullptr,nullptr);
 for (auto s : *(yystack_[0].value.str_list))
 {
  r->children().push_back(new ceps::ast::Identifier(s,nullptr,nullptr,nullptr)); 
 }
 delete (yystack_[0].value.str_list); 
 (yylhs.value.ast_node) = r;
}
#line 854 "ceps.tab.c"
    break;

  case 32:
#line 305 "../src/grammar/ceps.y"
{
  (yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 862 "ceps.tab.c"
    break;

  case 33:
#line 309 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Expression((yystack_[0].value.ast_node),nullptr,nullptr);
}
#line 870 "ceps.tab.c"
    break;

  case 34:
#line 315 "../src/grammar/ceps.y"
         {driver.symboltable().push_scope();}
#line 876 "ceps.tab.c"
    break;

  case 35:
#line 315 "../src/grammar/ceps.y"
                                                                    {driver.symboltable().pop_scope();}
#line 882 "ceps.tab.c"
    break;

  case 36:
#line 316 "../src/grammar/ceps.y"
{
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[3].value.sval),(yystack_[1].value.ast_node),nullptr,nullptr);
}
#line 890 "ceps.tab.c"
    break;

  case 37:
#line 320 "../src/grammar/ceps.y"
{
 (yylhs.value.ast_node) = new ceps::ast::Struct(*(yystack_[2].value.sval),new ceps::ast::Identifier(*(yystack_[1].value.sval),nullptr,nullptr,nullptr),(yystack_[0].value.ast_node),nullptr);
}
#line 898 "ceps.tab.c"
    break;

  case 38:
#line 327 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 906 "ceps.tab.c"
    break;

  case 39:
#line 335 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node) = new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
 	delete (yystack_[0].value.sval);
 }
#line 915 "ceps.tab.c"
    break;

  case 40:
#line 341 "../src/grammar/ceps.y"
 {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr);
    delete (yystack_[0].value.sval);
  }
#line 924 "ceps.tab.c"
    break;

  case 41:
#line 347 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 933 "ceps.tab.c"
    break;

  case 42:
#line 352 "../src/grammar/ceps.y"
{
 //TODO:Handle kindid
}
#line 941 "ceps.tab.c"
    break;

  case 43:
#line 358 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr);
}
#line 949 "ceps.tab.c"
    break;

  case 44:
#line 364 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep{},nullptr,nullptr,nullptr);
}
#line 957 "ceps.tab.c"
    break;

  case 45:
#line 370 "../src/grammar/ceps.y"
{  
	//ceps::ast::neg(*dynamic_cast<ceps::ast::Double*>($2));
	//$$=$2;
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('-',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 967 "ceps.tab.c"
    break;

  case 46:
#line 377 "../src/grammar/ceps.y"
{  
	
	(yylhs.value.ast_node) = new ceps::ast::Unary_operator('!',(yystack_[0].value.ast_node),nullptr,nullptr); 
}
#line 976 "ceps.tab.c"
    break;

  case 47:
#line 383 "../src/grammar/ceps.y"
{
 (yylhs.value.ast_node) = new ceps::ast::Binary_operator('#',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 984 "ceps.tab.c"
    break;

  case 48:
#line 388 "../src/grammar/ceps.y"
{
 (yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::DOTDOT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 992 "ceps.tab.c"
    break;

  case 49:
#line 393 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(',',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1000 "ceps.tab.c"
    break;

  case 50:
#line 398 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('|',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1008 "ceps.tab.c"
    break;

  case 51:
#line 402 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('&',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1016 "ceps.tab.c"
    break;

  case 52:
#line 408 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('+',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1024 "ceps.tab.c"
    break;

  case 53:
#line 413 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('-',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1032 "ceps.tab.c"
    break;

  case 54:
#line 419 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('*',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);			
}
#line 1040 "ceps.tab.c"
    break;

  case 55:
#line 424 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('/',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1048 "ceps.tab.c"
    break;

  case 56:
#line 429 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('^',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1056 "ceps.tab.c"
    break;

  case 57:
#line 433 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('.',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr); 
}
#line 1064 "ceps.tab.c"
    break;

  case 58:
#line 438 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator('=',(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1072 "ceps.tab.c"
    break;

  case 59:
#line 442 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1080 "ceps.tab.c"
    break;

  case 60:
#line 446 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1088 "ceps.tab.c"
    break;

  case 61:
#line 450 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1096 "ceps.tab.c"
    break;

  case 62:
#line 454 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_LT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1104 "ceps.tab.c"
    break;

  case 63:
#line 458 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_GT_EQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1112 "ceps.tab.c"
    break;

  case 64:
#line 462 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Binary_operator(ceps::Cepsparser::token::REL_OP_NEQ,(yystack_[2].value.ast_node),(yystack_[0].value.ast_node),nullptr);
}
#line 1120 "ceps.tab.c"
    break;

  case 65:
#line 466 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1128 "ceps.tab.c"
    break;

  case 66:
#line 470 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1136 "ceps.tab.c"
    break;

  case 67:
#line 475 "../src/grammar/ceps.y"
{
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call{(yystack_[3].value.ast_node),(yystack_[1].value.ast_node)};
}
#line 1145 "ceps.tab.c"
    break;

  case 68:
#line 480 "../src/grammar/ceps.y"
{
 //std::cout << "FUNCTION!!" << std::endl;
 (yylhs.value.ast_node) = new ceps::ast::Func_call((yystack_[2].value.ast_node),new  ceps::ast::Call_parameters(), nullptr);
}
#line 1154 "ceps.tab.c"
    break;

  case 69:
#line 486 "../src/grammar/ceps.y"
{
 //TODO: Function Definitions
}
#line 1162 "ceps.tab.c"
    break;

  case 70:
#line 489 "../src/grammar/ceps.y"
  { (yylhs.value.ast_node) = (yystack_[3].value.ast_node); }
#line 1168 "ceps.tab.c"
    break;

  case 81:
#line 516 "../src/grammar/ceps.y"
 {
    (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
 }
#line 1177 "ceps.tab.c"
    break;

  case 82:
#line 521 "../src/grammar/ceps.y"
{
   (yylhs.value.ast_node) = new ceps::ast::Identifier(*(yystack_[0].value.sval),nullptr,nullptr,nullptr); 
	delete (yystack_[0].value.sval);
}
#line 1186 "ceps.tab.c"
    break;

  case 83:
#line 529 "../src/grammar/ceps.y"
 {
  auto head = new ceps::ast::Loop_head();
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1197 "ceps.tab.c"
    break;

  case 84:
#line 536 "../src/grammar/ceps.y"
 {
  ceps::ast::Loop_head_ptr head = ceps::ast::as_loop_head_ptr((yystack_[3].value.ast_node));
  head->children().push_back((yystack_[2].value.ast_node));
  head->children().push_back((yystack_[0].value.ast_node));
  (yylhs.value.ast_node) = head;
 }
#line 1208 "ceps.tab.c"
    break;

  case 85:
#line 547 "../src/grammar/ceps.y"
 {
  auto temp = new ceps::ast::Loop();

  temp->children().push_back((yystack_[4].value.ast_node));
  temp->children().push_back((yystack_[1].value.ast_node));
  (yylhs.value.ast_node) = temp;
 }
#line 1220 "ceps.tab.c"
    break;

  case 86:
#line 558 "../src/grammar/ceps.y"
{
  auto temp = new ceps::ast::Template_defintion(*(yystack_[6].value.sval),*(yystack_[4].value.str_list),nullptr,nullptr,nullptr);
  
}
#line 1229 "ceps.tab.c"
    break;

  case 87:
#line 564 "../src/grammar/ceps.y"
{
}
#line 1236 "ceps.tab.c"
    break;

  case 88:
#line 568 "../src/grammar/ceps.y"
{
}
#line 1243 "ceps.tab.c"
    break;

  case 89:
#line 574 "../src/grammar/ceps.y"
 {
  auto symbol = driver.symboltable().lookup(*(yystack_[4].value.sval),true,true,false);
  symbol->category = ceps::parser_env::Symbol::MACRO;
  symbol->payload = (yystack_[1].value.ast_node);
  auto temp{new ceps::ast::Macrodef(*(yystack_[4].value.sval),symbol,*(yystack_[3].value.ast_list))};
  delete (yystack_[4].value.sval);delete (yystack_[3].value.ast_list);
  (yylhs.value.ast_node) = temp; 	  
 }
#line 1256 "ceps.tab.c"
    break;

  case 94:
#line 597 "../src/grammar/ceps.y"
 {
  (yylhs.value.ast_node) = new ceps::ast::Call_parameters((yystack_[0].value.ast_node),nullptr,nullptr);
 }
#line 1264 "ceps.tab.c"
    break;

  case 95:
#line 602 "../src/grammar/ceps.y"
 {
  if((yystack_[0].value.ast_node) != nullptr) 
  	ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back((yystack_[0].value.ast_node));
    
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
 }
#line 1275 "ceps.tab.c"
    break;

  case 96:
#line 612 "../src/grammar/ceps.y"
 {
  (yylhs.value.ast_node) = (yystack_[0].value.ast_node);
 }
#line 1283 "ceps.tab.c"
    break;

  case 97:
#line 623 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node)=(yystack_[0].value.ast_node);
 }
#line 1291 "ceps.tab.c"
    break;

  case 98:
#line 630 "../src/grammar/ceps.y"
 {
 	(yylhs.value.ast_node) = new ceps::ast::Rawmap();
 }
#line 1299 "ceps.tab.c"
    break;

  case 99:
#line 634 "../src/grammar/ceps.y"
 { 
 	if (!ceps::ast::nlf_ptr((yystack_[0].value.ast_node))->empty()) 
 	 ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back((yystack_[0].value.ast_node)); 
 	(yylhs.value.ast_node) = (yystack_[1].value.ast_node);
 }
#line 1309 "ceps.tab.c"
    break;

  case 100:
#line 643 "../src/grammar/ceps.y"
{
	(yylhs.value.ast_node) = new ceps::ast::Atoms();
}
#line 1317 "ceps.tab.c"
    break;

  case 101:
#line 647 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1326 "ceps.tab.c"
    break;

  case 102:
#line 652 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int(-1 * (yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1335 "ceps.tab.c"
    break;

  case 103:
#line 658 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Int((yystack_[0].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1345 "ceps.tab.c"
    break;

  case 104:
#line 666 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1354 "ceps.tab.c"
    break;

  case 105:
#line 671 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double(-1.0*(yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1363 "ceps.tab.c"
    break;

  case 106:
#line 677 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[2].value.ast_node))->children().push_back(new ceps::ast::Double((yystack_[0].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));

  (yylhs.value.ast_node) = (yystack_[2].value.ast_node);
}
#line 1373 "ceps.tab.c"
    break;

  case 107:
#line 685 "../src/grammar/ceps.y"
{
  ceps::ast::nlf_ptr((yystack_[1].value.ast_node))->children().push_back(new ceps::ast::String(*(yystack_[0].value.sval),nullptr,nullptr,nullptr));
  delete (yystack_[0].value.sval);
  (yylhs.value.ast_node) = (yystack_[1].value.ast_node);
}
#line 1383 "ceps.tab.c"
    break;

  case 108:
#line 693 "../src/grammar/ceps.y"
{
  (yylhs.value.ast_list) = new std::vector<ceps::ast::Nodebase_ptr>;
}
#line 1391 "ceps.tab.c"
    break;

  case 109:
#line 698 "../src/grammar/ceps.y"
{
  (yystack_[0].value.ast_list) -> push_back(new ceps::ast::String(*(yystack_[3].value.sval),nullptr,nullptr,nullptr));
  (yystack_[0].value.ast_list) -> push_back(new ceps::ast::String(*(yystack_[1].value.sval),nullptr,nullptr,nullptr));
  (yylhs.value.ast_list) = (yystack_[0].value.ast_list);
}
#line 1401 "ceps.tab.c"
    break;

  case 110:
#line 705 "../src/grammar/ceps.y"
{
  (yystack_[0].value.ast_list) -> push_back(new ceps::ast::String(*(yystack_[3].value.sval),nullptr,nullptr,nullptr)); 
  (yystack_[0].value.ast_list) -> push_back(new ceps::ast::Int((yystack_[1].value.ival),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));  
  (yylhs.value.ast_list) = (yystack_[0].value.ast_list);
}
#line 1411 "ceps.tab.c"
    break;

  case 111:
#line 712 "../src/grammar/ceps.y"
{
  (yystack_[0].value.ast_list) -> push_back(new ceps::ast::String(*(yystack_[3].value.sval),nullptr,nullptr,nullptr));
  (yystack_[0].value.ast_list)->push_back(new ceps::ast::Double((yystack_[1].value.fval),ceps::ast::Unit_rep(),nullptr,nullptr,nullptr));
  (yylhs.value.ast_list) = (yystack_[0].value.ast_list);
}
#line 1421 "ceps.tab.c"
    break;


#line 1425 "ceps.tab.c"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

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
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
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
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
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
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Cepsparser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  Cepsparser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const short Cepsparser::yypact_ninf_ = -206;

  const signed char Cepsparser::yytable_ninf_ = -35;

  const short
  Cepsparser::yypact_[] =
  {
    -206,    27,   467,  -206,  -206,  -206,    64,    74,   -14,     1,
      -3,    -1,    20,    42,    61,    61,    61,    61,     1,   -22,
    -206,    70,  -206,  -206,  -206,     1,     1,     1,  -206,  -206,
       6,  -206,   861,  -206,    28,    36,  -206,  -206,    66,    75,
      80,  -206,  -206,   588,     1,    76,    20,    60,  -206,  -206,
    -206,  -206,  -206,  -206,   -21,    61,    61,   107,    -5,    90,
      90,   122,  -206,   129,   190,   621,    -5,   522,  -206,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,   486,     1,  -206,
    -206,     1,     1,  -206,    99,   139,  -206,   657,  -206,  -206,
     105,   -19,  -206,  -206,  -206,   -24,   140,   114,   118,     1,
    -206,  -206,  -206,     1,    61,  -206,  -206,  -206,   957,   893,
     925,   957,   987,  1016,  1028,  1055,   106,   151,  1063,  1085,
      56,    56,   175,   175,   222,    -5,  -206,   861,   -17,  -206,
     555,   861,   861,   121,    80,   467,     1,   124,   127,   131,
     161,   245,   130,   -16,    38,  -206,  -206,   693,   264,  -206,
     861,  -206,     1,  -206,  -206,   123,  -206,   197,   861,  -206,
       1,  -206,  -206,     7,  -206,  -206,   166,    61,    61,    61,
     319,  -206,  -206,  -206,   173,     1,   165,   123,   726,   170,
    -206,  -206,  -206,   467,   338,   861,  -206,  -206,   147,   154,
    -206,   393,  -206,  -206,  -206,  -206,  -206,   199,   759,     1,
     171,   123,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
     412,     1,   123,   792,  -206,  -206,  -206,   828,  -206,   123,
     123,   217,  -206,   123,  -206
  };

  const signed char
  Cepsparser::yydefact_[] =
  {
       4,     0,     2,     1,    43,    44,     0,     0,     0,     0,
       0,     0,   112,     0,     0,     0,     0,     0,     0,    41,
      40,    42,    39,     4,    14,     0,     0,     0,     3,     8,
       0,    22,    33,    10,     0,     0,    27,    26,     0,     0,
      90,    41,    42,     0,     0,     0,   112,     0,    29,    28,
      30,    18,    17,    19,     0,   108,   108,     0,    45,     0,
       0,    17,    20,    31,     0,     0,    46,     0,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    11,
      12,     0,     0,    93,     0,    91,     7,     0,    82,    81,
       0,     0,   113,    98,     4,     0,     0,     0,     0,     0,
       4,    37,    35,     0,     0,     5,    65,    66,    48,    47,
      49,    58,    50,    51,    61,    60,    59,    64,    62,    63,
      53,    52,    54,    55,    56,    57,    68,    96,     0,    94,
       0,    25,    24,     0,    90,     0,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     4,     6,     0,     0,    36,
      32,    21,     0,    67,    70,    71,    92,    15,    83,     4,
       0,    23,   100,    99,    87,     4,     0,   108,   108,   108,
       0,    13,    38,    95,     0,     0,     0,    71,     0,     0,
      72,    76,    77,     0,     0,    84,   101,   104,     0,     0,
     107,     0,     4,   110,   111,   109,    89,     0,     0,     0,
       0,    71,    69,    16,    85,   102,   105,   103,   106,    88,
       0,     0,    71,     0,    78,    73,    86,     0,    75,     0,
      71,    79,    74,     0,    80
  };

  const short
  Cepsparser::yypgoto_[] =
  {
    -206,  -206,   -20,  -143,  -206,    -2,   125,  -206,  -206,  -206,
    -206,   168,    -9,   -94,  -205,  -206,   132,  -206,  -131,  -206,
    -206,    87,  -206,  -206,    77,  -206,  -206,  -206,   -55,   192
  };

  const short
  Cepsparser::yydefgoto_[] =
  {
      -1,     1,     2,    28,    29,   106,    63,    30,    31,    60,
     159,   111,    32,   189,   190,   191,   100,   101,    33,    34,
      35,    94,    95,   138,   139,   149,   150,   173,   107,    47
  };

  const short
  Cepsparser::yytable_[] =
  {
      43,   108,   167,    64,     4,     5,   162,   114,     8,    58,
     196,   197,    54,    55,    56,    57,    65,    66,    67,    62,
      51,    52,    53,    59,   231,    98,    99,     3,   234,   -34,
     104,   152,    46,   105,   192,    97,   147,    18,   163,   176,
      40,   177,   178,   198,   199,    41,    20,    42,    22,    87,
     213,    44,    88,    45,   200,    25,   192,    26,    27,    68,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   137,   140,
     192,    89,   141,   142,   151,   179,    48,    49,    50,    90,
     158,   192,    91,   210,    83,    84,    85,    86,   192,   192,
     157,    92,   192,    62,   160,    51,    52,    53,    36,    37,
      87,   103,   161,    88,    51,    61,    53,   225,    38,    39,
      98,    99,   203,   204,   205,    93,     4,     5,   228,   184,
       8,   185,   186,   109,    11,   180,   232,   168,    78,    79,
      80,   110,    81,    82,    83,    84,    85,    86,   113,   194,
     215,   216,   114,   137,   143,   201,   188,   217,   218,    18,
      87,   195,   144,    88,   146,   155,   154,    41,    20,    42,
      22,   156,   165,   172,   187,   169,   208,    25,   188,    26,
      27,   175,   220,   171,    79,    80,   170,    81,    82,    83,
      84,    85,    86,     4,     5,     6,     7,     8,     9,    10,
     223,    11,   188,    12,    13,    87,    14,   193,    88,    15,
      16,    17,   227,   188,    84,    85,    86,   202,   207,   209,
     188,   188,   212,   224,   188,   221,    18,   233,   112,    87,
     153,   166,    88,   148,    19,    20,    21,    22,   102,   183,
       0,    23,   115,    24,    25,     0,    26,    27,     4,     5,
       6,     7,     8,     9,    10,     0,    11,     0,    12,    13,
       0,    14,    85,    86,    15,    16,    17,     4,     5,     6,
       7,     8,     9,    10,     0,    11,    87,    12,    13,    88,
      14,    18,     0,    15,    16,    17,     0,     0,     0,    19,
      20,    21,    22,     0,     0,     0,    23,   174,    24,    25,
      18,    26,    27,     0,     0,     0,     0,     0,    19,    20,
      21,    22,     0,     0,     0,    23,   182,    24,    25,     0,
      26,    27,     4,     5,     6,     7,     8,     9,    10,     0,
      11,     0,    12,    13,     0,    14,     0,     0,    15,    16,
      17,     4,     5,     6,     7,     8,     9,    10,     0,    11,
       0,    12,    13,     0,    14,    18,     0,    15,    16,    17,
       0,     0,     0,    19,    20,    21,    22,     0,     0,     0,
      23,   206,    24,    25,    18,    26,    27,     0,     0,     0,
       0,     0,    19,    20,    21,    22,     0,     0,     0,    23,
     214,    24,    25,     0,    26,    27,     4,     5,     6,     7,
       8,     9,    10,     0,    11,     0,    12,    13,     0,    14,
       0,     0,    15,    16,    17,     4,     5,     6,     7,     8,
       9,    10,     0,    11,     0,    12,    13,     0,    14,    18,
       0,    15,    16,    17,     0,     0,     0,    19,    20,    21,
      22,     0,     0,     0,    23,   219,    24,    25,    18,    26,
      27,     0,     0,     0,     0,     0,    19,    20,    21,    22,
       0,     0,     0,    23,   226,    24,    25,     0,    26,    27,
       4,     5,     6,     7,     8,     9,    10,     0,    11,     0,
      12,    13,     0,    14,     0,     0,    15,    16,    17,     4,
       5,     0,     0,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,     0,     0,     0,     0,     0,
       0,    19,    20,    21,    22,     0,     0,     0,    23,     0,
      24,    25,    18,    26,    27,     0,     0,     0,     0,     0,
      41,    20,    42,    22,     0,     0,     0,    69,     0,     0,
      25,   136,    26,    27,    70,    71,     0,     0,    72,    73,
      74,    75,    76,    77,    78,    79,    80,     0,    81,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,     0,    87,    70,    71,    88,
     117,    72,    73,    74,    75,    76,    77,    78,    79,    80,
       0,    81,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    87,
      70,    71,    88,   164,    72,    73,    74,    75,    76,    77,
      78,    79,    80,     0,    81,    82,    83,    84,    85,    86,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,    96,    87,    70,    71,    88,     0,    72,    73,    74,
      75,    76,    77,    78,    79,    80,     0,    81,    82,    83,
      84,    85,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,     0,    87,   116,     0,    88,    70,
      71,     0,     0,    72,    73,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,    87,   145,     0,    88,    70,    71,     0,     0,    72,
      73,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,    83,    84,    85,    86,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,   181,    87,    70,    71,
      88,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,     0,    81,    82,    83,    84,    85,    86,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,   211,
      87,    70,    71,    88,     0,    72,    73,    74,    75,    76,
      77,    78,    79,    80,     0,    81,    82,    83,    84,    85,
      86,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,   222,    87,    70,    71,    88,     0,    72,    73,
      74,    75,    76,    77,    78,    79,    80,     0,    81,    82,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,    87,   229,     0,    88,
      70,    71,     0,     0,    72,    73,    74,    75,    76,    77,
      78,    79,    80,     0,    81,    82,    83,    84,    85,    86,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,   230,    87,    70,    71,    88,     0,    72,    73,    74,
      75,    76,    77,    78,    79,    80,     0,    81,    82,    83,
      84,    85,    86,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,     0,     0,    87,    71,     0,    88,    72,
      73,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,    83,    84,    85,    86,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,     0,     0,    87,     0,     0,
      88,    72,    73,    74,    75,    76,    77,    78,    79,    80,
       0,    81,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,    88,    72,    73,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,    88,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,    88,    75,    76,    77,    78,    79,
      80,     0,    81,    82,    83,    84,    85,    86,    76,    77,
      78,    79,    80,     0,    81,    82,    83,    84,    85,    86,
      87,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,    88,    77,    78,    79,    80,
       0,    81,    82,    83,    84,    85,    86,    80,     0,    81,
      82,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,    88,     0,     0,     0,     0,    87,     0,     0,
      88,    81,    82,    83,    84,    85,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,    88
  };

  const short
  Cepsparser::yycheck_[] =
  {
       9,    56,   145,    23,     3,     4,    23,    23,     7,    18,
       3,     4,    14,    15,    16,    17,    25,    26,    27,    21,
      44,    45,    46,    45,   229,    44,    45,     0,   233,    51,
      51,    55,    12,    54,   165,    44,    55,    36,    55,    55,
      54,     3,     4,    36,    37,    44,    45,    46,    47,    54,
     193,    54,    57,    54,    47,    54,   187,    56,    57,    53,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
     211,    53,    91,    92,   104,    47,    44,    45,    46,    53,
     110,   222,    26,   187,    38,    39,    40,    41,   229,   230,
     109,    26,   233,   105,   113,    44,    45,    46,    44,    45,
      54,    51,   114,    57,    44,    45,    46,   211,    44,    45,
      44,    45,   177,   178,   179,    45,     3,     4,   222,     6,
       7,     8,     9,    26,    11,   155,   230,   146,    32,    33,
      34,    51,    36,    37,    38,    39,    40,    41,    26,   169,
       3,     4,    23,   162,    55,   175,   165,     3,     4,    36,
      54,   170,    23,    57,    59,    51,    26,    44,    45,    46,
      47,    53,    51,    12,    51,    51,   185,    54,   187,    56,
      57,    51,   202,    52,    33,    34,    59,    36,    37,    38,
      39,    40,    41,     3,     4,     5,     6,     7,     8,     9,
     209,    11,   211,    13,    14,    54,    16,    10,    57,    19,
      20,    21,   221,   222,    39,    40,    41,    51,    45,    54,
     229,   230,    52,    52,   233,    26,    36,    10,    60,    54,
     105,   144,    57,   101,    44,    45,    46,    47,    46,   162,
      -1,    51,    52,    53,    54,    -1,    56,    57,     3,     4,
       5,     6,     7,     8,     9,    -1,    11,    -1,    13,    14,
      -1,    16,    40,    41,    19,    20,    21,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    54,    13,    14,    57,
      16,    36,    -1,    19,    20,    21,    -1,    -1,    -1,    44,
      45,    46,    47,    -1,    -1,    -1,    51,    52,    53,    54,
      36,    56,    57,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    -1,    -1,    -1,    51,    52,    53,    54,    -1,
      56,    57,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    -1,    13,    14,    -1,    16,    -1,    -1,    19,    20,
      21,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      -1,    13,    14,    -1,    16,    36,    -1,    19,    20,    21,
      -1,    -1,    -1,    44,    45,    46,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    36,    56,    57,    -1,    -1,    -1,
      -1,    -1,    44,    45,    46,    47,    -1,    -1,    -1,    51,
      52,    53,    54,    -1,    56,    57,     3,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    13,    14,    -1,    16,
      -1,    -1,    19,    20,    21,     3,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    13,    14,    -1,    16,    36,
      -1,    19,    20,    21,    -1,    -1,    -1,    44,    45,    46,
      47,    -1,    -1,    -1,    51,    52,    53,    54,    36,    56,
      57,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      -1,    -1,    -1,    51,    52,    53,    54,    -1,    56,    57,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    -1,
      13,    14,    -1,    16,    -1,    -1,    19,    20,    21,     3,
       4,    -1,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    -1,    -1,    -1,    51,    -1,
      53,    54,    36,    56,    57,    -1,    -1,    -1,    -1,    -1,
      44,    45,    46,    47,    -1,    -1,    -1,    15,    -1,    -1,
      54,    55,    56,    57,    22,    23,    -1,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    54,    22,    23,    57,
      58,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    54,
      22,    23,    57,    58,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    53,    54,    22,    23,    57,    -1,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    54,    55,    -1,    57,    22,
      23,    -1,    -1,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    54,    55,    -1,    57,    22,    23,    -1,    -1,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    53,    54,    22,    23,
      57,    -1,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    53,
      54,    22,    23,    57,    -1,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    53,    54,    22,    23,    57,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    54,    55,    -1,    57,
      22,    23,    -1,    -1,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    53,    54,    22,    23,    57,    -1,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    23,    -1,    57,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    30,    31,
      32,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    57,    31,    32,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    34,    -1,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57
  };

  const signed char
  Cepsparser::yystos_[] =
  {
       0,    61,    62,     0,     3,     4,     5,     6,     7,     8,
       9,    11,    13,    14,    16,    19,    20,    21,    36,    44,
      45,    46,    47,    51,    53,    54,    56,    57,    63,    64,
      67,    68,    72,    78,    79,    80,    44,    45,    44,    45,
      54,    44,    46,    72,    54,    54,    12,    89,    44,    45,
      46,    44,    45,    46,    65,    65,    65,    65,    72,    45,
      69,    45,    65,    66,    62,    72,    72,    72,    53,    15,
      22,    23,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    36,    37,    38,    39,    40,    41,    54,    57,    53,
      53,    26,    26,    45,    81,    82,    53,    72,    44,    45,
      76,    77,    89,    51,    51,    54,    65,    88,    88,    26,
      51,    71,    71,    26,    23,    52,    55,    58,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    55,    72,    83,    84,
      72,    72,    72,    55,    23,    55,    59,    55,    76,    85,
      86,    62,    55,    66,    26,    51,    53,    72,    62,    70,
      72,    65,    23,    55,    58,    51,    81,    63,    72,    51,
      59,    52,    12,    87,    52,    51,    55,     3,     4,    47,
      62,    53,    52,    84,     6,     8,     9,    51,    72,    73,
      74,    75,    78,    10,    62,    72,     3,     4,    36,    37,
      47,    62,    51,    88,    88,    88,    52,    45,    72,    54,
      73,    53,    52,    63,    52,     3,     4,     3,     4,    52,
      62,    26,    53,    72,    52,    73,    52,    72,    73,    55,
      53,    74,    73,    10,    74
  };

  const signed char
  Cepsparser::yyr1_[] =
  {
       0,    60,    61,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    64,    64,    65,    65,    65,
      66,    66,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    69,    70,    68,    68,    71,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    75,
      75,    76,    76,    77,    77,    78,    79,    79,    79,    80,
      81,    81,    81,    82,    83,    83,    84,    85,    86,    86,
      87,    87,    87,    87,    87,    87,    87,    87,    88,    88,
      88,    88,    89,    89
  };

  const signed char
  Cepsparser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     3,     4,     3,     1,     2,
       1,     2,     2,     5,     1,     5,     7,     1,     1,     1,
       1,     3,     1,     5,     4,     4,     2,     2,     2,     2,
       2,     2,     4,     1,     0,     0,     4,     3,     3,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     3,     7,
       4,     0,     1,     3,     6,     4,     1,     1,     3,     5,
       7,     1,     1,     3,     4,     7,     8,     5,     7,     6,
       0,     1,     3,     1,     1,     3,     1,     1,     0,     2,
       1,     2,     3,     3,     2,     3,     3,     2,     0,     4,
       4,     4,     0,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Cepsparser::yytname_[] =
  {
  "END", "error", "$undefined", "INTNUM", "FLOATNUM", "STRUCT", "VAL",
  "FUN", "RET", "IF", "ELSE", "FOR", "EOL", "RAWMAP", "KIND", "DOTDOT",
  "TEMPLATE", "TEMPLATE_ID", "TEMPLATE_PARAM", "MACRO", "LABEL", "LET",
  "'#'", "','", "LEFTARROW", "RIGHTARROW", "'='", "'|'", "'&'",
  "REL_OP_GT", "REL_OP_LT", "REL_OP_EQ", "REL_OP_NEQ", "REL_OP_LT_EQ",
  "REL_OP_GT_EQ", "'~'", "'-'", "'+'", "'*'", "'/'", "'^'", "'.'", "NEG",
  "NOT", "STRUCTID", "ID", "KINDID", "LITERAL", "METRIC_UNIT", "FUNCCALL",
  "INDEXOP", "'{'", "'}'", "';'", "'('", "')'", "'!'", "'['", "']'", "':'",
  "$accept", "cepsscript", "stmts", "stmt", "ifthenelse", "general_id",
  "id_list", "decl", "struct_decl", "$@1", "$@2", "struct_initialization",
  "expr", "func_body", "func_stmts", "if_then_else", "id_or_struct_id",
  "for_loop_head", "for_loop", "template", "macro_definition",
  "parameter_list", "parameter", "argument_list", "argument", "raw_map",
  "raw_lines", "raw_line", "attribute_list", "eols", YY_NULLPTR
  };


  const short
  Cepsparser::yyrline_[] =
  {
       0,   139,   139,   148,   156,   163,   164,   169,   173,   179,
     184,   190,   195,   199,   203,   210,   214,   222,   223,   224,
     228,   232,   240,   244,   248,   253,   259,   265,   270,   278,
     286,   294,   304,   308,   315,   315,   315,   319,   326,   334,
     340,   345,   351,   356,   362,   368,   375,   382,   387,   392,
     397,   401,   406,   411,   417,   422,   427,   432,   437,   441,
     445,   449,   453,   457,   461,   465,   469,   474,   479,   485,
     489,   495,   497,   501,   502,   503,   504,   505,   506,   510,
     511,   515,   520,   528,   535,   546,   557,   563,   567,   573,
     585,   587,   588,   592,   596,   601,   611,   622,   630,   633,
     642,   646,   651,   657,   665,   670,   676,   684,   693,   697,
     704,   711,   722,   724
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
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Cepsparser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Cepsparser::token_number_type
  Cepsparser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,     2,    22,     2,     2,    28,     2,
      54,    55,    38,    37,    23,    36,    41,    39,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    59,    53,
       2,    26,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    57,     2,    58,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,    27,    52,    35,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    24,    25,    29,
      30,    31,    32,    33,    34,    42,    43,    44,    45,    46,
      47,    48,    49,    50
    };
    const int user_token_number_max_ = 293;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

#line 22 "../src/grammar/ceps.y"
} // ceps
#line 2109 "ceps.tab.c"

#line 727 "../src/grammar/ceps.y"


void ceps::Cepsparser::error (const ceps::Cepsparser::location_type& l, const std::string& m)
{
	driver.error(l,m);	
}
