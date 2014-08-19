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

#ifndef CEPS_TYPEFUNC_H_
#define CEPS_TYPEFUNC_H_


/**
 * @brief: Type function which returns the n-th (starting at 0) type given a list of m > n types.
 *
 * @example: getNth_type<1,char,int> evaluates to int
 *
 */
template<int N  ,typename... Ts> //  |Ts...| > N >= 0, where |.| denotes length of type-list Ts...
 struct getNth_type;

/**
 * @brief Base case, N == 0.
 */
template<typename T,typename... Ts>
	struct getNth_type<0,T,Ts...>
	{
		using type = T;
	};

/**
 * @brief Inductive step
 */
template<int N,typename T,typename... Ts>
	struct getNth_type<N,T,Ts...>:public getNth_type<N-1,Ts...>
	{

	};

/**
 * @brief Conditional: returns Type_true if the non type parameter == true, returns Type_false otherwise.
 */
template<bool,typename Type_true,typename Type_false>
	struct select_type;

/**
 * @brief Case bool parameter == true
 */
template<typename Type_true,typename Type_false>
 	 struct select_type<true,Type_true,Type_false>
	{
		using type = Type_true;
	};

/**
 * @brief Case bool parameter == false
 */
template<typename Type_true,typename Type_false>
 struct select_type<false,Type_true,Type_false>
	{
		using type = Type_false;
	};

struct True_type
{
};
struct False_type
{
};

template<bool cond>
	struct Select_bool_type;

template<>
	struct Select_bool_type<true>
		{
			using type = True_type;
		};
template<>
	struct Select_bool_type<false>
		{
			using type = False_type;
		};



#endif
