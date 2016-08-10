
# include <cstdlib>
# include <sstream>
# include <iostream>
# include <unititem.H>

using namespace std;

void usage()
{
  cout << "./test-unititem n" << endl;
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    usage();

  cout << "Testing UnitItem object" << endl
       << endl;

  UnitItemTable table;

  size_t n = atoi(argv[1]);
  for (size_t i = 0; i < n; ++i)
    {
      ostringstream name, symbol, desc;
      name << "Name_" << i;
      symbol << "Symbol_" << i;
      desc << "Description_" << i;
      table.register_item(new UnitItem(name.str(), symbol.str(), desc.str()));
    }

  for (size_t i = 0; i < n; ++i)
    {
      ostringstream name, symbol;
      name << "Name_" << i;
      symbol << "Symbol_" << i;

      if (not table.exists_name(name.str()))
	{
	  cout << "test failed in line " << __LINE__ << endl;
	  abort();
	}

      if (not table.exists_symbol(symbol.str()))
	{
	  cout << "test failed in line " << __LINE__ << endl;
	  abort();
	}

      if (table.search_by_name(name.str()) == nullptr)
	{
	  cout << "test failed in line " << __LINE__ << endl;
	  abort();
	}
	  
      if (table.search_by_symbol(symbol.str()) == nullptr)
	{
	  cout << "test failed in line " << __LINE__ << endl;
	  abort();
	}  
    }

  if (table.size() != n)
    {
      cout << "test failed in line " << __LINE__ << endl;
      abort();
    }  

  for (auto it = table.get_it(); it.has_curr(); it.next())
    cout << *it.get_curr().second << endl
	 << endl;

  return 0;
}
