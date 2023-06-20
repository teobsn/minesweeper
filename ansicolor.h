#include <string>

const std::string numberMapColor[9] = {
    "\033[0m",         // 0 bombs (ansi reset)
    "\033[1m\033[34m", // 1 bomb  (ansi blue)
    "\033[1m\033[32m", // 2 bombs (ansi green)
    "\033[1m\033[31m", // 3 bombs (ansi red)
    "\033[1m\033[35m", // 4 bombs (ansi magenta)
    "\033[1m\033[33m", // 5 bombs (ansi yellow)
    "\033[1m\033[36m", // 6 bombs (ansi cyan)
    "\033[2m\033[31m", // 7 bombs (ansi dim red)
    "\033[2m\033[35m", // 8 bombs (ansi dim magenta)
};

const std::string ansiReset = "\033[0m";
const std::string ansiDimWhite = "\033[2m\033[37m";