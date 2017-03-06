
# include <iostream>
# include <biblio.H>

using namespace std;

int main()
{
  BibEntry::entries().for_each([] (auto ptr)
     {
       std::cout << *ptr << endl; 
     });

  return 0;
}
