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

#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 8  && __cplusplus)
 #define FULL_CPP11_COMPLIANT_COMPILER
#endif

#ifdef FULL_CPP11_COMPLIANT_COMPILER
		#define TYPE_ALIAS(Type_A,Type_B) using Type_A = Type_B ;
#else
		#define TYPE_ALIAS(Type_A,Type_B) typedef Type_B Type_A ;
#endif

#define ERROR(x) throw std::runtime_error{\
						std::string{" In module "} + std::string{__FILE__} + std::string{", at line "} + std::to_string(__LINE__) + std::string{": "} + std::string{x} };
