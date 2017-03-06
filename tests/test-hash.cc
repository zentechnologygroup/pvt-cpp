
# include <string>
# include <iostream>

# include "../lib/xxhash.c"
# include <hash-fct.H>

# include <tclap/CmdLine.h>

using namespace TCLAP;
using namespace std;
using namespace Aleph;

CmdLine cmd = { "test-hash", ' ', "0" };

ValueArg<size_t> n = { "n", "n", "n", false, 1000, "n", cmd };
ValueArg<string> s =
  { "s", "string", "string", false, "Hola mundo", "string", cmd };

vector<string> vals = { "x", "s" };
ValuesConstraint<string> allowed = vals;
ValueArg<string> type = { "t", "type", "type", true, "", &allowed, cmd };

size_t hash_x(const string & str) { return XXH64(str.c_str(), str.size(), 0); }

size_t hash_s(const string & str) { return dft_hash_fct(str); }

size_t (*hash_fct)(const string & str) = nullptr;

int main(int argc, char *argv[])
{
  cmd.parse(argc, argv);
  size_t n = ::n.getValue();
  hash_fct = type.getValue() == "x" ? hash_x : hash_s;
  const string & str = s.getValue();
  for (size_t i = 0; i < n; ++i)
    size_t v = (*hash_fct)(str);
}
