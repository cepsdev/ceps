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

#ifndef CEPS_AST_H_
#define CEPS_AST_H_


#include<map>
#include <string>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <valarray>
#include <array>
#include <memory>

#include "global_defs.hh"
#include "typefunc.hh" //We need some type functions used later on.
#include "si_units.hh"
#include "si_literals.hh"
#include <cctype>
namespace ceps {
 namespace ast {

 /**
  * The following list defines all Node-kinds currently available. The type of a node is "computed"  at compile time.
  * One of  the ingredients every node type depends upon is it's kind.
  */
	enum class Ast_node_kind {
		root,
		structdef,
		identifier,
		string_literal,
		int_literal,
		float_literal,
		expr,
		si_unit_or_derived,
		binary_operator,
		stmts,
		stmt,
		valdef,
		lambda,
		formal_parameters,
		formal_parameter,
		lambda_body,
		rawmap, /*16*/
		atoms,
		vector,
		unary_operator,
		scope,
		func_call,
		call_parameters,
		long_literal,
		unsigned_long_literal,
		kind_def,
		kind,
		symbol,
		loop,
		for_loop_head,
		nodeset,
		nodeset_path_expr
	};

 }
}

/**
 * @brief Specialization of the generic getNth_type template for YaMDL AST nodes.
 * Inductive case.
 */
template<int N,ceps::ast::Ast_node_kind K,template <ceps::ast::Ast_node_kind,typename...> class  T,typename... Ts>
 struct getNth_type<N, T <K,Ts...> >
	{
	    using Base = getNth_type<N, Ts... >;
		using type = typename Base::type;
	};

/**
 * @brief Specialization of the generich getNth_type template for YaMDL AST nodes.
 * Base case.
 */

template<ceps::ast::Ast_node_kind K,template <ceps::ast::Ast_node_kind,typename...> class  T,typename... Ts>
 struct getNth_type<0, T <K,Ts...> >
	{
	    using Base = getNth_type<0, Ts... >;
		using type = typename Base::type;
	};


namespace ceps {
 namespace ast {


/**
 * @brief Maps AST kinds to human readable descriptions.
 */
extern const char * ast_node_kind_to_text[];

/**
 * @brief Maps AST kinds to text.
 */
inline const char * c_str(Ast_node_kind k);


/**
 * The template 'type_map_is_leaf' and its specializations define a mapping AST KIND -> BOOL which is computed
 * at compile time. It enables us to perform some compile time optimizations, e.g. leafs need no storage for child nodes.
 */

/**
 * Default is a non leaf
 */
template<Ast_node_kind what>
	struct type_map_is_leaf
	{
		static constexpr bool yes = false;
	};

/**
 * Macro which saves us from typeing to much.
 */
#define AST_NODE_IS_LEAF(kind_name) template<>\
																		struct type_map_is_leaf<Ast_node_kind::kind_name>\
																			{\
																				static constexpr bool yes = true;\
																			};

#define AST_NODE_IS_NON_LEAF(kind_name)

/**
 * Strings,integers and floats have no child nodes.
 */
AST_NODE_IS_LEAF(string_literal)
AST_NODE_IS_LEAF(int_literal)
AST_NODE_IS_LEAF(float_literal)

/**
 * All AST nodesn are derived from Nodebase
 */
struct Nodebase
{
	static const int PRETTY_PRINT_INDENT_STEP_SIZE = 2 ;//Indendation used for pretty printing
	static const int iword_index; //Used by the pretty_print ostream manipulator, see chapter 15.6 in [Josuttis: "The C++ Standard Library, 2nd edition"]

	Ast_node_kind kind_;// we store the kind of a node at runtime

	/**
	 * @brief returns reference to kind of node.
	 */
	Ast_node_kind & kind()
		{
			return kind_;
		}

	/**
	 * @brief returns kind of node
	 */
	Ast_node_kind kind() const
		{
			return kind_;
		}

	/**
	 * @brief A node has a unique representation as S-Expression  (see https://en.wikipedia.org/wiki/S-expression). The
	 * virtual members print/print_content compute the unique S-Expression for a node.
	 *
	 */
	virtual void print(std::ostream& out,bool pretty_print = false,int indent = 0) const;
	virtual void print_content(std::ostream& out,bool pretty_print = false,int indent = 0) const
		{
		}
	virtual void print_value(std::ostream& out) const
	{

	}

	virtual ~Nodebase()
	{

	}

	virtual Nodebase* clone()
	{
		return new Nodebase(*this);
	}

	virtual void copy_children(std::vector<Nodebase*> &)
	{

	}


	/**
	 * ostream manipulator turning pretty printing on.
	 * see chapter 15.6 in [Josuttis: "The C++ Standard Library, 2nd edition"]
	 */
	static std::ostream& pretty_print(std::ostream& os);
	/**
	 * ostream manipulator turning pretty printing off.
	 * see chapter 15.6 in [Josuttis: "The C++ Standard Library, 2nd edition"]
	 */
	static std::ostream& pretty_print_off(std::ostream& os);

protected:
	/**
	 * Helper function printing n >= 0 blanks
	 */
	void print_ws(std::ostream& out,int n) const;
};

using Nodebase_ptr = Nodebase*;

/*
 * A node which is a leaf, inherits from Leafbase.
 */
struct Leafbase
{
	Leafbase(Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
	{
	}

};

/**
 * Nonleafs have child nodes.
 */
struct Nonleafbase
{
	using Container_t = std::vector<Nodebase_ptr>;
	std::vector<Nodebase_ptr> children_;

	Nonleafbase(Nonleafbase const & rhs)
	{
		for(auto p : rhs.children_)
		{
			children_.push_back(p->clone());
		}
	}

	std::vector<Nodebase_ptr> & children()
			{
				return children_;
			}
	std::vector<Nodebase_ptr> const & children() const
			{
				return children_;
			}
	Nodebase_ptr left()
			{
				return children_[0];
			}
	Nodebase_ptr right()
			{
				return children_[1];
			}

	Nodebase_ptr first()
			{
				return children_[0];
			}
	Nodebase_ptr second()
			{
				return children_[1];
			}

	bool empty() const
	{
		return children_.size() == 0;
	}

	Nonleafbase(Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
	{
		if (child1 == nullptr) return;
		children().push_back(child1);
		if (child2 == nullptr) return;
		children().push_back(child2);
		if (child3 == nullptr) return;
		children().push_back(child3);

	}

	virtual ~Nonleafbase()
	{

		for(Nodebase_ptr p : children())
		{
			delete p;
		}
	}

};



template <Ast_node_kind what,typename... members>
	struct ast_node;

template <Ast_node_kind what>
 	 struct ast_node<what> :  public select_type< type_map_is_leaf<what>::yes ,Leafbase,Nonleafbase >::type, public
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 Nodebase
	{
	    using First_base = typename select_type< type_map_is_leaf<what>::yes ,Leafbase,Nonleafbase >::type;
	    using This_type = ast_node<what>;
		ast_node(Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr):First_base{child1,child2,child3}
		{
			kind() = what;
		}

		/*
		 * @brief: Prints content if leaf
		 */
		 void print_content_helper(std::ostream& out, True_type const &,bool pretty_print,int indent) const
		 {}

		 /**
		  * @brief: Prints content if non leaf
		  */
		 void print_content_helper(std::ostream& out, False_type const &,bool pretty_print,int indent) const
		 {
		    int pre_indent = indent;
			indent+=PRETTY_PRINT_INDENT_STEP_SIZE;
			 for(auto p : Nonleafbase::children())
			 {
					if(pretty_print)
					{
						out << "\n";
						Nodebase::print_ws(out,indent);
					}
				 p->print(out,pretty_print,indent);
			 }
			 indent = pre_indent;
			if(pretty_print){out << "\n";Nodebase::print_ws(out,indent);}

		 }

		void print_content(std::ostream& out,bool pretty_print,int indent) const override
		{
			 print_content_helper(out,typename Select_bool_type<type_map_is_leaf<what>::yes >::type{},pretty_print,indent);
		}
		virtual void print_value(std::ostream& out) const override
		{

		}
		Nodebase* clone()
		{
			return new This_type(*this);
		}

};



template <Ast_node_kind what,typename T,typename... rest>
	struct ast_node<what,T,rest...>: public ast_node<what,rest...>
	{
		T x;
		using Base = ast_node<what,rest...>;
		using This_type = ast_node<what,T,rest...>;
		ast_node(T val_1,rest... args,Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
			: Base{args...,child1,child2,child3},x{val_1}
			{

			}
		void print_content(std::ostream& out,bool pretty_print,int indent) const override
			{
				out << x<< " "; Base::print_content(out,pretty_print,indent);
			}


		virtual void print_value(std::ostream& out) const override
		{
			out << x<< " ";Base::print_value(out);
		}

		Nodebase* clone()
		{
			return new This_type(*this);
		}
	};


template <Ast_node_kind what,typename... rest>
	struct ast_node<what,std::string,rest...>: public ast_node<what,rest...>
	{
		using T = std::string;
		T x;
		using Base = ast_node<what,rest...>;
		using This_type = ast_node<what,std::string,rest...>;
		ast_node(T val_1,rest... args,Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
			: Base(args...,child1,child2,child3),x(val_1)
			{

			}
		void print_content(std::ostream& out,bool pretty_print,int indent) const override
			{
				out << "\""<< x<< "\" "; Base::print_content(out,pretty_print,indent);
			}


		virtual void print_value(std::ostream& out) const override
		{
			out <<"\""<< x<< "\" ";Base::print_value(out);
		}

		Nodebase* clone()
		{
			return new This_type(*this);
		}

	};

template<int N,Ast_node_kind what,typename... Ts>
	struct peel_off_base;

template<Ast_node_kind what,typename T,typename... Ts>
	struct peel_off_base<0,what,T,Ts...>
	{
		using type = ast_node<what,T,Ts...>;
	};

template<int N,Ast_node_kind what,typename T,typename... Ts>
	struct peel_off_base<N,what,T,Ts...>:public peel_off_base<N-1,what,Ts...>
	{
	};

template<int N,Ast_node_kind what,typename... Ts>
	typename getNth_type<N,Ts...>::type& get( ast_node<what,Ts...> & n)
	{
		return static_cast< typename peel_off_base<N,what,Ts...>::type&>(n).x;
	}


std::ostream& operator << (std::ostream & out, Nodebase const & n);

inline Nonleafbase* nlf_ptr(Nodebase* n)
{
	return dynamic_cast<Nonleafbase*>(n);
}

struct Unit_rep
{
	using sc_t = signed char;
	sc_t m;
	sc_t kg;
	sc_t s;
	sc_t ampere;
	sc_t kelvin;
	sc_t mol;
	sc_t candela;
	Unit_rep()
		:m{},kg{},s{},ampere{},kelvin{},mol{},candela{}
	{}
	Unit_rep(sc_t p1,sc_t p2,sc_t p3, sc_t p4,sc_t p5,sc_t p6,sc_t p7)
		:m{p1},kg{p2},s{p3},ampere{p4},kelvin{p5},mol{p6},candela{p7}
	{}


};

inline bool operator == (Unit_rep const & u1, Unit_rep const & u2)
{
	return 	(u1.m == u2.m) &&
			(u1.kg == u2.kg) &&
			(u1.s == u2.s) &&
			(u1.ampere == u2.ampere) &&
			(u1.kelvin == u2.kelvin) &&
			(u1.mol == u2.mol) &&
			(u1.candela == u2.candela);
}

inline bool operator != (Unit_rep const & u1, Unit_rep const & u2)
{
	return 	! (u1 == u2);
}

inline Unit_rep operator + (Unit_rep const & u1, Unit_rep & u2)
{
	return Unit_rep{(Unit_rep::sc_t)(u1.m + u2.m),
					(Unit_rep::sc_t)(u1.kg + u2.kg),
					(Unit_rep::sc_t)(u1.s + u2.s),
					(Unit_rep::sc_t)(u1.ampere + u2.ampere),
					(Unit_rep::sc_t)(u1.kelvin + u2.kelvin),
					(Unit_rep::sc_t)(u1.mol + u2.mol),
					(Unit_rep::sc_t)(u1.candela + u2.candela)
				};
}

inline Unit_rep operator - (Unit_rep const & u1, Unit_rep & u2)
{
	return Unit_rep{(Unit_rep::sc_t)(u1.m - u2.m),
					(Unit_rep::sc_t)(u1.kg - u2.kg),
					(Unit_rep::sc_t)(u1.s - u2.s),
					(Unit_rep::sc_t)(u1.ampere - u2.ampere),
					(Unit_rep::sc_t)(u1.kelvin - u2.kelvin),
					(Unit_rep::sc_t)(u1.mol - u2.mol),
					(Unit_rep::sc_t)(u1.candela - u2.candela)
				};
}

inline std::ostream& operator << (std::ostream& os, Unit_rep const & u)
{
	if (u.m != 0) os << "m^" << (int)u.m;
	if (u.kg != 0) os << "kg^" << (int)u.kg;
	if (u.s != 0) os << "s^" << (int)u.s;
	if (u.ampere != 0) os << "A^" << (int)u.ampere;
	if (u.kelvin != 0) os << "K^" << (int)u.kelvin;
	if (u.mol != 0) os << "mol^" << (int)u.mol;
	if (u.candela != 0) os << "cd^" << (int)u.candela;

	return os;
}

inline Unit_rep operator * (int scalar, Unit_rep & u)
{
	return Unit_rep{(Unit_rep::sc_t)(scalar*u.m),
					(Unit_rep::sc_t)(scalar*u.kg),
					(Unit_rep::sc_t)(scalar*u.s),
					(Unit_rep::sc_t)(scalar*u.ampere),
					(Unit_rep::sc_t)(scalar*u.kelvin),
					(Unit_rep::sc_t)(scalar*u.mol),
					(Unit_rep::sc_t)(scalar*u.candela)
				};
}


inline Unit_rep all_one_unit()
{
	return Unit_rep{1,1,1,1,1,1,1};
}

inline Unit_rep all_zero_unit()
{
	return Unit_rep{0,0,0,0,0,0,0};
}

inline Unit_rep m_unit()
{
	return Unit_rep{1,0,0,0,0,0,0};
}
inline Unit_rep kg_unit()
{
	return Unit_rep{0,1,0,0,0,0,0};
}
inline Unit_rep s_unit()
{
	return Unit_rep{0,0,1,0,0,0,0};
}
inline Unit_rep ampere_unit()
{
	return Unit_rep{0,0,0,1,0,0,0};
}
inline Unit_rep kelvin_unit()
{
	return Unit_rep{0,0,0,0,1,0,0};
}
inline Unit_rep mol_unit()
{
	return Unit_rep{0,0,0,0,0,1,0};
}
inline Unit_rep candela_unit()
{
	return Unit_rep{0,0,0,0,0,0,1};
}


typedef ast_node<Ast_node_kind::root> Root;
typedef ast_node<Ast_node_kind::expr> Expression;
typedef ast_node<Ast_node_kind::binary_operator,char> Binary_operator;
typedef ast_node<Ast_node_kind::unary_operator,char> Unary_operator;
typedef ast_node<Ast_node_kind::structdef,std::string>  Struct;
typedef ast_node<Ast_node_kind::identifier,std::string> Identifier;
typedef ast_node<Ast_node_kind::string_literal,std::string> String;

typedef ast_node<Ast_node_kind::int_literal,
				 int, //value
				 Unit_rep // SI Unit
				 > Int;

typedef ast_node<Ast_node_kind::long_literal,
				 long, //value
				 Unit_rep // SI Unit
				 > Int64;

typedef ast_node<Ast_node_kind::unsigned_long_literal,
				 unsigned long, //value
				 Unit_rep // SI Unit
				 > Uint64;

typedef ast_node<Ast_node_kind::float_literal,
				double, //value
				Unit_rep // SI Unit
				> Double;
//typedef ast_node<Ast_node_kind::si_unit_or_derived,short int> SI_unit;
typedef ast_node<Ast_node_kind::stmts> Stmts;
typedef ast_node<Ast_node_kind::stmt> Stmt;
typedef ast_node<Ast_node_kind::valdef,std::string> Valdef;


typedef ast_node<Ast_node_kind::kind,std::string> Kind;
typedef ast_node<Ast_node_kind::kind_def,std::string> Kinddef;
typedef ast_node<Ast_node_kind::symbol,std::string,std::string> Symbol;

typedef ast_node<Ast_node_kind::lambda> Lambda;
typedef ast_node<Ast_node_kind::lambda_body> Lambda_body;
typedef ast_node<Ast_node_kind::formal_parameters> Formal_parameters;
typedef ast_node<Ast_node_kind::formal_parameter> Formal_parameter;
typedef ast_node<Ast_node_kind::rawmap> Rawmap;
typedef ast_node<Ast_node_kind::atoms> Atoms;
typedef ast_node<Ast_node_kind::vector> Vector;
typedef ast_node<Ast_node_kind::scope> Scope;

typedef ast_node<Ast_node_kind::func_call> Func_call;
typedef ast_node<Ast_node_kind::call_parameters> Call_parameters;
typedef ast_node<Ast_node_kind::loop> Loop;
typedef ast_node<Ast_node_kind::for_loop_head> Loop_head;
typedef ast_node<Ast_node_kind::nodeset,std::string> Ast_nodeset;
typedef ast_node<Ast_node_kind::nodeset_path_expr> Nodeset_path_expr;






TYPE_ALIAS(Root_ptr , Root*)
TYPE_ALIAS(Expression_ptr , Expression*)
TYPE_ALIAS(Binary_operator_ptr , Binary_operator*)
TYPE_ALIAS(Unary_operator_ptr , Unary_operator*)
TYPE_ALIAS(Struct_ptr , Struct*)
TYPE_ALIAS(Identifier_ptr , Identifier*)
TYPE_ALIAS(String_ptr , String*)
TYPE_ALIAS(Int_ptr , Int*)
TYPE_ALIAS(Double_ptr , Double*)
//TYPE_ALIAS(SI_ptr , SI*)
TYPE_ALIAS(Stmts_ptr , Stmts*)
TYPE_ALIAS(Stmt_ptr , Stmt*)
TYPE_ALIAS(Valdef_ptr , Valdef*)
TYPE_ALIAS(Lambda_ptr , Lambda*)
TYPE_ALIAS(Lambda_body_ptr , Lambda_body*)
TYPE_ALIAS(Formal_parameters_ptr , Formal_parameters*)
TYPE_ALIAS(Formal_parameter_ptr , Formal_parameter*)
TYPE_ALIAS(Rawmap_ptr , Rawmap*)
TYPE_ALIAS(Atoms_ptr , Atoms*)
TYPE_ALIAS(Scope_ptr , Scope*)
TYPE_ALIAS(Vector_ptr , Vector*)
TYPE_ALIAS(Loop_ptr , Loop*)
TYPE_ALIAS(Loop_head_ptr , Loop_head*)
TYPE_ALIAS(Ast_nodeset_ptr, Ast_nodeset*)
TYPE_ALIAS(Nodeset_path_expr_ptr, Nodeset_path_expr*)




 inline Double_ptr as_double_ptr(Nodebase_ptr p)
 {
	return dynamic_cast<Double_ptr>(p);
 }
 inline Double & as_double_ref(Nodebase_ptr p)
 {
	return *as_double_ptr(p);
 }
 inline Int_ptr as_int_ptr(Nodebase_ptr p)
 {
	return dynamic_cast<Int_ptr>(p);
 }
 inline Int & as_int_ref(Nodebase_ptr p)
 {
	return *as_int_ptr(p);
 }
 inline Identifier & as_id_ref(Nodebase_ptr p)
  {
 	return *dynamic_cast<Identifier_ptr>(p);
  }

 inline Identifier const  & as_id_ref_const(Nodebase_ptr p)
   {
  	return *dynamic_cast<Identifier const *>(p);
   }


 inline Loop_ptr as_loop_ptr(Nodebase_ptr p)
  {
   return dynamic_cast<Loop_ptr>(p);
  }
  inline Loop & as_loop_ref(Nodebase_ptr p)
  {
   return *as_loop_ptr(p);
  }

  inline Binary_operator* as_binop_ptr(Nodebase_ptr p)
    {
   	return dynamic_cast<Binary_operator*>(p);
    }

  inline Binary_operator & as_binop_ref(Nodebase_ptr p)
    {
   	return *as_binop_ptr(p);
    }

 inline Struct_ptr as_struct_ptr(Nodebase_ptr p)
  {
 	return dynamic_cast<Struct_ptr>(p);
  }
  inline Struct & as_struct_ref(Nodebase_ptr p)
  {
 	return *as_struct_ptr(p);
  }
  inline String_ptr as_string_ptr(Nodebase_ptr p)
  {
   	return dynamic_cast<String_ptr>(p);
  }
  inline String & as_string_ref(Nodebase_ptr p)
  {
   	return *as_string_ptr(p);
  }

  inline Stmts_ptr as_stmts_ptr(Nodebase_ptr p)
   {
    	return dynamic_cast<Stmts_ptr>(p);
   }
  inline Stmts & as_stmts_ref(Nodebase_ptr p)
   {
    	return *as_stmts_ptr(p);
   }



  inline Rawmap_ptr as_rawmap_ptr(Nodebase_ptr p)
  {
   	return dynamic_cast<Rawmap_ptr>(p);
  }
  inline Rawmap & as_rawmap_ref(Nodebase_ptr p)
  {
   	return *as_rawmap_ptr(p);
  }
  inline Atoms_ptr as_atoms_ptr(Nodebase_ptr p)
  {
   	return dynamic_cast<Atoms_ptr>(p);
  }

  inline Symbol & as_symbol_ref(Nodebase_ptr p)
  {
   	return *dynamic_cast<Symbol*>(p);
  }


  inline Loop_head & as_loop_head_ref(Nodebase_ptr p)
  {
   	return *dynamic_cast<Loop_head_ptr>(p);
  }

  inline Loop_head const & as_loop_head_ref_const(Nodebase_ptr p)
   {
    	return *dynamic_cast<Loop_head_ptr>(p);
   }

  inline Loop_head_ptr as_loop_head_ptr(Nodebase_ptr p)
  {
   	return dynamic_cast<Loop_head_ptr>(p);
  }

 inline Ast_nodeset & as_ast_nodeset_ref(Nodebase_ptr p)
  {
   	return *dynamic_cast<Ast_nodeset_ptr>(p);
  }


 inline Ast_nodeset_ptr as_ast_nodeset_ptr(Nodebase_ptr p)
  {
	  return dynamic_cast<Ast_nodeset_ptr>(p);
  }

 inline Ast_nodeset_ptr create_ast_nodeset(std::string index_operator_argument,std::vector<Nodebase_ptr> const & children)
 {
	 Ast_nodeset_ptr t = new ast_node<Ast_node_kind::nodeset,std::string>(index_operator_argument, nullptr, nullptr, nullptr);
	 t->children().insert(t->children().end(),children.begin(),children.end());
	 return t;
 }

 inline Nodeset_path_expr & as_ast_nodeset_path_expr_ref(Nodebase_ptr p)
   {
    	return *dynamic_cast<Nodeset_path_expr_ptr>(p);
   }


  inline Nodeset_path_expr_ptr as_nodeset_path_expr_ptr(Nodebase_ptr p)
   {
 	  return dynamic_cast<Nodeset_path_expr_ptr>(p);
   }

  inline bool is_a_nodeset(Nodebase_ptr p)
  {
	  return p->kind() == ceps::ast::Ast_node_kind::nodeset;
  }

  inline bool is_an_identifier(Nodebase_ptr p)
  {
  	  return p->kind() == ceps::ast::Ast_node_kind::identifier;
  }
  inline bool is_a_struct(Nodebase_ptr p)
  {
	  return p->kind() == ceps::ast::Ast_node_kind::structdef;
  }

 Nodebase_ptr box(int j);
 Nodebase_ptr box(unsigned int j);
 Nodebase_ptr box(long j);
 Nodebase_ptr box(unsigned long j);

 Nodebase_ptr box(double d);
 Nodebase_ptr box(std::string const & s);

 template<typename T, size_t N>
 Nodebase_ptr box(std::array<T,N> const & v)
 {
	 Vector_ptr vv = new Vector;
	 for(int i = 0; i < N; ++i)
	 {
		 vv->children().push_back(box(v[i]));
	 }
	 return vv;
 }

#ifdef CEPS_CORE_WITH_BOOST
#include "boost/array.hpp"

 template<typename T, size_t N>
  Nodebase_ptr box(boost::array<T,N> const & v)
  {
 	 Vector_ptr vv = new Vector;
 	 for(int i = 0; i < N; ++i)
 	 {
 		 vv->children().push_back(box(v[i]));
 	 }
 	 return vv;
  }


#endif


template<typename T> auto get0th(T & x) -> typename getNth_type<0,T>::type
		{
			return get<0>(x);
		}

/*
getNth_type<0, Struct >::type & name(Struct& x);
getNth_type<0, Identifier>::type & name(Identifier& x);
getNth_type<0, String >::type & value(String& x);
getNth_type<0, Double >::type & value(Double& x);
getNth_type<0, Int>::type & value(Int& x);
getNth_type<0, Int>::type &  neg(Int& x);
getNth_type<0, Double>::type &  neg(Double& x);
*/



inline getNth_type<0,  Binary_operator >::type & op(Binary_operator& x)
{
	return get<0>(x);
}
inline getNth_type<0,  Unary_operator >::type & op(Unary_operator& x)
{
	return get<0>(x);
}

inline getNth_type<0,  Struct >::type & name(Struct& x)
{
	return get<0>(x);
}
inline getNth_type<0,  Identifier>::type & name(Identifier& x)
{
	return get<0>(x);
}
inline getNth_type<0,  String >::type & value(String& x)
{
	return get<0>(x);
}
inline getNth_type<0, Double >::type & value(Double& x)
{
	return get<0>(x);
}
inline getNth_type<0, Int>::type & value(Int& x)
{
	return get<0>(x);
}
inline getNth_type<0,  Valdef>::type & name(Valdef& x)
{
	return get<0>(x);
}

inline getNth_type<0, Int>::type & neg(Int& x)
{
	return get<0>(x) =  - get<0>(x);
}

inline getNth_type<0, Double>::type & neg(Double& x)
{
	return get<0>(x) =  - get<0>(x);
}

inline getNth_type<1, Double>::type & unit(Double& x)
{
	return get<1>(x);
}

inline getNth_type<1, Int>::type &  unit(Int& x)
{
	return get<1>(x);
}

inline getNth_type<0,  Symbol >::type & name(Symbol& x)
{
	return get<0>(x);
}

inline getNth_type<1,  Symbol >::type & kind(Symbol& x)
{
	return get<1>(x);
}

inline getNth_type<0,  Ast_nodeset >::type & apply_idx_op_operand(Ast_nodeset& x)
{
	return get<0>(x);
}

/*
const std::valarray<int> METER = {1,0,0,0,0,0,0};
const std::valarray<int> KG = {0,1,0,0,0,0,0};
const std::valarray<int> SECOND = {0,0,1,0,0,0,0};
const std::valarray<int> AMPERE = {0,0,0,1,0,0,0};
const std::valarray<int> KELVIN = {0,0,0,0,1,0,0};
const std::valarray<int> MOL = {0,0,0,0,0,1,0};
const std::valarray<int> CANDELA = {0,0,0,0,0,0,1};

*/



/***************************** Parsetree ***********************************/
class Parsetree
{
public:
	  Parsetree():root_{nullptr}
	  {
	  }
	  Parsetree(Nodebase_ptr r):root_{r}
	  {
	  }
	  Nodebase_ptr get_root()
	  {
		  return root_;
	  }

private:
	  Nodebase_ptr root_;
};


/*************************** Utilities **********************************/








struct strct
{
	Struct_ptr p_strct;
	template<typename... Ts> strct(std::string const & name,Ts /*const &*/...);
	strct() = delete;
	strct(const strct & rhs)
	{
		p_strct = (rhs.p_strct != nullptr ? (Struct_ptr)rhs.p_strct->clone(): nullptr);
		//p_strct = rhs.p_strct;
		//b_copy = true;
	}
	strct(strct && rhs)

	{
		p_strct = rhs.p_strct;
		rhs.p_strct = nullptr;
	}

	Struct_ptr get_root() {return p_strct;}
	~strct()
	{
		if (p_strct != nullptr)
		{
			//std::cout << "!!"<<std::endl;
			delete p_strct;
			p_strct = nullptr;
		}
	}
};

Struct_ptr make_struct(std::string const & name);


template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, int  n,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new Int{n,Unit_rep{}});
		return p;
	}

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, unsigned int  n,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new Int{(int)n,Unit_rep{}});
		return p;
	}


template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, long  n,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new Int64{n,Unit_rep{}});
		return p;
	}

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, unsigned long  n,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new Uint64{n,Unit_rep{}});
		return p;
	}

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, double  n,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new Double{n,Unit_rep{}});
		return p;
	}

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, std::string const & s,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new String{s});
		return p;
	}

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, Rawmap * rawmap,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		if (rawmap != nullptr) p->children().insert(
				p->children().begin(),
				rawmap);
		return p;
	}

template<typename U,typename... Ts>
	Struct_ptr make_struct(std::string const & name, SI::Quantity<U,double> const & v,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
			p->children().begin(),
			new Double{v.value_,Unit_rep{U::m,U::kg,U::s,U::A,U::K,U::mol,U::cd}});
		return p;
	}
template<typename U,typename... Ts>
	Struct_ptr make_struct(std::string const & name, SI::Quantity<U,int> const & v,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
			p->children().begin(),
			new Int{v.value_,Unit_rep{U::m,U::kg,U::s,U::A,U::K,U::mol,U::cd}});
		return p;
	}


template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, const char * sz,Ts const &... args)
	{
		return  make_struct(name,std::string{sz},args...);
	}

template<typename T,typename... Ts>
	Struct_ptr make_struct(std::string const & name, T * p,Ts const &... args)
	{
		return  make_struct(name,*p,args...);
	}

template<typename T,typename... Ts>
	Struct_ptr make_struct(std::string const & name, std::vector<T> const  & il ,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);

		for (auto it = il.crbegin();it!=il.crend();++it)
		{
			T const & elem = *it;
			p->children().insert(
					p->children().begin(),box(elem));

		}
		return  p;
	}



template<typename T, size_t N, typename... Ts>
	Struct_ptr make_struct(std::string const & name, std::array<T,N> const  & arr ,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);

		for (size_t i = 0; i < N; ++i)
		{
			p->children().push_back(
					box(arr[i]));

		}
		return  p;
	}

#ifdef CEPS_CORE_WITH_BOOST
#include "boost/array.hpp"

template<typename T, size_t N, typename... Ts>
	Struct_ptr make_struct(std::string const & name, boost::array<T,N> const  & arr ,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);

		for (size_t i = 0; i < N; ++i)
		{
			p->children().push_back(
					box(arr[i]));

		}
		return  p;
	}

#endif

template<typename T,typename... Ts>
	Struct_ptr make_struct(std::string const & name, std::vector<T *> const  & il ,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);

		for (auto it = il.crbegin();it!=il.crend();++it)
		{
			auto  elem = *it;
			p->children().insert(
					p->children().begin(),elem);

		}
		return  p;
	}

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, std::vector<strct> const  & il ,Ts const &... args)
	{
		Struct_ptr p =  make_struct(name,args...);
		//std::cout << p << std::endl;
		for (auto it = il.crbegin();it!=il.crend();++it)
		{
			//std::cout << it->p_strct << std::endl;
			Struct*  elem = (Struct*)it->p_strct->clone();
			p->children().insert(
					p->children().begin(),elem);

		}
		return  p;
	}




template<typename... Ts>
	Struct_ptr make_struct(std::string const & name,  strct const & strct_,Ts... args)
	{

		Struct_ptr p1 =  (Struct_ptr)strct_.p_strct->clone();
		Struct_ptr p2 = make_struct(name,args...);
		p2->children().insert(
						p2->children().begin(),
						p1);
		return p2;
	}


template<typename... Ts>
 strct::strct(std::string const & name,Ts /*const &*/... args)
		{
			p_strct = make_struct(name,args...);
		}

/****************************** I/O *************************************/
std::ostream& operator << (std::ostream& out, strct & s);


}//namespace ast
}//namespace ceps

#endif
