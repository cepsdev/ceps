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

#ifndef CEPS_GLOBAL_DEFS_HH
#define CEPS_GLOBAL_DEFS_HH
#include <stdlib.h>
#include <stdio.h>
#include <string>

#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 8  && __cplusplus)
 #define FULL_CPP11_COMPLIANT_COMPILER
#endif

#ifdef FULL_CPP11_COMPLIANT_COMPILER
		#define TYPE_ALIAS(Type_A,Type_B) using Type_A = Type_B ;
#else
		#define TYPE_ALIAS(Type_A,Type_B) typedef Type_B Type_A ;
#endif

inline std::string mk_string(int j)
{
	auto const buf_size = 64;
	char buffer[buf_size] = { 0 };
#if _MSC_VER >= 1700
	sprintf_s(buffer, buf_size,"%d", j);
#else
	sprintf(buffer, "%d", j);
#endif
	return buffer;
}

#define CEPSERROR(x) throw std::runtime_error{\
						std::string{" In module "} + std::string{__FILE__} + std::string{", at line "} + mk_string(__LINE__) + std::string{": "} + std::string{x} };


#endif
