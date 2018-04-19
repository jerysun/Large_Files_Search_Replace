/*
* filename: str_util.h
* describe: Frequently used routines for string
* author:   Jerry Sun <jerysun0818@gmail.com>
* date:     Octobler 1, 2017
* remark:   No dependency. Handy
* web:      http://nl.linkedin.com/in/jerysun
* blog:     https://sites.google.com/site/geekssmallworld
*/

#ifndef STR_UTIL_H
#define STR_UTIL_H

#include <string>

namespace str_util {
std::string trim(const std::string &s);
std::string trimLeft(const std::string &s);
std::string trimRight(const std::string &s);
std::string& lowercase(std::string &s);
std::string& uppercase(std::string &s);
}

#endif //STR_UTIL_H