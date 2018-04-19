/*
* filename: str_util.cpp
* describe: Frequently used routines for string
* author:   Jerry Sun <jerysun0818@gmail.com>
* date:     Octobler 1, 2017
* remark:   No dependency. Handy
* web:      http://nl.linkedin.com/in/jerysun
* blog:     https://sites.google.com/site/geekssmallworld
*/

#include "stdafx.h"
#include <algorithm>
#include <string>
#include <locale>

namespace str_util {
  std::string trimLeft(const std::string &s) {
    auto temp = s;
    temp.erase(std::begin(temp),
      std::find_if(std::begin(temp), std::end(temp),
        [](char c) {
      return !std::isspace(c, std::locale());
    }));
    return temp;
  }

  std::string trimRight(const std::string &s) {
    auto temp = s;
    temp.erase(std::find_if(std::rbegin(temp), std::rend(temp),
      [](char c) {
      return !std::isspace(c, std::locale());
    }).base(),
      std::end(temp));
    return temp;
  }

  std::string trim(const std::string &s) {
    return trimLeft(trimRight(s));
  }

  std::string& lowercase(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
  }

  std::string& uppercase(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
  }
}