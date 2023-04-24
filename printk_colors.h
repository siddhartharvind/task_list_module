#pragma once

#ifdef PRINTK_COLORS

#define RED(s)      "\e[01;31m" s "\e[0m"
#define GRN(s)      "\e[01;32m" s "\e[0m"
#define YEL(s)      "\e[01;33m" s "\e[0m"
#define MAG(s)      "\e[01;35m" s "\e[0m"
#define CYN(s)      "\e[01;36m" s "\e[0m"
#define WHT(s)      "\e[01;37m" s "\e[0m"
#define REDBG(s)    "\e[01;41m" s "\e[0m"
#define BLUBG(s)    "\e[01;44m" s "\e[0m"

#else // PRINTK_COLORS

#define RED(s)      s
#define GRN(s)      s
#define YEL(s)      s
#define MAG(s)      s
#define CYN(s)      s
#define WHT(s)      s
#define REDBG(s)    s
#define BLUBG(s)    s

#endif // PRINTK_COLORS
