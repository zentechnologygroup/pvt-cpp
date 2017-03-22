#ifndef CHECK_WORD_H
#define CHECK_WORD_H

// Check windows
#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

# ifdef ENV64BIT
# define HASHFCT XXH64
# else
# define HASHFCT XXH32
# endif



#endif
