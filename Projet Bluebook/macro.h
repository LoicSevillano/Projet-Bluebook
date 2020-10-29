#ifndef UTILS_MACRO_H_
#define UTILS_MACRO_H_
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  Created by Arthur Moncorge on Dec 9, 2008.
///  Copyright 2008. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <cstring> // for memset
#include <cstdlib> // for exit
#include <cmath>

#define BEST_CCAST(NEWTYPE, DATA)  ((NEWTYPE)(DATA))
#define BEST_STATIC_CPPCAST(NEWTYPE, DATA) static_cast<NEWTYPE>(DATA)

#define BEST_ROOT 0


using namespace std;

inline void BEST_EXIT(int i)
{
    // Put breakpoint here to catch issues with debugger

    exit(i);
}

inline void BEST_MESSAGE(std::string message)
{
    std::cout << "MESSAGE: "+message << "\n";
}

#ifdef CORDEBUG
#define BEST_DEBUG_ASSERT(x) \
    if (! (x)) \
{ \
    std::cout << "ERROR: Assert " << #x << " failed"; \
    std::cout << " on line " << __LINE__ ; \
    std::cout << " in file " << __FILE__ << "\n";  \
    std::cerr << "ERROR: Assert " << #x << " failed"; \
    std::cerr << " on line " << __LINE__ ; \
    std::cerr << " in file " << __FILE__ << "\n";  \
    BEST_EXIT(1); \
}
#else
#define BEST_DEBUG_ASSERT(x)
#endif

#define BEST_ASSERT(x) \
    if (! (x)) \
{ \
    std::cout << "ERROR: Assert " << #x << " failed"; \
    std::cout << " on line " << __LINE__ ; \
    std::cout << " in file " << __FILE__ << "\n";  \
    std::cerr << "ERROR: Assert " << #x << " failed"; \
    std::cerr << " on line " << __LINE__ ; \
    std::cerr << " in file " << __FILE__ << "\n";  \
    BEST_EXIT(1); \
}


#define BEST_ASSERT2(x, message) \
    if (! (x)) \
{ \
    std::cout << "ERROR: Assert " << #x << " failed"; \
    std::cout << " on line " << __LINE__ ; \
    std::cout << " in file " << __FILE__ << "\n";  \
    std::cerr << "ERROR: Assert " << #x << " failed"; \
    std::cerr << " on line " << __LINE__ ; \
    std::cerr << " in file " << __FILE__ << "\n";  \
    std::cout << message <<"\n"; \
    BEST_EXIT(1); \
}

#define BEST_WARNING(warn) \
{ \
    std::cout << "WARNING: " << warn << " in file " << __FILE__ << " at line " << __LINE__ << std::endl; \
    std::cerr << "WARNING: " << warn << " in file " << __FILE__ << " at line " << __LINE__ << std::endl; \
}

#define BEST_ERROR(err) \
{ \
    std::cout << "BEST_ERROR: " << err << " in file " << __FILE__ << " at line " << __LINE__ << std::endl; \
    std::cerr << "BEST_ERROR: " << err << " in file " << __FILE__ << " at line " << __LINE__ << std::endl; \
    BEST_EXIT(1); \
}

inline void BEST_ASSERT_FINITE(int i)
{
#ifndef COROPTIMIZE
    // This test fails for INFINITE and NAN in Windows and Linux
    int i2 = i+i;
    if (i2-i != i) BEST_ERROR("INFINITE value");
#endif
}

inline void BEST_ASSERT_FINITE(double i)
{
#ifndef COROPTIMIZE
    // This test fails for INFINITE and NAN in Windows and Linux
    if (std::isnan(i)) BEST_ERROR("NaN value");
    double i2 = i+i;
    if (i2-i != i) BEST_ERROR("INFINITE value");
#endif
}

inline void BEST_ASSERT_FINITE(int* d, int size)
{
#ifndef COROPTIMIZE
    for (int i=0; i<size; i++) BEST_ASSERT_FINITE(d[i]);
#endif
}

inline void BEST_ASSERT_FINITE(const double* d, int size)
{
#ifndef COROPTIMIZE
    for (int i=0; i<size; i++) BEST_ASSERT_FINITE(d[i]);
#endif
}

inline bool BEST_TEST_NAN(int i)
{
	return false; // int cannot be a NAN value
}

inline bool BEST_TEST_NAN(double i)
{
    return std::isnan(i);
}

// Do not use "==" or "!=" to test floating-point expression
inline bool BEST_TEST_DOUBLE_ZERO(double d)
{
    // Test for zero
    if (d<0.0 || d>0.0) return false;
    return true;
}

// Do not use "==" or "!=" to test floating-point expression
inline bool BEST_TEST_DOUBLE(double d1, double d2)
{
    // Test for zero
    if (d1-d2<0.0 || d1-d2>0.0) return false;
    return true;
}

// Do not use "==" or "!=" to test floating-point expression
inline bool BEST_TEST_DOUBLE_EQUALITY(double d1, double d2)
{
    // HACK. There is a magic number here
    if (d2-d1>1.0e-9 || d1-d2>1.0e-9) return false;
    return true;
}

inline void BEST_ASSERT_DOUBLE_EQUALITY(double d1, double d2)
{
#ifndef COROPTIMIZE
    BEST_ASSERT(d2-d1 < 1.0e-9 && d1-d2 < 1.0e-9); // HACK_LEO. There is a magic number here
#endif
}

inline void BEST_ASSERT_DOUBLE_NONEQUALITY(double d1, double d2)
{
#ifndef COROPTIMIZE
    BEST_ASSERT(d2-d1 > 0.0 || d1-d2 > 0.0);
#endif
}

// File utility
inline bool BEST_FILEEXISTS(std::string filename)
{
    bool rtn = false;
    std::ifstream testfile;
    testfile.open(filename.c_str(), std::ios::in);
    if (testfile) rtn=true;
    else          rtn=false;
    testfile.close();
    return rtn;
}

inline void BEST_WRITENEWFILE(std::string filename)
{
    std::ofstream testfile;
    testfile.open(filename.c_str());
    testfile.close();
}

// It's good to hide the use of "sizeof" b/c it is error prone
inline void BEST_SETZERO(int* target, int size)
{
    memset(target,0,size*sizeof(int));
}


inline void BEST_SETZERO(double* target, int size)
{
    memset(target,0,size*sizeof(double));
}

inline void BEST_SETZERO(char* target, int size)
{
    // Beware, cannot do a template here !!!
    memset(target,' ',size*sizeof(char));
}

inline void BEST_SETCONSTANT(int* target, int size, int cst)
{
    for (int i=0; i<size; ++i)  target[i]=cst;
}

inline void BEST_SETCONSTANT(double* target, int size, double cst)
{
    for (int i=0; i<size; ++i)  target[i]=cst;
}

inline void BEST_MEMCOPY(int* target, const int* source, int size)
{
    memcpy(target,source,size*sizeof(int));
}

inline void BEST_MEMCOPY(bool* target, const bool* source, int size)
{
    memcpy(target,source,size*sizeof(bool));
}

inline void BEST_MEMCOPY(float* target, const float* source, int size)
{
    memcpy(target,source,size*sizeof(float));
}

inline void BEST_MEMCOPY(double* target, const double* source, int size)
{
    memcpy(target,source,size*sizeof(double));
}

inline void BEST_MEMCOPY(char* target, const char* source, int size)
{
    memcpy(target,source,size*sizeof(char));
}

inline void BEST_MEMCOPYF2D(double* target, const float* source, int size)
{
    for (int i=0; i<size; i++) target[i] = BEST_STATIC_CPPCAST(double, source[i]);
}

inline void BEST_MEMCOPYD2F(float* target, const double* source, int size)
{
    for (int i=0; i<size; i++) target[i] = BEST_STATIC_CPPCAST(float, source[i]);
}

#endif