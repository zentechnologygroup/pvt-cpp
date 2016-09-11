# ifndef TECH_NODE_H
# define TECH_NODE_H

# include <string>

# include <htlist.H>

using namespace std;
using namespace Aleph;

class TechnicalNote
{
  const string db;       // data bank
  DynList<string> notes; // notes
  const string eq; // latex equation

};

# endif // TECH_NODE_H
