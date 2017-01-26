# ifndef ZEN_EXCEPTIONS_H
# define ZEN_EXCEPTIONS_H

# include <exception>
# include <sstream>

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

private:

  static const size_t buf_len = 50;

  char buffer[buf_len];

  virtual const char * what() const noexcept
  {
    size_t n = buf_len;
    const char * ptr = &buffer[0];
    
  }
};

# define DEFINE_ZEN_EXCEPTION_CTOR(name, category_msg)	\
  name(const string & category_msg,			\
       const size_t line_number,			\
       const string & file_name,			\
       const string & type,				\
       const string & msg)				\
    : ZenException(category_msg, line_number, file_name, type, msg)	\
  {									\
    


struct UnitOutOfRange : public ZenException
{
  UnitOutOfRange
};

# endif
