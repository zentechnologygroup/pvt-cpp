# ifndef TERM_COLORS_H
# define TERM_COLORS_H 1

# include <strings.h>

/* FOREGROUND */
# define COLOR_RESET  "\x1B[0m"
# define COLOR_RED  "\x1B[31m"
# define COLOR_GRN  "\x1B[32m"
# define COLOR_YEL  "\x1B[33m"
# define COLOR_BLU  "\x1B[34m"
# define COLOR_MAG  "\x1B[35m"
# define COLOR_CYN  "\x1B[36m"
# define COLOR_WHT  "\x1B[37m"

# define COLOR_FRED(x) COLOR_RED x COLOR_RESET
# define COLOR_FGREEN(x) COLOR_GRN x COLOR_RESET
# define COLOR_FYELLOW(x) COLOR_YEL x COLOR_RESET
# define COLOR_FBLUE(x) COLOR_BLU x COLOR_RESET
# define COLOR_FMAGENTA(x) COLOR_MAG x COLOR_RESET
# define COLOR_FCYAN(x) COLOR_CYN x COLOR_RESET
# define COLOR_FWHITE(x) COLOR_WHT x COLOR_RESET

# define COLOR_BOLD(x)	"\x1B[1m" x RST
# define COLOR_UNDL(x)	"\x1B[4m" x RST

/*
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");
*/

# endif // TERM_COLORS_H

