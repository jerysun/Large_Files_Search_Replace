/**
* This interface is the abstract of the search and replace function,
* it adopts the InputStream aiming to reduce the memory consumption to
* hundreds of mega bytes when processing the large file which could be
* up to several giga bytes.
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-16
*/

#pragma once

#include <fstream>
#include "SearchType.h"

struct SearchReplace {
  virtual bool fSearch(std::ifstream &in, const SearchType &searchType) const = 0;
  virtual void fReplace(std::ifstream &in, const SearchType &searchType, const std::string &replaceString) = 0;

  virtual std::string getOutPath(const std::string &path) const {
    std::string outPath = "";
    size_t idx = path.find_last_of('.');

    if (idx == std::string::npos) {
      return "";
    }

    outPath = path.substr(0, idx);
    outPath.append("_out");
    outPath.append(path.substr(idx));
    return outPath;
  }
};