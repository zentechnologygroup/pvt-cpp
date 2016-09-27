# ifndef CORRELATION_UTILS_H
# define CORRELATION_UTILS_H

# include <correlation.H>

inline void check_range(const string & caller, const string & op_msg,
			double val, double min, double max)
{
  if (not (val < min or val >max))
    return;

  ostringstream s;
  s << "In " << caller << ": ratio " << op_msg << " = " << val
    << " is not inside the interval [" << min << ", " << max << "]";
  throw domain_error(s.str());
}

# endif
