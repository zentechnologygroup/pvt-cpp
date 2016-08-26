#include <iostream>
#include <list>
#include <utility>
#include <vector>
 
template <typename Func, typename T, std::size_t ... Is>
decltype(auto) apply(Func&& f, const std::list<T>& pars, std::index_sequence<Is...>)
{
    std::vector<T> v(pars.rbegin(), pars.rend());
 
  return std::forward<Func>(f)(v.at(Is)...);
}
 
template <std::size_t N, typename Func, typename T>
decltype(auto) apply(Func&& f, const std::list<T>& pars)
{
  return apply(std::forward<Func>(f), pars, std::make_index_sequence<N>());
}
 
void print(int a, int b, int c, int d, int e, int f)
{
    std::cout << a << " " << b << " "<< c << " "<< d << " "<< e << " "<< f << std::endl;
}
 
 
int main()
{
    const std::list<int> l{4, 8, 15, 16, 23, 42};
 
    apply<6>(print, l);
}
 
