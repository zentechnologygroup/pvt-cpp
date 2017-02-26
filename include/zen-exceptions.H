# ifndef ZEN_EXCEPTIONS_H
# define ZEN_EXCEPTIONS_H

# include <exception>
# include <sstream>

# include <utils.H>

using namespace std;

struct ZenException : public std::runtime_error
{
  const size_t line_number = 0;
  const string file_name = "No defined";
  const string type = "No defined";
  const string msg = "No defined";

protected:

  string category_msg = "No defined";

private:

  static string make_what(const string & category_msg,
			  size_t line_number,
			  const string & file_name,
			  const string & type,
			  const string & msg)
  {
    ostringstream s;
    s << category_msg << "|" << file_name << "|" << line_number
      << "|" << type << "|" << msg;
    return s.str();
  }

public:

  ZenException() : runtime_error(msg) {}

  ZenException(const string & category_msg,
	       const size_t line_number,
	       const string & file_name,
	       const string & type,
	       const string & msg)
    : runtime_error(make_what(category_msg, line_number, file_name, type, msg)),
      line_number(line_number), file_name(file_name), type(type) {}
};

# define DEFINE_ZEN_EXCEPTION(name, category_msg)	\
  struct name : public ZenException			\
  {							\
    name(const size_t line_number,			\
	 const string & file_name,			\
	 const string & type,				\
	 const string & msg)						\
      : ZenException(#category_msg, line_number, file_name, type, msg) {} \
  };
    
# define ZENTHROW(type, msg) throw type(__LINE__, __FILE__, #type, msg)

DEFINE_ZEN_EXCEPTION(MinMaxReversed,
		     "minimum value is greater than maximum value");
DEFINE_ZEN_EXCEPTION(InvalidCsvHeader, "Invalid csv header");
DEFINE_ZEN_EXCEPTION(InvalidCsvRow, "Invalid csv row");
DEFINE_ZEN_EXCEPTION(InvalidValue, "Invalid value");

# endif
