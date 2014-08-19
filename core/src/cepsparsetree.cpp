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




#include "cepsparsetree.hh"


ceps::parsetree::Parsetrees ceps::parsetree::Parsetrees::operator[](ceps::parsetree::Node::Kind k) const
	  {
		  using ceps::parsetree::Node;
		  std::vector<Node*> temp;
		  for(std::vector<Node*>::const_iterator it = roots.begin();it!=roots.end();++it)
		  {
			  if(*it == NULL)
				  continue;
			  Parsetrees pts = (Parsetree(*it))[k];

			  temp.insert(temp.end(),pts.get_roots().begin(),pts.get_roots().end());

		  }
		  return Parsetrees(temp);
	  }




ceps::parsetree::Parsetrees ceps::parsetree::Parsetrees::operator[](ceps::parsetree::Ident identifier) const
	  {
		  using ceps::parsetree::Node;
		  std::vector<Node*> temp;
		  for(std::vector<Node*>::const_iterator it = roots.begin();it!=roots.end();++it)
		  {
			  if(*it == NULL)
				  continue;
			  for (Node::Iter it = (*it)->begin(); it != (*it)->end(); ++it)
			  {
				  if ( (*it)->kind != Node::identifier) continue;
				  ceps::parsetree::Identifier& id = *static_cast<ceps::parsetree::Identifier*>(*it);
				  if (id.get_name() == identifier.get_str())
					  temp.push_back(*it);
			  }

		  }
		  return Parsetrees(temp);
	  }


ceps::parsetree::Parsetrees ceps::parsetree::Parsetrees::operator[](ceps::parsetree::Structid identifier) const
	  {
		  using ceps::parsetree::Node;
		  std::vector<Node*> temp;
		  for(std::vector<Node*>::const_iterator it = roots.begin();it!=roots.end();++it)
		  {
			  if(*it == NULL)
				  continue;
			  if ( (*it)->kind != ceps::parsetree::Node::structdef) continue;
			  if ( (static_cast<ceps::parsetree::Structdef*>(*it))->get_name() == identifier.get_str())
				    temp.push_back(*it);


		  }
		  return Parsetrees(temp);
	  }

std::ostream& ceps::parsetree::operator << (std::ostream& os, ceps::parsetree::Parsetrees const & pts )
{



	for (auto it = pts.get_roots().cbegin();it != pts.get_roots().cend();++it )
	{
		os << **it;
		os << "\n\n";
	}

	/*for(int i = 0; i != pts.get_roots().size();++i)
	{
		os << *pts.get_roots()[i] << "\n\n";
	}*/

	return os;
}

std::ostream& ceps::parsetree::operator << (std::ostream& os,  ceps::parsetree::Node const & pts )
{
	os <<"(";
	pts.print(os);
	os << ")";
	return os;
}



