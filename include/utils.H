# ifndef UTILS_H
# define UTILS_H

# include <cstdlib>
# include <memory>
# include <cxxabi.h>
# include <string>
# include <sstream>
# include <typeinfo>

# include <ah-convert.H>
# include <htlist.H>
# include <tpl_array.H>

inline std::string demangle(const char* name)
{
  int status = -4; // some arbitrary value to eliminate the compiler warning
  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void(*)(void*)> res
    { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
  return (status==0) ? res.get() : name ;
}

# define CLASSNAME_TO_STRING(class_ptr) demangle(typeid(*class_ptr).name())

template <typename T>
inline std::string Rvector(const std::string & name, const DynList<T> & l)
{
  std::ostringstream s;
  s << name << " <- c(";
  for (auto it = l.get_it(); it.has_curr(); it.next())
    {
      const auto & v = it.get_curr();
      s << v;
      if (&v != &l.get_last())
	s << ", ";
    }
  s << ")";

  return s.str();
}

inline std::string Rvector(const std::string & name, const Array<double> & a)
{
  return Rvector(name, to_DynList(a));
} 

# endif // UTILS_H
