# include <type_traits>
# include <iostream>
# include <list>
# include <utility>
# include <vector>

# include <boost/type_traits.hpp>

# include <traits.hpp>

# include <htlist.H>
# include <ah-stl-utils.H>

using namespace boost;
using namespace utils;

template <typename F> struct arity;

template <typename Ret, typename ...Args> struct arity<Ret(Args...)>
{
  static constexpr std::size_t value = sizeof...(Args);
};

template <typename Func, typename T, std::size_t ... Is>
decltype(auto)
  apply(Func&& f, const DynList<T>& pars, std::index_sequence<Is...>)
{
  std::vector<T> v = to_vector(pars); 
  return std::forward<Func>(f)(v.at(Is)...);
}
 
template <typename Func, typename T>
decltype(auto) apply(Func&& f, const DynList<T>& pars)
{
  constexpr std::size_t N =
    arity<std::remove_pointer_t<std::decay_t<Func>>>::value;
  return apply(std::forward<Func>(f), pars, std::make_index_sequence<N>());
}

void print(int a, int b, int c, int d, int e, int f)
{
    std::cout << a << " " << b << " "<< c << " "<< d << " "<< e << " "<< f << std::endl;
}

struct Foo
{
  void m1(int, int, int) {}
};
  
using namespace std;


int main()
{
  //typedef void (*fct)(int a, int b, int c, int d, int e, int f);
  auto n = utils::function_traits<decltype(&Foo::m1)>::arity;

  cout << n << endl;

  const DynList<int> l{4, 8, 15, 16, 23, 42};
  apply(print, l);
}
 
