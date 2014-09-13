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

#ifndef SI_UNITS_H_
#define SI_UNITS_H_

#include<ostream>

#ifdef _MSC_VER

#if MSC_VER > 1700
#define CONSTEXPR constexpr
#else
#define NO_USERDEFINED_LITERALS
#define CONSTEXPR 

#endif

#else

#define CONSTEXPR constexpr

#endif

namespace SI
{

/**
 * According to 2.1.2 in "Bureau international des poids et mesures: Le Systeme international d’unites", 8ed 
 * we define a Unit as a parameterized type, i.e. a Unit is a template with seven nontype parameters (of type int), where
 * each parameter corresponds to one of the SI base units as defined in ibid. chapter 2. Because of their rare use the last four parameters have
 * default values (=0).
 * 
 */
 template<	int m_, 	/*Unit of length: meter. See 2.1.1.1 of the BIPM brochure on SI units */
 	 	int kg_, 	/*Unit of mass: kilogram. Ibid.  2.1.1.2 */
 	 	int s_,   	/*Unit of time: second. Ibid. 2.1.1.3*/
 	 	int A_ = 0, 	/*Unit of electric current: ampere.Ibid. 2.1.1.4 */
 	 	int K_ = 0, 	/*Unit of thermodynamic temperature: kelvin. Ibid. 2.1.1.5*/
 	 	int mol_ = 0, 	/*Unit of amount of substance: mole. Ibid. 2.1.1.6*/
 	 	int cd_ = 0	/*Unit of luminous intensity: candela. Ibid. 2.1.1.7*/
 	 	 	 	 >
 struct Unit
 {
	 enum {m = m_, kg = kg_, s = s_, A = A_, K = K_, mol = mol_, cd = cd_};
 };
 
 
 template<typename U1, typename U2>
  struct Uplus
  {
	 using type = Unit<	U1::m + U2::m,
				U1::kg + U2::kg,
			 	U1::s + U2::s,
			 	U1::A + U2::A,
			 	U1::K + U2::K,
			 	U1::mol + U2::mol,
			 	U1::cd + U2::cd>;
  };
 
 template <typename U1,typename U2>
  using Unit_plus = typename Uplus<U1,U2>::type;

 template<typename U1, typename U2>
   struct Uminus
   {
 	 using type = Unit<	U1::m - U2::m,
 			 	U1::kg - U2::kg,
 			 	U1::s - U2::s,
 			 	U1::A - U2::A,
 			 	U1::K - U2::K,
 			 	U1::mol - U2::mol,
 			 	U1::cd - U2::cd
			>;
   };
  
  template <typename U1,typename U2>
   using Unit_minus = typename Uminus<U1,U2>::type;

  template<typename U>
    struct Uinv
    {
  	 using type = Unit<	-U::m,
  			 	-U::kg,
  			 	-U::s,
  			 	-U::A,
  			 	-U::K,
  			 	-U::mol,
  			 	-U::cd
			>;
    };

  
  template <typename U>
   using Unit_inv = typename Uinv<U>::type;

 /**
  * What follows are some aliases
  */
 using M = Unit<1,0,0>;			// meters
 using Kg = Unit<0,1,0>; 			//kilograms
 using S = Unit<0,0,1>;			//seconds
 
 using MpS = Unit<1,0,-1>; 		//meters per second
 using MpS2 = Unit<1,0,-2>; 	//meters per square second
 
 using Metre = M;																
 using Kilogram = Kg;
 using Second = S;
 using Ampere = Unit<0,0,0,1>;
 using Kelvin = Unit<0,0,0,0,1>;
 using Mole = Unit<0,0,0,0,0,1>;
 using Candela = Unit<0,0,0,0,0,0,1>;
 
 //Some coherent derived units
 
 using Square_Metre = Unit<2,0,0>;   //derived quantity: Area
 using Cubic_Metre = Unit<3,0,0>;		//derived quantity: Volume
 using Metre_per_second = MpS;        //derived quantity: speed,velocity 
 using Metre_per_second_squared = MpS2;  //derived quantity: acceleration
 using Radian_per_second = Unit<0,0,-1>; //derived quantity: rotational speed, angular velocity
 using Reciprocal_metre = Unit<-1,0,0>; //derived quantity: Wavenumber \sigma, \tilde{\nu}
 using Kilogram_per_cubic_metre = Unit<-3,1,0>; //derived quantity: density, mass density
 using Kilogram_per_square_metre = Unit<-2,1,0>; //derived quantity: surface density
 using Cubic_metre_per_kilogram = Unit<3,-1,0>; //derived quantity: specific volume
 using Ampere_per_square_metre = Unit<-2,0,0,1>; //derived quantity: current density
 using Ampere_per_metre = Unit<-1,0,0,1>; //derived quantity: magnetic field strength
 using Mole_per_cubic_metre = Unit<-3,0,0,0,1>; //derived quantity: amount concentration,concentration
 
 using Radian = Unit<0,0,0>;
 using Steradian = Unit<0,0,0>;
 using Hertz = Unit<0,0,-1>;
 using Newton = Unit<1,1,-2>;
 using Pascal = Unit<-1,1,-2>;
 using Joule = Unit<2,1,-2>;
 using Watt = Unit<2,1,-3>;
 using Coulomb = Unit<0,0,1,1>;
 using Volt = Unit<2,1,-3,-1>;
 using Farad = Unit<-2,-1,4,2>;
 using Ohm = Unit<2,1,-3,-2>;
 using Siemens = Unit<-2,-1,3,2>;
 using Weber = Unit<2,1,-2,-1>;
 using Tesla = Unit<0,1,-2,-1>;
 using Henry = Unit<2,1,-2,-2>;
 using Celsius = Unit<0,0,0,0,1>;
 using Lumen = Unit<0,0,0,0,0,1>;
 using Iluminance = Unit<-1,0,0,0,0,1>;
 using Becquerel = Unit<0,0,-1>;
 using Gray = Unit<2,0,-2>; 
 using Sievert = Unit<2,0,-2>; 
 using Katal = Unit<0,0,-1,0,0,1>;
 
 //Quantities


 
 template<typename U, typename Scalar_type = double>
  struct Quantity
  {
	 Scalar_type value_;
	 CONSTEXPR explicit Quantity(Scalar_type value) 
	 	 :value_{value} 
	 {}	 
	 CONSTEXPR Quantity() : value_{}
	 {}
	 
	 Quantity( Quantity<U,Scalar_type> const & rhs):value_{rhs.value_}
	 {
		 
	 }
  };
 
 template<typename T>
 struct Quantity<Unit<0,0,0,0,0,0,0>,T >
 {
	 T value_;
	 CONSTEXPR Quantity(T value)
	 	 :value_{value} 
	 {}	 
	 CONSTEXPR Quantity() : value_{}
	 {}

	 CONSTEXPR inline operator T ()
	 		{
	 			return value_;
	 		}	 
 };

 using Length = Quantity<Metre,double>;
 using Velocity = Quantity<MpS,double>;
 using Mass = Quantity<Kg,double>;
 using Time = Quantity<Second,double>;
 using Current = Quantity<Ampere,double>;
 using Temperature = Quantity<Kelvin,double>;
 using Substance_amount = Quantity<Mole,double>;
 using Luminous_intensity = Quantity<Candela,double>;

 //Operations
 

 template<typename U, typename Scalar_type>
   Quantity<U,Scalar_type>& operator += (Quantity<U,Scalar_type>& lhs,Quantity<U,Scalar_type>rhs )
  {
	 lhs.value_ += rhs.value_;
 	 return lhs;
  }

 template<typename U, typename Scalar_type>
  Quantity<U,Scalar_type> operator + (Quantity<U,Scalar_type> lhs,Quantity<U,Scalar_type>rhs )
 {
	 return Quantity<U,Scalar_type>{lhs.value_ + rhs.value_};
 }
 
 template<typename U, typename Scalar_type>
  Quantity<U,Scalar_type> operator - (Quantity<U,Scalar_type> const & lhs,Quantity<U,Scalar_type> const & rhs )
 {
	 return Quantity<U,Scalar_type>{lhs.value_ - rhs.value_};
 }
 
 template<typename U1, typename U2, typename Scalar_type>
   Quantity<Unit_plus<U1,U2>,Scalar_type> operator * (Quantity<U1,Scalar_type> lhs,Quantity<U2,Scalar_type>rhs )
  {
 	 return Quantity<Unit_plus<U1,U2>,Scalar_type>{lhs.value_ * rhs.value_};
  }

 template<typename U,typename Scalar_type>
  Quantity<U,Scalar_type> operator * (Scalar_type s, Quantity<U,Scalar_type> q )
  {
	 return Quantity<U,Scalar_type>{s * q.value_};
  }


template<typename U,typename Scalar_type>
 Quantity<U,Scalar_type> operator * (Quantity<U,Scalar_type> q, Scalar_type s)
 {
	 return Quantity<U,Scalar_type>{q.value_ * s};
 }


template<typename U,typename Scalar_type>
 Quantity<Unit_inv<U>,Scalar_type> operator / (Scalar_type s, Quantity<U,Scalar_type> q )
 {
	 return Quantity<Unit_inv<U>,Scalar_type>{s * (1/ q.value_)};
 }

template<typename U>
 Quantity<Unit_inv<U>,double> operator / (int s, Quantity<U,double> q )
 {
	 return Quantity<Unit_inv<U>,double>{s * (1/ q.value_)};
 }

template<typename U>
 Quantity<Unit_inv<U>,double> operator / (double s, Quantity<U,int> q )
 {
	 return Quantity<Unit_inv<U>,double>{s * (1/ (double)q.value_)};
 }


template<typename U>
CONSTEXPR Quantity<U> operator - (Quantity<U> q)
 {
	 return Quantity<U>{ -1.0 *   q.value_};
 }

template<typename U>
CONSTEXPR Quantity<U, int> operator - (Quantity<U, int> q)
 {
	 return Quantity<U,int>{ -1 *   q.value_};
 }

template<typename U, typename Scalar_type>
CONSTEXPR  bool operator == (Quantity<U, Scalar_type> lhs, Quantity<U, Scalar_type>rhs)
 {
	 return lhs.value_ == rhs.value_;
 }

template<typename U, typename Scalar_type>
CONSTEXPR bool operator != (Quantity<U, Scalar_type> lhs, Quantity<U, Scalar_type>rhs)
 {
	 return lhs.value_ != rhs.value_;
 }

template<typename U, typename Scalar_type>
CONSTEXPR bool operator < (Quantity<U, Scalar_type> lhs, Quantity<U, Scalar_type>rhs)
 {
	 return lhs.value_ < rhs.value_;
 }

template<typename U, typename Scalar_type>
CONSTEXPR bool operator >(Quantity<U, Scalar_type> lhs, Quantity<U, Scalar_type>rhs)
 {
	 return lhs.value_ > rhs.value_;
 }

template<typename U, typename Scalar_type>
CONSTEXPR bool operator >= (Quantity<U, Scalar_type> lhs, Quantity<U, Scalar_type>rhs)
 {
	 return lhs.value_ >= rhs.value_;
 }

template<typename U, typename Scalar_type>
CONSTEXPR  bool operator <= (Quantity<U, Scalar_type> lhs, Quantity<U, Scalar_type>rhs)
 {
	 return lhs.value_ <= rhs.value_;
 }


//Specializations

template<typename Scalar_type>
   Quantity<Metre,Scalar_type> operator * (Quantity<Second,Scalar_type> lhs,Quantity<MpS,Scalar_type>rhs )
  {
 	 return Quantity<Metre,Scalar_type>{lhs.value_ * rhs.value_};
  }




template <int m_, int kg_, int s_,int A_, int K_,int mol_,int cd_, typename S>
 void suffix(std::ostream& os,
		           SI::Quantity<SI::Unit<m_,kg_,s_,A_,K_,mol_,cd_>, S> q)
 {
	if (m_ != 0)
	{
		if (std::abs(m_) > 1 || m_ < 0 )
			os << "m^"<<m_;
		else
			os << "m";
	}

	if (kg_ != 0)
	{
		if (std::abs(kg_) > 1 || kg_ < 0 )
			os << "kg^"<<kg_;
		else
			os << "kg";
	}

	if (s_ != 0)
	{
		if (std::abs(s_) > 1 || s_ < 0 )
			os << "s^"<<s_;
		else
			os << "s";
	}

	if (A_ != 0)
	{
		if (std::abs(A_) > 1 || A_ < 0 )
			os << "A^"<<A_;
		else
			os << "A";
	}

	if (K_ != 0)
	{
		if (std::abs(K_) > 1 || K_ < 0 )
			os << "K^"<<K_;
		else
			os << "K";
	}

	if (mol_ != 0)
	{
		if (std::abs(mol_) > 1 || mol_ < 0 )
			os << "mol^"<<mol_;
		else
			os << "mol";
	}

	if (cd_ != 0)
	{
		if (std::abs(cd_) > 1 || cd_ < 0 )
			os << "cd^"<<cd_;
		else
			os << "cd";
	}

 }

template<typename U, typename S>
 std::ostream& operator << (std::ostream& os,SI::Quantity<U,S>  q)
{
	os << q.value_;
	suffix(os,q);
	return os;
}


}

#endif
