#pragma once
#ifndef _d_h_base
#define _d_h_base

//
//
//
#define _d_arch_32 0
#define _d_arch_64 0

#define _d_arch_x86 0
#define _d_arch_x64 0

#define _d_bo_be 0
#define _d_bo_le 0
#define _d_bo_net 0

#define _d_os_win 0
#define _d_os_win32 0
#define _d_os_win64 0

#define _d_posix 0
#define _d_os_unix 0
#define _d_os_linux 0

#define _d_cc_msc 0
#define _d_cc_msc7 0
#define _d_cc_msc8 0
#define _d_cc_msc9 0
#define _d_cc_msc10 0
#define _d_cc_msc_major 0

#define _d_cc_gnu 0
#define _d_cc_mingw 0
#define _d_cc_gnu3 0
#define _d_cc_gnu4 0
#define _d_cc_major 0
#define _d_cc_gnu_minor 0
#define _d_cc_gnu_patch 0

// Force inline.
#define _d_inl_pre
#define _d_inl_post

// Fastcall.
#define _d_fc_pre
#define _d_fc_post

// Etc.
#define _d_cpp 0
#define _d_funcname
#define _d_quote_macro
#define _d_quote_value
#define _d_file_line
#define null 0
#define elif
#define forever

//
//
//
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  #undef _d_arch_32
  #define _d_arch_32 1

  #undef _d_arch_x86
  #define _d_arch_x86 1

  #undef _d_os_win
  #define _d_os_win 1

  #undef _d_os_win32
  #define _d_os_win32 1

  #define WIN32_LEAN_AND_MEAN
  //#ifndef _UNICODE
  //  #define _UNICODE
  //#endif

#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
  #undef _d_arch_64
  #define _d_arch_64 1

  #undef _d_arch_x64
  #define _d_arch_x64 1

  #undef _d_os_win
  #define _d_os_win 1

  #undef _d_os_win64
  #define _d_os_win64 1

  #define WIN32_LEAN_AND_MEAN
  //#ifndef _UNICODE
  //  #define _UNICODE
  //#endif

// Type 'cpp -dM some.h', or 'gcc -dM -E - < /dev/null' to get a list of predefined macros.
#elif defined(linux) || defined(__linux) || defined(__linux__)
  #undef _d_posix
  #define _d_posix 1

  #undef _d_os_unix
  #define _d_os_unix 1

  #undef _d_os_linux
  #define _d_os_linux 1

  #if defined(i386) || defined(__i386)
    #undef _d_arch_32
    #define _d_arch_32 1

    #undef _d_arch_x86
    #define _d_arch_x86 1
  #elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__ )
    #undef _d_arch_64
    #define _d_arch_64 1

    #undef _d_arch_x64
    #define _d_arch_x64 1
  #else
    #error "Unsupported architecture."
  #endif

#else
  #error "Unsupported OS."
#endif

//
//
//
#if defined(_MSC_VER)
  #undef _d_cc_msc
  #define _d_cc_msc 1

  #if _MSC_VER == 1300
    #undef _d_cc_msc7
    #define _d_cc_msc7 1

    #undef _d_cc_msc_major
    #define _d_cc_msc_major 7
  #elif _MSC_VER == 1400
    #undef _d_cc_msc8
    #define _d_cc_msc8 1

    #undef _d_cc_msc_major
    #define _d_cc_msc_major 8
  #elif _MSC_VER == 1500
    #undef _d_cc_msc9
    #define _d_cc_msc9 1

    #undef _d_cc_msc_major
    #define _d_cc_msc_major 9
  #elif _MSC_VER == 1600
    #undef _d_cc_msc10
    #define _d_cc_msc10 1

    #undef _d_cc_msc_major
    #define _d_cc_msc_major 10
  #endif

  #undef _d_funcname
  #define _d_funcname __FUNCSIG__

#elif defined(__GNUC__)
  #undef _d_cc_gnu
  #define _d_cc_gnu 1

  #ifdef __MINGW32__
    #undef _d_cc_mingw
    #define _d_cc_mingw 1
  #endif

  #if __GNUC__ == 3
    #undef _d_cc_gnu3
    #define _d_cc_gnu3 1
  #elif __GNUC__ == 4
    #undef _d_cc_gnu4
    #define _d_cc_gnu4 1
  #endif

  #undef _d_cc_gnu_major
  #undef _d_cc_gnu_minor
  #undef _d_cc_gnu_patch
  #define _d_cc_gnu_major __GNUC__
  #define _d_cc_gnu_minor __GNUC_MINOR__
  #define _d_cc_gnu_patch __GNUC_PATCHLEVEL__

  #undef _d_funcname
  #define _d_funcname __PRETTY_FUNCTION__

#else
  #error "Unsupported compiler."
#endif

//
//
//
#if _d_arch_x86 || _d_arch_x64
  #undef _d_bo_le
  #define _d_bo_le 1
#else
  #error "Unknown endianness."
#endif

//
//
//
#ifdef __cplusplus
  #undef _d_cpp
  #define _d_cpp 1

  #if _d_cc_msc10
    #undef null
    #define null nullptr
  #elif _d_cc_gnu && (_d_cc_gnu_major > 4 || (_d_cc_gnu4 && _d_cc_gnu_minor >= 6))
    #undef null
    #define null nullptr
  #elif _d_arch_32
    #undef null
    #define null 0
  #elif _d_arch_64
    #undef null
    #define null (0L)
  #else
    #error "Undefined: null."
  #endif
#else
  #undef null
  #define null ((void *)0)
#endif

//
//
//
#if _d_cc_msc
  #undef _d_inl_pre
  #define _d_inl_pre  __forceinline

  #undef _d_inl_post
  #define _d_inl_post
#elif _d_cc_gnu
  #undef _d_inl_pre
  #define _d_inl_pre __inline

  #if _d_cc_major > 3
    #undef _d_inl_post
    #define _d_inl_post __attribute__((always_inline))
  #else
    #undef _d_inl_post
    #define _d_inl_post
  #endif
#endif

//
//
//
#if _d_arch_x86
  #if _d_cc_msc
    #undef _d_fc_pre
    #define _d_fc_pre __fastcall

    #undef _d_fc_post
    #define _d_fc_post
  #elif _d_cc_gnu
    #if _d_cc_gnu_major > 3 || (_d_cc_gnu3 && _d_cc_gnu_minor > 3)
      #undef _d_fc_pre
      #define _d_fc_pre

      #undef _d_fc_post
      #define _d_fc_post __attribute__((fastcall))
    #else
      #undef _d_fc_pre
      #define _d_fc_pre

      #undef _d_fc_post
      #define _d_fc_post
    #endif
  #endif
#endif

//
//
//
#undef _d_quote_macro
#define _d_quote_macro(arg) #arg

#undef _d_quote_value
#define _d_quote_value(arg) _d_quote_macro(arg)

#undef _d_file_line
#define _d_file_line __FILE__":"_d_quote_value(__LINE__)

#undef elif
#define elif else if

#undef forever
#define forever for(;;)

//
//
//
typedef unsigned char       byte;
typedef unsigned char       octet;
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short int    int16;
typedef unsigned short int  uint16;

#if _d_cc_msc
  typedef signed    __int32         int32;
  typedef unsigned  __int32         uint32;
  typedef signed    __int64         int64;
  typedef unsigned  __int64         uint64;
#elif _d_cc_gnu
  #if _d_arch_32
    typedef signed int              int32;
    typedef unsigned int            uint32;
    typedef signed long long int    int64;
    typedef unsigned long long int  uint64;
  #elif _d_arch_64
    typedef signed int              int32;
    typedef unsigned int            uint32;
    typedef signed long int         int64;
    typedef unsigned long int       uint64;
  #endif
#endif

#if _d_arch_32
  typedef int32  word;
  typedef uint32 uword;
  typedef int32  ptrdiff;
#elif _d_arch_64
  typedef int64  word;
  typedef uint64 uword;
  typedef int64  ptrdiff;
#else
  #error "Undeclared: word, uword, ptrdiff."
#endif

typedef float  float32;
typedef double float64;

#endif _d_h_global // #ifndef _d_h_base
