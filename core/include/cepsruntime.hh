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

#ifndef CEPSRUNTIME_H_
#define CEPSRUNTIME_H_

#include <vector>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <map>

#include "symtab.hh"
#include "cepsparserdriver.hh"
#include "ceps_ast.hh"
#include "cepsnodeset.hh"
#include "ceps_interpreter.hh"

namespace ceps
{
	extern std::string get_version_info();
	extern bool INTERNAL_KILL_SIGNAL_RECEIVED;


	void kill();

	class Ceps_ostream
		{

			std::string socket_path_;
			std::vector<std::string> stack_;

		public:
			struct endl_{};
			static const endl_ endl;

			Ceps_ostream(std::string const & socket_path):socket_path_(socket_path){}
			void write(std::string const& fragment);

			Ceps_ostream&  operator << (std::string const& s)
			{
				stack_.push_back(s);
				return *this;
			}

			Ceps_ostream&  operator << (double v)
			{
				std::stringstream ss; ss << v;
				stack_.push_back(ss.str());
				return *this;
			}

			Ceps_ostream&  operator << (Ceps_ostream::endl_ const &)
			{
				std::string o;
				for(std::string const & s : stack_)
					o += s;
				stack_.clear();
				write(o);
				return *this;
			}

		};



	template <typename T>
	class Queue_thread_safe
	{
		mutable std::mutex mut;
		std::queue<T> data_queue;
		std::condition_variable data_cond;
	public:
		Queue_thread_safe()
		{}
		Queue_thread_safe(Queue_thread_safe const & other)
		{
			std::lock_guard<std::mutex> lk{other.mut};
			data_queue = other.data_queue;
		}
		void push(T new_value)
		{
			std::lock_guard<std::mutex> lk{mut};
			data_queue.push(new_value);
			data_cond.notify_one();
		}
		void wait_and_pop(T& value)
		{
			std::unique_lock<std::mutex> lk{mut};
			data_cond.wait(lk,[this]{return !data_queue.empty() || ceps::INTERNAL_KILL_SIGNAL_RECEIVED;});
			if (INTERNAL_KILL_SIGNAL_RECEIVED){value=T{}; return;}
			value = data_queue.front();
			data_queue.pop();
		}

		std::shared_ptr<T> wait_and_pop()
		{
			std::unique_lock<std::mutex> lk{mut};
			data_cond.wait(lk,[this]{return !data_queue.empty() || ceps::INTERNAL_KILL_SIGNAL_RECEIVED;});
			if (INTERNAL_KILL_SIGNAL_RECEIVED)
				return nullptr;
			std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
			data_queue.pop();
			return res;
		}

		bool try_pop(T& value)
		{
			std::lock_guard<std::mutex> lk{mut};
			if(data_queue.empty())
				return false;
			value = data_queue.front();
			data_queue.pop();
			return true;
		}

		std::shared_ptr<T> try_pop()
		{
			std::lock_guard<std::mutex> lk{mut};
			if(data_queue.empty())
				return std::shared_ptr<T>{};
			std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
			data_queue.pop();
			return res;
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> lk{mut};
			return data_queue.empty();
		}

		void signal()
		{
			data_cond.notify_all();
		}

	};


	class Ceps_raw_channel
	{
		Queue_thread_safe<std::string> plain_fragments_;
		Queue_thread_safe<ceps::ast::Struct*> interpreted_structs_;

		Queue_thread_safe<std::string> & get_msg_queue()
		{
			return plain_fragments_;
		}

		Queue_thread_safe<std::string> const & get_msg_queue() const
		{
			return plain_fragments_;
		}

		Queue_thread_safe<ceps::ast::Struct*> & get_strct_queue()
		{
				return interpreted_structs_;
		}

		Queue_thread_safe<ceps::ast::Struct*> const & get_strct_queue() const
		{
				return interpreted_structs_;
		}

	public:
		void push(std::string const & msg)
		{
			get_msg_queue().push(msg);
		}

		void pop(std::string& msg)
		{
			get_msg_queue().wait_and_pop(msg);
		}

		void push(ceps::ast::Struct* strct)
		{
			//@TODO: use shared pointer
			get_strct_queue().push(static_cast<ceps::ast::Struct*>(strct->clone()));
		}

		void pop(ceps::ast::Struct*& strct)
		{
			get_strct_queue().wait_and_pop(strct);
		}

		void signal_all()
		{
			get_msg_queue().signal();
			get_strct_queue().signal();
		}


	};

	class Ceps_Environment
	{
	public:

	private:
		Ceps_raw_channel fragment_channel_;
		ceps::parser_env::Symboltable symboltable_;
		std::vector<ceps::ast::Parsetree> parsetrees_uninterpreted_;
		/**
		 * predicates_ stores a list of functions with signature Nodeset --> bool
		 * After evaluation but before merging each predicate is evaluated until a
		 * predicate returns true, in this case the flag signal_ is set.
		 *
		 */
		std::vector<bool (*) (ceps::ast::Nodeset const &)> predicates_;
		
		
		ceps::ast::Root_ptr universum_;
		bool signal_ = false;
		
		std::map<std::string,int> unique_structs_;

		//binop overloading

		ceps::interpreter::Environment interp_env_;

	public :
		
		mutable std::mutex mut_;
		
		ceps::interpreter::Environment& interpreter_env()
		{
			return interp_env_;
		}

		Ceps_Environment()
			:fragment_channel_{},
			 symboltable_{},
			 universum_{new ceps::ast::Root{}}
		{
			symboltable_.push_scope();
		}

		Ceps_Environment(	std::string ceps_init,
							std::vector<bool (*) (ceps::ast::Nodeset const &)> const & pred_ = 
							std::vector<bool (*) (ceps::ast::Nodeset const &)>{} )
						:fragment_channel_(),
						 symboltable_(),
						 predicates_(pred_),
						 universum_(new ceps::ast::Root())
		{
			symboltable_.push_scope();
			std::stringstream is{ceps_init};
			Ceps_parser_driver driver(symboltable_,is);
			ceps::Cepsparser parser(driver);
			if (parser.parse() != 0)
				throw std::runtime_error{std::string{__FILE__}};
			eval_and_merge( driver.parsetree() );
		}

		Ceps_Environment(Ceps_Environment const &) = delete;
		Ceps_Environment& operator = (Ceps_Environment const &) = delete;
		Ceps_Environment(Ceps_Environment&& rhs) = default;

		void unique(std::string const & s)
		{
			unique_structs_.insert(std::make_pair(s,0));
		}
		Ceps_raw_channel& get_fragment_queue()
		{
			return fragment_channel_;
		}
		ceps::parser_env::Symboltable& get_global_symboltable()
		{
			return symboltable_;
		}

		void push_parsetree(ceps::ast::Parsetree pt)
		{
			std::lock_guard<std::mutex> lk{mut_};
			parsetrees_uninterpreted_.push_back(pt);
		}

		std::shared_ptr<ceps::ast::Parsetree> get_latest_uninterpreted_parsetree()
		{
			std::lock_guard<std::mutex> lk{mut_};
			if(parsetrees_uninterpreted_.empty())
				return std::shared_ptr<ceps::ast::Parsetree>{};
			std::shared_ptr<ceps::ast::Parsetree> res{std::make_shared<ceps::ast::Parsetree>(parsetrees_uninterpreted_.back())};
			parsetrees_uninterpreted_.pop_back();
			return res;

		}
		
		void eval_and_merge(ceps::ast::Parsetree pt,bool ignore_predicates = true,bool dont_evaluate = false);
		bool get_and_reset_signal_state()
		{
			std::lock_guard<std::mutex> lk{mut_};
			bool temp = signal_;
			signal_ = false;
			return temp;
		}

		ceps::ast::Root_ptr get_universum()
		{
			return universum_;
		}		
	};

	extern Ceps_Environment* CURRENT_ENV_INSTANCE;

	extern void ceps_main(Ceps_Environment& env);
	extern void ceps_main_strct_consumer(Ceps_Environment& env);
	extern void ceps_start( void (*func) (ceps::ast::Nodeset& ),
							 ceps::Ceps_Environment* ceps_env,
							 int intervall_u_sec );


	void execute_script(std::istream & script,
			            void (*callback)(ceps::ast::Nodeset &),
			            void (*)(Ceps_Environment&) = nullptr );
}

#endif /* CEPSRUNTIME_H_ */
