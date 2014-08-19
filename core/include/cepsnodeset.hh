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

#ifndef YAMDL_NODESET_H_
#define YAMDL_NODESET_H_

#include <iterator>
#include <set>
#include "ceps_ast.hh"

namespace ceps
 {
  namespace ast
  {
  
  
  

	class Nodeset;
	
	
	struct first
	{
		std::string struct_name_;
		first(std::string const & struct_name):struct_name_{struct_name}
		{}
	};
	struct last
	{
		std::string struct_name_;
		last(std::string const & struct_name):struct_name_{struct_name}
		{}
	};
	struct nth
	{
		std::string struct_name_;
		int i_;
		nth(std::string const & struct_name,int i):struct_name_{struct_name},i_{i}
		{}
		nth(int i):struct_name_{},i_{i}
		{}
	};

	template <typename F>
		struct filter_
		{
			std::string struct_name_;
			F f_;
			filter_(std::string const & struct_name, F f):struct_name_{struct_name},f_{f}
			{}
	
			bool operator () (Nodeset& n) { return f_(n);}
		};
	
	struct all
		{
			std::string struct_name_;
			all(std::string const & struct_name):struct_name_{struct_name}
			{}
		};
	
	template<typename F> filter_<F> filter(std::string const & s, F f)
			{
				return filter_<F>{s,f};
			}
	
	#define WHERE(x,y) [filter( (x), [](Nodeset const & current){return (current y) ; } )]
	#define SELECT(s) [(s)]
	#define FROM(x) (x)
	
	
	template<typename U>
	 SI::Quantity<U,double> get_double(Nodeset const & nset);
	template<typename U>
	 SI::Quantity<U,int> get_int(Nodeset const & nset);
	
	struct Nodeset
	{
	
		std::vector<Nodebase_ptr> nodes_;
		std::vector<Nodebase_ptr> parents_;


		using Containertype = std::vector<Nodebase_ptr>;
		
		struct Iterator : public std::iterator <std::bidirectional_iterator_tag,Nodeset>
		{
			using Baseiterator = Containertype::iterator;
			Baseiterator base_;
			explicit Iterator(Baseiterator baseiterator): base_{baseiterator}
				 {
				 }
			bool operator == (const Iterator & rhs) const
				{
				   return base_ == rhs.base_;
				}

			bool operator != (const Iterator & rhs) const
				{
				   return base_ != rhs.base_;
				}
			Iterator& operator ++()
				{
					++base_;return *this;
				}

			Iterator operator ++(int)
				{
					Iterator temp{base_};
					this->operator++();
					return temp;
				}
			Nodeset operator*()
			{
				return Nodeset{*base_};
			}

			Nodeset operator*() const
			{
				return Nodeset{*base_};
			}


			
		};
		
		Iterator begin()
		{
			return Iterator{nodes_.begin()};			
		}

		Iterator end()
		{
			return Iterator{nodes_.end()};			
		}
		
		Nodeset()
		{}
		Nodeset(std::vector<Nodebase_ptr> const & v):nodes_{v}
		{}
	
		Nodeset(Nodebase_ptr n)
		{
			if (n == nullptr)
				return;
			if (n->kind() == Ast_node_kind::root)
			{
				parents_.push_back(n);
				for(Nodebase_ptr p : nlf_ptr(n)->children())
					if (p) nodes_.push_back(p);
			}
			else nodes_.push_back(n);
		}
		Nodeset(strct const  & s):nodes_{s.p_strct}{}

		void erase()
		{
			if (nodes_.empty())
				return;
			if (parents_.empty())
			{
				for(auto e : nodes_)
					delete e;
				nodes_.clear();
			}

			std::set<Nodebase_ptr> to_erase(nodes_.begin(),nodes_.end());
			for(Nodebase_ptr p : parents_)
			{
				std::vector<Nodebase_ptr> temp;
				for(auto e: nlf_ptr(p)->children())
				{
					if (to_erase.find(e) != to_erase.end())
						continue;
					temp.push_back(e);
				}
				nlf_ptr(p)->children() = temp;
			}

			nodes_.clear();


			//for(auto e : to_erase)
				//delete e;
		}
	
	
		std::vector<Nodebase_ptr>& nodes() {return nodes_;}
		std::vector<Nodebase_ptr> const & nodes() const {return nodes_;}
	
		/*
		 * Returns number of nodes in nodeset.
		 * Exception: if nodeset contains on and only one rawmap or one and onl< atoms it returns the
		 * number of rows of the rawmap.
		 */
		size_t size() const {
			if (nodes().size() == 1 && (nodes()[0]->kind() == Ast_node_kind::rawmap || nodes()[0]->kind() == Ast_node_kind::atoms) )
			{
				return nlf_ptr(nodes()[0])->children().size();
			}
			return nodes().size();
		}
		
		bool empty() const {
			return size() == 0;
		}
	
	
	
		Nodeset operator [] (int idx) const {
			if (nodes().size() == 1 && (nodes()[0]->kind() == Ast_node_kind::rawmap || nodes()[0]->kind() == Ast_node_kind::vector) )
			{
				return Nodeset{nlf_ptr(nodes()[0])->children()[idx]};
			}
			return Nodeset{nodes()[idx]};
		}
	
		Nodeset operator [] (char const * sz) const {
			return operator[](std::string{sz});
		}
	
		Nodeset operator [] (std::string const & s) const {
			std::vector<Nodebase_ptr> temp;
			for(Nodebase_ptr p : nodes())
			{
				if (p->kind() == Ast_node_kind::structdef)
				{
					if(name(as_struct_ref(p)) == s)
						temp.insert(temp.end(), nlf_ptr(p)->children().begin(),nlf_ptr(p)->children().end() );
				}
			}
	
			return Nodeset{temp};
		}
	
		template <typename F> Nodeset operator [] (filter_<F> const & filt) const {
			std::vector<Nodebase_ptr> temp;
			for(Nodebase_ptr p : nodes())
			{
				if (p->kind() == Ast_node_kind::structdef)
				{
					if(name(as_struct_ref(p)) == filt.struct_name_ && filt.f_(Nodeset{nlf_ptr(p)->children()}))
						temp.insert(temp.end(), nlf_ptr(p)->children().begin(),nlf_ptr(p)->children().end() );
				}
			}
	
			return Nodeset{temp};
		}
	
		Nodeset operator [] (all const & filt) const {
				std::vector<Nodebase_ptr> temp;
				for(Nodebase_ptr p : nodes())
				{
					if (p->kind() == Ast_node_kind::structdef)
					{
						if(name(as_struct_ref(p)) == filt.struct_name_ )
							temp.push_back(p);
					}
				}
	
				return Nodeset{temp};
			}
	
		Nodeset operator [] (first const & s) const {
			std::vector<Nodebase_ptr> temp;
			for(Nodebase_ptr p : nodes())
			{
				if (p->kind() == Ast_node_kind::structdef)
				{
					if(name(as_struct_ref(p)) == s.struct_name_)
					{
						temp.insert(temp.end(), nlf_ptr(p)->children().begin(),nlf_ptr(p)->children().end() );
						break;
					}
				}
			}
	
			return Nodeset{temp};
		}
	
		Nodeset operator [] (last const & s) const {
			Nodebase_ptr result = nullptr;
			for(Nodebase_ptr p : nodes())
			{
				if (p->kind() == Ast_node_kind::structdef)
				{
					if(name(as_struct_ref(p)) == s.struct_name_)
						result = p;
				}
			}
			if (result == nullptr)
				return Nodeset{};
			std::vector<Nodebase_ptr> temp;
			temp.insert(temp.end(), nlf_ptr(result)->children().begin(),nlf_ptr(result)->children().end() );
			return Nodeset{temp};
		}
	
		Nodeset operator [] (nth const & s) const {
			Nodebase_ptr result = nullptr;
			size_t i = 0;
			if (s.struct_name_.length() == 0)
			{
				if (nodes().size() <= (size_t)s.i_)
					return Nodeset{};
				result = nodes()[s.i_];
			}
			else for(Nodebase_ptr p : nodes())
			{
				if (p->kind() == Ast_node_kind::structdef)
				{
					if(name(as_struct_ref(p)) == s.struct_name_)
					{
						if (i == (size_t)s.i_){ result = p;break;}
						++i;
					}
				}
			}
			if (result == nullptr)
				return Nodeset{};
			std::vector<Nodebase_ptr> temp;
			temp.insert(temp.end(), nlf_ptr(result)->children().begin(),nlf_ptr(result)->children().end() );
			return Nodeset{temp};
		}
	
		std::string as_str() const
		{
			if (size() != 1)
				ERROR("Cannot convert to string: too many / not enough elements in nodeset.");
			if (nodes_[0]->kind() != Ast_node_kind::string_literal)
				ERROR("Cannot convert to string: nodeset contains no string.");
			String & s_ref = as_string_ref(nodes_[0]);
			return value(s_ref);
		}
	
		int as_int() const
		{
	
			if (size() != 1)
				ERROR("Cannot convert to int.");
	
			if (nodes_[0]->kind() != Ast_node_kind::int_literal
				&&
				nodes_[0]->kind() != Ast_node_kind::float_literal
				)
			   ERROR("Cannot convert to int: no numerical value.");
	
			if (nodes_[0]->kind() == Ast_node_kind::int_literal)
			{
				if (unit(as_int_ref(nodes_[0])) != all_zero_unit())
					ERROR("Cannot convert to int: not a scalar.");
	
				return value(as_int_ref(nodes_[0]));
	
			}
			if (nodes_[0]->kind() == Ast_node_kind::float_literal)
			{
				if (unit(as_double_ref(nodes_[0])) != all_zero_unit())
					ERROR("Cannot convert to int: not a scalar.");
				return (int)value(as_double_ref(nodes_[0]));
			}
			ERROR("Cannot convert to int");
	
		}
	
		double as_double() const
		{
			if (size() != 1)
				ERROR("Cannot convert to double.");
			if (nodes_[0]->kind() != Ast_node_kind::int_literal
				&&
				nodes_[0]->kind() != Ast_node_kind::float_literal
				)
			   ERROR("Cannot convert to double: no numerical value.");
	
			if (nodes_[0]->kind() == Ast_node_kind::int_literal)
			{
				if (unit(as_int_ref(nodes_[0])) != all_zero_unit())
					ERROR("Cannot convert to double: not a scalar.");
	
				return (double)value(as_int_ref(nodes_[0]));
	
			}
			if (nodes_[0]->kind() == Ast_node_kind::float_literal)
			{
				if (unit(as_double_ref(nodes_[0])) != all_zero_unit())
					ERROR("Cannot convert to int: not a scalar.");
				return value(as_double_ref(nodes_[0]));
			}
			ERROR("Cannot convert to int");
		}
	
	
		operator std::string() const
		{
			return as_str();
		}
	
		operator int() const
		{
			return as_int();
		}
	
		operator double() const
		{
			return as_double();
		}
	
		template<typename U>
			SI::Quantity<U> operator [] (SI::Quantity<U,double>) const
			{
				return get_double<U>(*this);
			}
		template<typename U>
			SI::Quantity<U,int> operator [] (SI::Quantity<U,int>) const
			{
				return get_int<U>(*this);
			}
	
		Nodeset operator() (unsigned int i,unsigned int j) const
			{
				if ( nodes_.size() != 1)
					ERROR("To many nodes in nodeset");
				if (nodes_[0]->kind() != Ast_node_kind::rawmap)
					ERROR("Not a map");
				Rawmap & mp = as_rawmap_ref(nodes_[0]);
				if (i >= mp.children().size())
					ERROR("Indices out of range");
				if (j >= nlf_ptr(mp.children()[i])->children().size())
					ERROR("Indices out of range");
				return Nodeset{nlf_ptr(mp.children()[i])->children()[j]};
			}
	
	};

	using Nset = Nodeset;
	using Nset_checked = Nodeset;
	
	
	template<typename U>
	 SI::Quantity<U,double> get_double(Nset const & nset)
	 {
		if (nset.size() == 0)
			ERROR("Empty nodeset.");
		if (nset.size() > 1)
			ERROR("Too many elements in nodeset.");
		Nodebase_ptr node = nset.nodes()[0];
		if (node->kind() != Ast_node_kind::float_literal && node->kind() != Ast_node_kind::int_literal)
			ERROR("Element in nodeset has incompatible type.");
		if (node->kind() == Ast_node_kind::float_literal)
		{
		 Double & node_ref = as_double_ref(node);
		 if( Unit_rep{U::m,U::kg,U::s,U::A,U::K,U::mol,U::cd} != unit(node_ref) )
			 ERROR("Incompatible unit.");
		 return SI::Quantity<U,double>{value(node_ref)};
		}
		if (node->kind() == Ast_node_kind::int_literal)
		{
			 Int & node_ref = as_int_ref(node);
			 if( Unit_rep{U::m,U::kg,U::s,U::A,U::K,U::mol,U::cd} != unit(node_ref) )
				 ERROR("Incompatible unit.");
			 return SI::Quantity<U,double>{(double)value(node_ref)};
		}
	
	
		return SI::Quantity<U,double>{};
	 }
	
	template<typename U>
	 SI::Quantity<U,int> get_int(Nset const & nset)
	 {
		if (nset.size() == 0)
			ERROR("Empty nodeset.");
		if (nset.size() > 1)
			ERROR("Too many elements in nodeset.");
		Nodebase_ptr node = nset.nodes()[0];
		if (node->kind() != Ast_node_kind::float_literal && node->kind() != Ast_node_kind::int_literal)
			ERROR("Element in nodeset has incompatible type.");
		if (node->kind() == Ast_node_kind::float_literal)
		{
		 Double & node_ref = as_double_ref(node);
		 if( Unit_rep{U::m,U::kg,U::s,U::A,U::K,U::mol,U::cd} != unit(node_ref) )
			 ERROR("Incompatible unit.");
		 return SI::Quantity<U,int>{(int)value(node_ref)};
		}
		if (node->kind() == Ast_node_kind::int_literal)
		{
			 Int & node_ref = as_int_ref(node);
			 if( Unit_rep{U::m,U::kg,U::s,U::A,U::K,U::mol,U::cd} != unit(node_ref) )
				 ERROR("Incompatible unit.");
			 return SI::Quantity<U,int>{(int)value(node_ref)};
		}
	
	
		return SI::Quantity<U,int>{};
	 }
	
	/****************************** I/O *************************************/
	
	inline std::ostream& operator << (std::ostream & os,Nodeset const & ns)
	{
		for(Nodebase_ptr p : ns.nodes())
		{
			os << *p << std::endl;
		}
		return os;
	}
	

  }//namespace ast
}//namespace ceps

#endif
