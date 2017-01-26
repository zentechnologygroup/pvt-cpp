# ifndef ZEN_EXCEPTIONS_H
# define ZEN_EXCEPTIONS_H

# include <exception>
# include <sstream>

using namespace std;

struct ZenException : public exception
{
  const size_t line_number = 0;
  const string file_name = "No defined";
  const string type = "No defined";
  const string msg = "No defined";

  ZenException() {}

  ZenException(const size_t line_number,
	       const string & file_name,
	       const string & type,
	       const string & msg)
    : line_number(line_number), file_name(file_name), type(type), msg(msg) {}

  virtual const char * what() const noexcept
  {
    ostringstream s;
    s << file_name << "|" << line_number << "|" << type << "|" << msg;
    return s.str().c_str();
  }
};

struct UnitOutOfRange : public ZenException
{

};

# endif
