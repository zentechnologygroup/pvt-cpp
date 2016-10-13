
# ifndef TEST_UTILS_H
# define TEST_UTILS_H

# include <iostream>
# include <sstream>

using namespace std;

inline void __test_assert(bool pred, const char * exp_text,
			  const char * filename, const size_t linenum)
{
  if (pred)
    return;

  cout << "assert(" << exp_text << ") " << filename << " : " << linenum
       << " violated" << endl;
  abort();
}

# define test_assert(cond)						\
  (void)((cond) || (__test_assert(cond, #cond, __FILE__, __LINE__),0))

# endif // TEST_UTILS_H
