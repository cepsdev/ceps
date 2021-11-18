/*
Copyright 2021 Tomas Prerovsky (cepsdev@hotmail.com).

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


#include "cepsruntime.hh"
#include <memory>

//UNIX Headers
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WIN32

#else

#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#endif

const ceps::Ceps_ostream::endl_ ceps::Ceps_ostream::endl;
bool ceps::INTERNAL_KILL_SIGNAL_RECEIVED = false;
ceps::Ceps_Environment* ceps::CURRENT_ENV_INSTANCE = nullptr;




std::string ceps::get_version_info()
{
	std::stringstream ss;
     #ifdef __GNUC__
	 		ss
				<< "VERSION 1.1 (" __DATE__ << ") BUILT WITH GCC "<< "" __VERSION__ ""<< " on GNU/LINUX "
	 #ifdef __LP64__
					  << "64BIT"
	 #else
					  << "32BIT"
	 #endif
					  << " (C) BY THE AUTHORS OF ceps";
	#else
		#ifdef _MSC_FULL_VER
			ss
				<< "VERSION 1.1 (" __DATE__ << ") BUILT WITH MS VISUAL C++ " << _MSC_FULL_VER << " on Windows "
			#ifdef _WIN64
				<< "64BIT"
			#else
				<< "32BIT"
			#endif
				<< " (C) BY THE AUTHORS OF ceps";
		#endif
	#endif
	ss << " (ceps is hosted at github: https://github.com/cepsdev/ceps.git) ";
	return ss.str();
}

void ceps::kill()
{
	//std::cout << "\nINTERNAL_KILL_SIGNAL_RECEIVED [1]" << std::endl;
	INTERNAL_KILL_SIGNAL_RECEIVED = true;
	if (!CURRENT_ENV_INSTANCE)
		return;
	CURRENT_ENV_INSTANCE->get_fragment_queue().signal_all();
	//std::cout << "\nINTERNAL_KILL_SIGNAL_RECEIVED [2]" << std::endl;
}


#ifdef _WIN32

void ceps::Ceps_ostream::write(std::string const& fragment)
{
	throw std::runtime_error("Not Implemented.");
}

#else

void ceps::Ceps_ostream::write(std::string const& fragment)
{
	sockaddr_un addr;
	int sfd;

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1)
		throw std::runtime_error(__FILE__+std::string(": socket allocation failed."));

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_path_.c_str(), sizeof(addr.sun_path) - 1);

	if (connect(sfd, (struct sockaddr *) &addr,sizeof(struct sockaddr_un)) == -1)
		throw std::runtime_error(__FILE__+std::string(": connect failed."));

	::write(sfd, fragment.c_str(), fragment.size());

	close(sfd);
}

#endif

void ceps::ceps_main(ceps::Ceps_Environment& env)
{
		CURRENT_ENV_INSTANCE = &env;
		//std::cout << "\nYaMDL Runtime up." << std::endl;

		for(;!INTERNAL_KILL_SIGNAL_RECEIVED;)
		{
			try{

				std::string current_fragment;

				env.get_fragment_queue().pop(current_fragment);
				if (current_fragment.length() == 0) continue;

				//std::cout << "frag:" << current_fragment << std::endl;

				ceps::ast::Parsetree pt;
				std::stringstream is{current_fragment};
				{
					std::lock_guard<std::mutex> lk{env.mut_};
					Ceps_parser_driver driver(env.get_global_symboltable(),is);
					ceps::Cepsparser parser(driver);

					if (parser.parse() != 0)
						continue;
					if (driver.errors_occured())
					 continue;
					pt = driver.parsetree();
				}
				env.eval_and_merge( pt , false);
			} catch (ceps::interpreter::semantic_exception & se)
			{
				std::cerr << "[ERROR][YaMDL Main Thread: Interpreter]:"<< se.what() << std::endl;
			}
			catch (std::runtime_error & re)
			{
				std::cerr << "[ERROR][YaMDL Main Thread]:"<< re.what() << std::endl;
			}
		}//for
		//std::cout << "\nYaMDL Runtime down." << std::endl;
		exit(0);
}


void ceps::ceps_main_strct_consumer(ceps::Ceps_Environment& env)
{
		CURRENT_ENV_INSTANCE = &env;
		//std::cout << "\nYaMDL Runtime up." << std::endl;
		for(;!INTERNAL_KILL_SIGNAL_RECEIVED;)
		{

			try{
				ceps::ast::Struct* current_strct;
				env.get_fragment_queue().pop(current_strct);
				if (current_strct == nullptr) continue;
				ceps::ast::Root* root_p = new ceps::ast::Root;
				ceps::ast::Parsetree pt{root_p};
				root_p->children().push_back(current_strct);
				env.eval_and_merge( pt , false, true);
			} catch (ceps::interpreter::semantic_exception & se)
			{
				std::cerr << "[ERROR][YaMDL Main Thread: Interpreter]:"<< se.what() << std::endl;
			}
			catch (std::runtime_error & re)
			{
				std::cerr << "[ERROR][YaMDL Main Thread]:"<< re.what() << std::endl;
			}
		}//for
		//std::cout << "\nYaMDL Runtime down." << std::endl;
		exit(0);
}



void ceps::ceps_start( void (*func) (ceps::ast::Nodeset&),
						 ceps::Ceps_Environment* ceps_env,
						 int intervall_u_sec)
{
	if (func == nullptr)
		return;
	for(;!INTERNAL_KILL_SIGNAL_RECEIVED;)
	{
		using namespace ceps::parsetree;
#ifdef _WIN32
#else
		usleep(intervall_u_sec < 0 ? 10 : intervall_u_sec); // sleep 10 msec.
#endif

		if(intervall_u_sec < 0 && !ceps_env->get_and_reset_signal_state())
			continue;

		try
		{
			std::lock_guard<std::mutex> lk{ceps_env->mut_};
			ceps::ast::Nodeset ns{ceps_env->get_universum()};
			func(ns);
		} catch(...)
		{
			//TODO: Logging etc.
		}
	}
}


void ceps::Ceps_Environment::eval_and_merge(ceps::ast::Parsetree pt,bool ignore_predicates,bool dont_evaluate)
{
	std::lock_guard<std::mutex> lk{mut_};
	ceps::ast::Nodebase_ptr p = dont_evaluate ? pt.get_root(): 
	 ceps::interpreter::evaluate_generic(pt.get_root(),symboltable_,interpreter_env(),nullptr,nullptr,nullptr,ceps::interpreter::thoroughness_t::normal);
	if (p == nullptr) return;
	if (!ignore_predicates && !signal_)
	{
		if (predicates_.size() == 0)
			signal_ = true;
		else for(auto pred : predicates_)
		{
			if ( (signal_ = pred(ceps::ast::Nodeset{p})) )
				break;

		}
	}
	universum_->children().insert(universum_->children().end(),
								 ceps::ast::nlf_ptr(p)->children().begin(),
								 ceps::ast::nlf_ptr(p)->children().end());

	if (unique_structs_.size() == 0)
		return;
	for (auto & elem :unique_structs_ )
		elem.second = 0;
	for(ceps::ast::Nodebase_ptr pnode : universum_->children())
	{
		if (pnode->kind() != ceps::ast::Ast_node_kind::structdef) continue;
		ceps::ast::Struct_ptr sp = as_struct_ptr(pnode);
		std::map<std::string,int>::iterator it = unique_structs_.find(ceps::ast::name(*sp));
		if (it == unique_structs_.end())
			continue;
		++it->second;
	}
	auto pend = std::remove_if(universum_->children().begin(),universum_->children().end(),
			[&](ceps::ast::Nodebase_ptr const & pnode)
			  {
				if (pnode == nullptr) return true;
				if (pnode->kind() != ceps::ast::Ast_node_kind::structdef) return false;
				ceps::ast::Struct_ptr sp = as_struct_ptr(pnode);
				std::map<std::string,int>::iterator it = unique_structs_.find(ceps::ast::name(*sp));
				if (it == unique_structs_.end())
					return false;
				if (it->second > 1)
				{
					--it->second;
					return true;
				}
				return false;

			  }
			 );
	universum_->children().erase(pend,universum_->children().end());
}




void ceps::execute_script(std::istream & script,
		                   void (*callback)(ceps::ast::Nodeset &),
		                   void (*prologue)(Ceps_Environment&) )
{
	ceps::Ceps_Environment ceps_env{""};
	try{
		if (prologue != nullptr)
			prologue(ceps_env);

		Ceps_parser_driver driver{ceps_env.get_global_symboltable(),script};
		ceps::Cepsparser parser{driver};

		if (parser.parse() != 0)
			throw std::runtime_error{""};

		if (driver.errors_occured())
			throw std::runtime_error{""};

		ceps::ast::Nodebase_ptr p = ceps::interpreter::evaluate_generic(
			driver.parsetree().get_root(),
			ceps_env.get_global_symboltable(),
			ceps_env.interpreter_env(),
			nullptr,nullptr,nullptr,ceps::interpreter::thoroughness_t::normal);
			
		ceps::ast::Nodeset ns{p};
		callback(ns);
	} catch (ceps::interpreter::semantic_exception & se)
	{
		std::cerr << "[ERROR][Interpreter]:"<< se.what() << std::endl;
		throw se;
	}
	catch (std::runtime_error & re)
	{
		std::cerr << "[ERROR][System]:"<< re.what() << std::endl;
		throw re;
	}
}



