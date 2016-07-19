
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
      name << "Name " << i;
      symbol << "Symbol " << i;
      desc << "Description " << i;
      table.register_item(name.str(), symbol.str(), desc.str());
    }

  for (size_t i = n; i < 2*n; ++i)
    {
      ostringstream name, symbol, desc;
      name << "Name " << i;
      symbol << "Symbol " << i;
      desc << "Description " << i;
      table.register_item(UnitItem(name.str(), symbol.str(), desc.str()));
    }

  for (size_t i = 0; i < 2*n; ++i)
    {
      ostringstream name, symbol;
      name << "Name " << i;
      symbol << "Symbol " << i;

      if (not table.exists_name(name.str()))
	{
	  cout << "test failed in line " << __LINE__ << endl;
	  abort();
	}
    }

  for (auto it = table.get_it(); it.has_curr(); it.next())
    cout << *it.get_curr().second << endl
	 << endl;

  return 0;
}
