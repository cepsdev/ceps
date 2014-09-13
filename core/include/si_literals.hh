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

#include "si_units.hh"

#ifndef SI_LITERALS_H_
#define SI_LITERALS_H_


namespace SI {

#ifndef NO_USERDEFINED_LITERALS

/*
template <typename T>
	constexpr Quantity<Metre,T> operator"" _m(T value)
	{
		return Quantity<Metre,T>{value};
	}
*/

	CONSTEXPR Quantity<SI::Metre> operator  ""  _m(long double value)
	{
		return Quantity<SI::Metre>{ static_cast<double>(value) };
	}

	CONSTEXPR Quantity<SI::Metre, int> operator  ""  _m(unsigned long long value)
	{
		return Quantity<SI::Metre,int>{ static_cast<int>(value) };
	}


	CONSTEXPR Quantity<SI::Kilogram> operator  ""  _kg(long double value)
	{
		return Quantity<SI::Kilogram>{ static_cast<double>(value) };
	}

	CONSTEXPR Quantity<SI::Kilogram, int> operator  ""  _kg(unsigned long long value)
	{
		return Quantity<SI::Kilogram,int>{ static_cast<int>(value) };
	}

 //Time
	CONSTEXPR Quantity<SI::Second> operator  ""  _s(long double value)
 	{
 		return Quantity<SI::Second>{ static_cast<double>(value) };
 	}

	CONSTEXPR Quantity<SI::Second, int> operator  ""  _s(unsigned long long value)
 	{
 		return Quantity<SI::Second,int>{ static_cast<int>(value) };
 	}
  
  
  //Current
	CONSTEXPR Quantity<SI::Ampere> operator  ""  _A(long double value)
  	{
  		return Quantity<SI::Ampere>{ static_cast<double>(value) };
  	}

	CONSTEXPR Quantity<SI::Ampere, int> operator  ""  _A(unsigned long long value)
  	{
  		return Quantity<SI::Ampere,int>{ static_cast<int>(value) };
  	}

	CONSTEXPR Quantity<SI::Kelvin> operator  ""  _K(long double value)
   	{
   		return Quantity<SI::Kelvin>{ static_cast<double>(value) };
   	}

	CONSTEXPR Quantity<SI::Kelvin, int> operator  ""  _K(unsigned long long value)
   	{
   		return Quantity<SI::Kelvin,int>{ static_cast<int>(value) };
   	}
    
    
    
	CONSTEXPR Quantity<SI::Mole> operator  ""  _mol(long double value)
    	{
    		return Quantity<SI::Mole>{ static_cast<double>(value) };
    	}

	CONSTEXPR Quantity<SI::Mole, int> operator  ""  _mol(unsigned long long value)
    	{
    		return Quantity<SI::Mole,int>{ static_cast<int>(value) };
    	}
    

     
	CONSTEXPR Quantity<SI::Candela> operator  ""  _cd(long double value)
     	{
     		return Quantity<SI::Candela>{ static_cast<double>(value) };
     	}

	CONSTEXPR Quantity<SI::Candela, int> operator  ""  _cd(unsigned long long value)
     	{
     		return Quantity<SI::Candela,int>{ static_cast<int>(value) };
     	}


/*using Metre = M;																
 using Kilogram = Kg;
 using Second = S;
 using Ampere = Unit<0,0,0,1>;
 using Kelvin = Unit<0,0,0,0,1>;
 using Mole = Unit<0,0,0,0,0,1>;
 using Candela = Unit<0,0,0,0,0,0,1>;
 */
	
#endif

}

#endif
