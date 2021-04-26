/*
Copyright 2014,2015,2016,2017,2018,2019,2020,2021 Tomas Prerovsky (cepsdev@hotmail.com).

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
#include "include_gen/ceps.tab.hh"
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
		symbol, /*27*/
		loop,
		for_loop_head,
		nodeset,
		nodeset_path_expr,
		template_definition,
		template_id,
		ifelse,
		ret,
		byte_array,
        error,
        undef,
        none,
		macro_definition,
		algorithm_definition,
		label,
		let,
		undefined = 4999,
		user_defined = 5000
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
		static const bool yes = false;
	};

/**
 * Macro which saves us from typeing to much.
 */
#define AST_NODE_IS_LEAF(kind_name) template<>\
																		struct type_map_is_leaf<Ast_node_kind::kind_name>\
																			{\
																				static const bool yes = true;\
																			};

#define AST_NODE_IS_NON_LEAF(kind_name)

/**
 * Strings,integers and floats have no child nodes.
 */
AST_NODE_IS_LEAF(string_literal)
AST_NODE_IS_LEAF(int_literal)
AST_NODE_IS_LEAF(float_literal)
AST_NODE_IS_LEAF(long_literal)
AST_NODE_IS_LEAF(unsigned_long_literal)

/**
 * All AST nodes are derived from Nodebase
 */
class Nodebase
{
public:
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
        virtual void print_content(std::ostream& ,bool pretty_print = false,int indent = 0) const
		{
		}
        virtual void print_value(std::ostream& ) const
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
	bool owns_children_ = true;

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

	bool owns_children () const {return owns_children_ ;}
	bool& owns_children () {return owns_children_ ;}

	virtual ~Nonleafbase()
	{

		if (owns_children_ ) for(Nodebase_ptr p : children())
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
			out << x<< "";Base::print_value(out);
		}

		Nodebase* clone()
		{
			return new This_type(*this);
		}
	};


template <Ast_node_kind what,typename... rest>
	struct ast_node<what,std::vector<Nodebase_ptr>,rest...>: public ast_node<what,rest...>
	{
		using T = std::vector<Nodebase_ptr>;
		T x;
		using Base = ast_node<what,rest...>;
		using This_type = ast_node<what,std::vector<Nodebase_ptr>,rest...>;
		ast_node(T val_1,rest... args,Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
			: Base(args...,child1,child2,child3),x(val_1)
			{

			}
		void print_content(std::ostream& out,bool pretty_print,int indent) const override
			{
			 Base::print_content(out,pretty_print,indent);
			}


		virtual void print_value(std::ostream& out) const override
		{
			Base::print_value(out);
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
			out <<"\""<< x<< "\"";Base::print_value(out);
		}

		Nodebase* clone()
		{
			return new This_type(*this);
		}

	};

template <Ast_node_kind what,typename... rest>
	struct ast_node<what,std::vector<std::string>,rest...>: public ast_node<what,rest...>
	{
		using T = std::vector<std::string>;
		T x;
		using Base = ast_node<what,rest...>;
		using This_type = ast_node<what,std::vector<std::string>,rest...>;
		ast_node(T val_1,rest... args,Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
			: Base(args...,child1,child2,child3),x(val_1)
			{

			}
		void print_content(std::ostream& out,bool pretty_print,int indent) const override
			{
				for(auto const & s : x)
				{
					out << "\""<< s <<"\" ";
				}
				Base::print_content(out,pretty_print,indent);
			}


		virtual void print_value(std::ostream& out) const override
		{
			for(auto const & s : x)
			{
				out << "\""<< s <<"\" ";
			}
			Base::print_value(out);
		}

		Nodebase* clone()
		{
			return new This_type(*this);
		}

	};

template <Ast_node_kind what,typename... rest>
	struct ast_node<what,std::vector<unsigned char>,rest...>: public ast_node<what,rest...>
	{
		using T = std::vector<unsigned char>;
		T x;
		using Base = ast_node<what,rest...>;
		using This_type = ast_node<what,std::vector<unsigned char>,rest...>;
		ast_node(T val_1,rest... args,Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
			: Base(args...,child1,child2,child3),x(val_1)
			{

			}
		void print_content(std::ostream& out,bool pretty_print,int indent) const override
			{
				for(auto const & s : x)
				{
					out << " "<< (int)s;
				}
				Base::print_content(out,pretty_print,indent);
			}


		virtual void print_value(std::ostream& out) const override
		{
			for(auto const & s : x)
			{
				out << " "<< (int)s;
			}
			Base::print_value(out);
		}

		Nodebase* clone()
		{
			return new This_type(*this);
		}

	};

template <typename... rest>
	struct ast_node<Ast_node_kind::binary_operator,int,rest...>: public ast_node<Ast_node_kind::binary_operator,rest...>
	{
		using T = int;
		T x;
		using Base = ast_node<Ast_node_kind::binary_operator,rest...>;
		using This_type = ast_node<Ast_node_kind::binary_operator,int,rest...>;
		ast_node(T val_1,rest... args,Nodebase_ptr child1=nullptr,Nodebase_ptr child2=nullptr,Nodebase_ptr child3=nullptr)
			: Base(args...,child1,child2,child3),x(val_1)
			{

			}
		void print_content(std::ostream& out,bool pretty_print,int indent) const override
			{
			    if (x < ceps::Cepsparser::token::DOTDOT)
				 out << (char)x << " ";
			    else if (x == ceps::Cepsparser::token::DOTDOT)
			     out << ".." << " ";
			    else if (x == ceps::Cepsparser::token::REL_OP_EQ)
			     out << "==" << " ";
			    else if (x == ceps::Cepsparser::token::REL_OP_NEQ)
			   	 out << "!=" << " ";
			    else if (x == ceps::Cepsparser::token::REL_OP_GT)
			   	 out << ">" << " ";
			    else if (x == ceps::Cepsparser::token::REL_OP_LT)
			   	 out << "<" << " ";
			    else if (x == ceps::Cepsparser::token::REL_OP_GT_EQ)
			   	 out << ">=" << " ";
			    else if (x == ceps::Cepsparser::token::REL_OP_LT_EQ)
			   	 out << "<=" << " ";
			    Base::print_content(out,pretty_print,indent);
			}


		virtual void print_value(std::ostream& out) const override
		{
			if (x < ceps::Cepsparser::token::DOTDOT)
			 out << (char)x << " ";
			else if (x == ceps::Cepsparser::token::DOTDOT)
			 out << ".." << "";
		    else if (x == ceps::Cepsparser::token::REL_OP_EQ)
		     out << "==" << " ";
		    else if (x == ceps::Cepsparser::token::REL_OP_NEQ)
		   	 out << "!=" << " ";
		    else if (x == ceps::Cepsparser::token::REL_OP_GT)
		   	 out << ">" << " ";
		    else if (x == ceps::Cepsparser::token::REL_OP_LT)
		   	 out << "<" << " ";
		    else if (x == ceps::Cepsparser::token::REL_OP_GT_EQ)
		   	 out << ">=" << " ";
		    else if (x == ceps::Cepsparser::token::REL_OP_LT_EQ)
		   	 out << "<=" << " ";

			Base::print_value(out);
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
typedef ast_node<Ast_node_kind::binary_operator,int> Binary_operator;
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

typedef ast_node<Ast_node_kind::template_definition,std::string,std::vector<std::string>> Template_defintion;
typedef ast_node<Ast_node_kind::template_id,std::string> Template_id;
typedef ast_node<Ast_node_kind::ifelse> Ifelse;
typedef ast_node<Ast_node_kind::ret> Return;
typedef ast_node<Ast_node_kind::byte_array, std::vector<unsigned char> > Byte_array;

typedef ast_node<Ast_node_kind::macro_definition,std::string /*name*/, void* /*symbol entry*/, std::vector<Nodebase_ptr> /* attributes */> Macrodef;
typedef ast_node<Ast_node_kind::label,std::string /*name*/, std::vector<Nodebase_ptr> /* attributes */,  void* /*symbol entry*/> Label;
typedef ast_node<Ast_node_kind::let,std::string /*lhs*/> Let;

typedef ast_node<Ast_node_kind::error, std::string , int , void* > Error;
typedef ast_node<Ast_node_kind::undef> Undefined;
typedef ast_node<Ast_node_kind::none> None;

typedef ast_node<Ast_node_kind::user_defined,int,void*> User_defined;






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


typedef ast_node<Ast_node_kind::error, std::string , int , void* > Error;


///////////// is_* predicates


template<ceps::ast::Ast_node_kind k> bool is(Nodebase_ptr p) {
	return p->kind() == k;
}




inline bool is_binop(Nodebase_ptr p)
{
 return p->kind() == ceps::ast::Ast_node_kind::binary_operator;
}

inline bool is_a_symbol(Nodebase_ptr p)
{
 return p->kind() == ceps::ast::Ast_node_kind::symbol;
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

inline bool is_a_byte_array(Nodebase_ptr p)
{
 return p->kind() == ceps::ast::Ast_node_kind::byte_array;
}

inline bool is_an_error(Nodebase_ptr p)
{
 return p->kind() == ceps::ast::Ast_node_kind::error;
}

inline bool is_a_string(Nodebase_ptr p)
{
 return p->kind() == ceps::ast::Ast_node_kind::string_literal;
}

inline bool is_a_let(Nodebase_ptr p)
{
 return p->kind() == ceps::ast::Ast_node_kind::let;
}
























/////////// as_* casts


inline Let* as_let_ptr(Nodebase_ptr p)
 {
	return static_cast<Let*>(p);
 }
inline Let& as_let_ref(Nodebase_ptr p)
 {
	return *static_cast<Let*>(p);
 }

inline Unary_operator* as_unary_op_ptr(Nodebase_ptr p)
 {
	return static_cast<Unary_operator*>(p);
 }
inline Unary_operator& as_unary_op_ref(Nodebase_ptr p)
 {
	return *static_cast<Unary_operator*>(p);
 }

inline Label* as_label_ptr(Nodebase_ptr p)
 {
	return static_cast<Label*>(p);
 }
inline Label& as_label_ref(Nodebase_ptr p)
 {
	return *static_cast<Label*>(p);
 }

inline Valdef* as_valdef_ptr(Nodebase_ptr p)
 {
	return static_cast<Valdef*>(p);
 }
inline Valdef& as_valdef_ref(Nodebase_ptr p)
 {
	return *static_cast<Valdef*>(p);
 }

inline Macrodef* as_macrodef_ptr(Nodebase_ptr p)
 {
	return static_cast<Macrodef*>(p);
 }
 inline Macrodef & as_macrodef_ref(Nodebase_ptr p)
 {
	return *as_macrodef_ptr(p);
 }


inline Nodebase_ptr func_call_target(Func_call& fc){
	return fc.children()[0];
} 

inline Func_call& as_func_call_ref(Nodebase_ptr p){
	return *dynamic_cast<ceps::ast::Func_call*>(p);
}

inline Error* as_error_ptr(Nodebase_ptr p)
{
 return dynamic_cast<Error*>(p);
}
inline Error & as_error_ref(Nodebase_ptr p)
{
 return *as_error_ptr(p);
}


inline Byte_array* as_byte_array_ptr(Nodebase_ptr p)
{
 return dynamic_cast<Byte_array*>(p);
}
inline Byte_array & as_byte_array_ref(Nodebase_ptr p)
{
 return *as_byte_array_ptr(p);
}

inline Return* as_return_ptr(Nodebase_ptr p)
 {
	return dynamic_cast<Return*>(p);
 }
 inline Return & as_return_ref(Nodebase_ptr p)
 {
	return *as_return_ptr(p);
 }


 inline User_defined* as_user_defined_ptr(Nodebase_ptr p)
  {
 	return dynamic_cast<User_defined*>(p);
  }
 inline User_defined & as_user_defined_ref(Nodebase_ptr p)
  {
 	return *as_user_defined_ptr(p);
  }

inline Ifelse* as_ifelse_ptr(Nodebase_ptr p)
 {
	return dynamic_cast<Ifelse*>(p);
 }
 inline Ifelse & as_ifelse_ref(Nodebase_ptr p)
 {
	return *as_ifelse_ptr(p);
 }

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
 	return *static_cast<Identifier_ptr>(p);
  }

 inline Identifier const  & as_id_ref_const(Nodebase_ptr p)
   {
  	return *dynamic_cast<Identifier const *>(p);
   }

 inline Template_defintion* as_template_definition_ptr(Nodebase_ptr p)
 {
   return dynamic_cast<Template_defintion*>(p);
 }

 inline Template_defintion& as_template_definition_ref(Nodebase_ptr p)
 {
   return *as_template_definition_ptr(p);
 }

 inline Template_id* as_template_id_ptr(Nodebase_ptr p)
 {
   return dynamic_cast<Template_id*>(p);
 }

 inline Template_id& as_template_id_ref(Nodebase_ptr p)
 {
   return *as_template_id_ptr(p);
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
 	return static_cast<Struct_ptr>(p);
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
   	return *static_cast<Symbol*>(p);
  }


 inline Symbol* as_symbol_ptr(Nodebase_ptr p)
  {
   	return static_cast<Symbol*>(p);
  }


  inline Loop_head & as_loop_head_ref(Nodebase_ptr p)
  {
   	return *static_cast<Loop_head_ptr>(p);
  }

  inline Loop_head const & as_loop_head_ref_const(Nodebase_ptr p)
   {
    	return *static_cast<Loop_head_ptr>(p);
   }

  inline Loop_head_ptr as_loop_head_ptr(Nodebase_ptr p)
  {
   	return static_cast<Loop_head_ptr>(p);
  }

 inline Ast_nodeset & as_ast_nodeset_ref(Nodebase_ptr p)
  {
   	return *static_cast<Ast_nodeset_ptr>(p);
  }


 inline Ast_nodeset_ptr as_ast_nodeset_ptr(Nodebase_ptr p)
  {
	  return static_cast<Ast_nodeset_ptr>(p);
  }

 inline Scope & as_scope_ref(Nodebase_ptr p)
 {
  	return *static_cast<Scope*>(p);
 }


 inline Scope* as_scope_ptr(Nodebase_ptr p)
 {
   return dynamic_cast<Scope*>(p);
 }

inline Kinddef & as_kinddef_ref(Nodebase_ptr p)
 {
  	return *static_cast<Kinddef*>(p);
 }


 inline Kinddef * as_kinddef_ptr(Nodebase_ptr p)
 {
   return static_cast<Kinddef*>(p);
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




template<typename T> auto get0th(T & x) -> typename getNth_type<0,T>::type
		{
			return get<0>(x);
		}


// Binary Operator 

inline getNth_type<0,  Binary_operator >::type & op(Binary_operator& x)
{
	return get<0>(x);
}

inline std::string op_val(Binary_operator& x){
	if(op(x) == '.') return ".";
	if(op(x) == '+') return "+";
	if(op(x) == '-') return "-";
	if(op(x) == '*') return "*";
	if(op(x) == '/') return "/";
	if(op(x) == '%') return "%";
	if(op(x) == '!') return "!";
	if(op(x) == '~') return "~";
	if(op(x) == '=') return "=";
	if(op(x) == '>') return ">";
	if(op(x) == '<') return "<";
	if(op(x) == '&') return "&&";
    if(op(x) == '|') return "||";

	if(op(x) == ceps::Cepsparser::token::DOTDOT) return "..";
	if(op(x) == ceps::Cepsparser::token::NOT) return "!";
	if(op(x) == ceps::Cepsparser::token::REL_OP_EQ) return "==";
	if(op(x) == ceps::Cepsparser::token::REL_OP_GT) return ">";
    if(op(x) == ceps::Cepsparser::token::REL_OP_LT) return "<";
    if(op(x) == ceps::Cepsparser::token::REL_OP_GT_EQ) return ">=";
	if(op(x) == ceps::Cepsparser::token::REL_OP_LT_EQ) return "<=";
	if(op(x) == ceps::Cepsparser::token::REL_OP_NEQ) return "!=";

	if ((unsigned int)op(x) <= 255){
	 std::string t;
	 t.push_back(op(x));
	 return t;
	}
	return ""; 
}  

// Unary Operator

inline getNth_type<0,  Unary_operator >::type & op(Unary_operator& x)
{
	return get<0>(x);
}

// User Defined

inline getNth_type<0,  User_defined >::type & id(User_defined& x)
{
	return get<0>(x);
}

// Label

inline getNth_type<0,  Label >::type & name(Label& x)
{
	return get<0>(x);
}

inline getNth_type<1,  Label >::type & attributes(Label& x)
{
	return get<1>(x);
}

inline getNth_type<2,  Label >::type & tag_data(Label& x)
{
	return get<2>(x);
}

// Struct

inline getNth_type<0,  Struct >::type & name(Struct& x)
{
	return get<0>(x);
}

// Identifier

inline getNth_type<0,  Identifier>::type & name(Identifier& x)
{
	return get<0>(x);
}

// String

inline getNth_type<0,  String >::type & value(String& x)
{
	return get<0>(x);
}

// Double

inline getNth_type<0, Double >::type & value(Double& x)
{
	return get<0>(x);
}

inline getNth_type<0, Double>::type & neg(Double& x)
{
	return get<0>(x) =  - get<0>(x);
}

inline getNth_type<1, Double>::type & unit(Double& x)
{
	return get<1>(x);
}

// Int

inline getNth_type<0, Int>::type & value(Int& x)
{
	return get<0>(x);
}

inline getNth_type<0, Int>::type & neg(Int& x)
{
	return get<0>(x) =  - get<0>(x);
}

inline getNth_type<1, Int>::type &  unit(Int& x)
{
	return get<1>(x);
}


// Int64

inline getNth_type<0, Int64>::type & value(Int64& x)
{
	return get<0>(x);
}

inline getNth_type<0, Int64>::type & neg(Int64& x)
{
	return get<0>(x) =  - get<0>(x);
}

inline getNth_type<1, Int64>::type &  unit(Int64& x)
{
	return get<1>(x);
}

// Valdef

inline getNth_type<0,  Valdef>::type & name(Valdef& x)
{
	return get<0>(x);
}

// Symbol

inline getNth_type<0,  Symbol >::type & name(Symbol& x)
{
	return get<0>(x);
}

inline getNth_type<1,  Symbol >::type & kind(Symbol& x)
{
	return get<1>(x);
}

// Ast_nodeset

inline getNth_type<0,  Ast_nodeset >::type & apply_idx_op_operand(Ast_nodeset& x)
{
	return get<0>(x);
}

// Byte_array

inline getNth_type<0,  Byte_array >::type & bytes(Byte_array& x)
{
	return get<0>(x);
}

// Error

inline getNth_type<0,  Error >::type & err_msg(Error& x)
{
        return get<0>(x);
}

inline getNth_type<1,  Error >::type & err_code(Error& x)
{
        return get<1>(x);
}

inline getNth_type<2,  Error >::type & err_tag_data(Error& x)
{
        return get<2>(x);
}

// Macrodef

inline getNth_type<0,  Macrodef >::type & name(Macrodef& x)
{
	return get<0>(x);
}

inline getNth_type<1,  Macrodef >::type & symbol_table_info(Macrodef& x)
{
	return get<1>(x);
}

inline getNth_type<2,  Macrodef >::type & attributes(Macrodef& x)
{
	return get<2>(x);
}


// Let

inline getNth_type<0,  Let >::type & name(Let& x)
{
	return get<0>(x);
}

// Kinddef

inline getNth_type<0,  Kinddef >::type & kind(Kinddef& x)
{
	return get<0>(x);
}

inline std::pair<bool,int> is_int(Nodebase_ptr p)
 {
	  auto i = 0;
	  auto r = p->kind() == ceps::ast::Ast_node_kind::int_literal;
	  if (r) i = ceps::ast::value(ceps::ast::as_int_ref(p));
 	  return std::make_pair(r,i);
 }

void flatten_func_args(ceps::ast::Nodebase_ptr r, std::vector<ceps::ast::Nodebase_ptr>& v);
inline bool is_a_funccall(Nodebase_ptr p,std::string& func_id,std::vector<ceps::ast::Nodebase_ptr>& args)
  {
	  auto r = p->kind() == ceps::ast::Ast_node_kind::func_call;
	  if (!r) return r;
	  ceps::ast::Func_call& func_call = *dynamic_cast<ceps::ast::Func_call*>(p);
	  ceps::ast::Identifier& id = *dynamic_cast<ceps::ast::Identifier*>(func_call.children()[0]);
	  func_id = name(id);
	  if (nlf_ptr(func_call.children()[1])->children().size()){
		  flatten_func_args(nlf_ptr(func_call.children()[1])->children()[0],args);
	  }
	  return r;
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
struct ident{
	std::string v;
	ident(std::string s):v{s}{}
	ident() = default;
};

Nodebase_ptr box(ident const & s);


Struct_ptr make_struct(std::string const & name);

template<typename... Ts>
	Struct_ptr make_struct(std::string const & name, ident i,Ts const &... args)
	{

		Struct_ptr p =  make_struct(name,args...);
		p->children().insert(
				p->children().begin(),
				new Identifier{i.v});
		return p;
	}

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
Nodebase_ptr read_xml_file(std::string path);

}//namespace ast
}//namespace ceps

#endif
