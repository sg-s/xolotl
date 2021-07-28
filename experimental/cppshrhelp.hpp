/* Copyright 2019-2020 The MathWorks, Inc. */

/* cppshrhelp.hpp
 * Shared library helper include file for cross platform portability
 * no defines are needed to use the library. */

#ifndef CPPSHRHELP
#define CPPSHRHELP

#ifdef _MSC_VER
# define DLL_EXPORT __declspec(dllexport)
#elif __GNUC__ >= 4
# define DLL_EXPORT __attribute__ ((visibility("default")))
#else
# define DLL_EXPORT
#endif

#endif

