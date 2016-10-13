
# include <initializer_list>
# include <iostream>
# include <htlist.H>

# include <units/units.H>

using namespace std;

struct Distance : public PhysicalQuantity
{
  static const Distance & get_instance()
  {
    static Distance instance;
    return instance;
  }
  
private:
  
  Distance()
    : PhysicalQuantity("Distance", "d",
		       "Measure about how far appart an object is") 
  {
    cout << "Distance ctor" << endl;
  }

public:
  
  Distance(const Distance&) = delete;
  void operator = (const Distance&) = delete;
};

class Kilometer : public Unit
{
  Kilometer()
    : Unit("Kilometer", "km", "1000 meters",
	   Distance::get_instance(), 0, numeric_limits<double>::max())
  {
    cout << "Kilometer ctor" << endl;
  }
public:
  static const Kilometer & get_instance()
  {
    static Kilometer instance;
    return instance;
  }
  
  Kilometer(const Kilometer&) = delete;
  void operator = (const Kilometer&) = delete;
};


struct Time : public PhysicalQuantity
{
  static const Time & get_instance()
  {
    static Time instance;
    return instance;
  }
private:
  Time() : PhysicalQuantity("Time", "t", "The mistery of the life")
  {
    cout << "Time ctor" << endl;
  }
public:
  Time(const Time&) = delete;
  void operator = (const Time&) = delete;
};

class Hour : public Unit
{
  Hour()
    : Unit("Hour", "h", "60 minutes", Time::get_instance(),
	   0, numeric_limits<double>::max())
  {
    cout << "Hour ctor" << endl;
  }
public:
  static const Hour & get_instance()
  {
    static Hour instance;
    return instance;
  }
  Hour(const Hour&) = delete;
  void operator = (const Hour&) = delete;
};

struct Speed : public PhysicalQuantity
{
  static const Speed & get_instance()
  {
    static Speed instance;
    return instance;
  }
private:
  Speed() : PhysicalQuantity("Speed", "v", "Rate of change of position")
  {
    cout << "Spped ctor" << endl;
  }

public:
  Speed(const Speed&) = delete;
  void operator = (const Speed&) = delete;
};

class Km_h : public Unit
{
  Km_h()
    : Unit("Km_h", "km/h", "Standard measure of speed",
	   Speed::get_instance(), 0, numeric_limits<double>::max())
  {
    cout << "Km_h ctor" << endl;
  }

public:
  static const Km_h & get_instance()
  {
    static Km_h instance;
    return instance;
  }
  Km_h(const Km_h&) = delete;
  void operator = (const Km_h&) = delete;
};

template <> struct Combine_Units<Kilometer, Hour>
{
  using type = Km_h;
  Combine_Units()
  {
    cout << "Combine_Units<Kilometer, Hour>" << endl;
    const auto & i = Kilometer::get_instance();
    __compound_unit_tbl.insert({ i.name,
	  Hour::get_instance().name},
      Km_h::get_instance());
  }

  static const Combine_Units cb;
};

const Combine_Units<Kilometer, Hour> Combine_Units<Kilometer, Hour>::cb;

// template <class T>
// DynList<string> expand(const T & t) { return DynList<string>(t.foo()); }

template <class T, class ... Targs>
DynList<string> expand(T head, Targs ... tail)
{
  cout << "call" << endl;
  DynList<string> ret(head.foo());
  ret.append(expand(tail...));
  return ret;
}


int main()
{
  return 0;
}
