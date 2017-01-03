
# include <iostream>
# include <correlations/par-list.H>

using namespace std;

void test()
{
  ParList plist;

  plist.insert("p", 100, &psia::get_instance());
  plist.insert("rs", 1110, &SCF_STB::get_instance());
  plist.insert("t", 180, &Fahrenheit::get_instance());
  plist.insert("po", 100, &Lb_ft3::get_instance());
  plist.insert("uo", 1.2, &CP::get_instance());

  auto p = plist.search("p");
  auto rs = plist.search("rs");
  auto t = plist.search("t");
  auto po = plist.search("po");
  auto uo = plist.search("uo");

  cout << p << endl
       << rs << endl
       << t << endl
       << po << endl
       << uo << endl;
}

int main()
{
  test();
}
