/**
* This is the class body of the implementation of the
* interface SearchReplace for text files processing,
* using std::iftream and std::oftream to reduce the
* memory consumption.
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-17
*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>
#include "TextSearchReplace.h"

bool TextSearchReplace::fSearch(std::ifstream &in, const SearchType &searchType) const {
  if (const_cast<SearchType&>(searchType).getSearchString().empty())
    return false;

  TypeEnum typeEnum = const_cast<SearchType&>(searchType).getTypeEnum();
  char line[256] = { 0 };
  size_t len = sizeof line;
  std::string str;
  std::regex re(const_cast<SearchType&>(searchType).getSearchString());

  try {
    while (in.getline(line, len)) {
      switch (typeEnum) {
      case TypeEnum::PHRASE:
        str += line;
        if (str.length() < len - 1) {
          if (std::string(line).find(const_cast<SearchType&>(searchType).getSearchString()) != std::string::npos) {
            return true;
          }
          str = "";
          break;
        }
        else {
          continue;
        }
      case TypeEnum::PATTERN:
        str += line;
        if (str.length() < len - 1) {
          if (std::regex_search(line, re)) {
            return true;
          }
          str = "";
          break;
        }
        else {
          continue;
        }
      case TypeEnum::WILDCARD:
        //TODO
        return false;
      case TypeEnum::VARIABLE:
        //TODO
        return false;
      case TypeEnum::UNKNOWN:
        return false;
      default:
        break;
      }
    }
    in.close();
  }
  catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    if (in.is_open())
      in.close();

    return false;
  }

  return false;
}

void TextSearchReplace::fReplace(std::ifstream &in, const SearchType &searchType,
  const std::string &replaceString) {
  if (const_cast<SearchType&>(searchType).getSearchString().empty() || replaceString.empty())
    return;

  TypeEnum typeEnum = const_cast<SearchType&>(searchType).getTypeEnum();

  try {
    std::ofstream fo(getOutPath(m_path));
    char line[256] = { 0 };
    size_t len = sizeof line;
    std::string str;

    while (in.getline(line, len)) {
      switch (typeEnum) {
      case TypeEnum::PHRASE:
        str += line;
        if (str.length() < len - 1) {
          str += "\n"; // means getline() returns because it indeed encounters the line breaker,
                       // but getline() discards it and doesn't write it into line, so we add it
          phraseReplace(str, const_cast<SearchType&>(searchType).getSearchString(), replaceString, fo);
          str = "";
          break;
        }
        else {
          continue;
        }
      case TypeEnum::PATTERN:
        str += line;
        if (str.length() < len - 1) {
          str += "\n"; // means getline() returns because it indeed encounters the line breaker,
                       // but getline() discards it and doesn't write it into line, so we add it
          patternReplace(str, const_cast<SearchType&>(searchType).getSearchString(), replaceString, fo);
          str = "";
          break;
        }
        else {
          continue;
        }
      case TypeEnum::WILDCARD:
        //TODO
        return;
      case TypeEnum::VARIABLE:
        //TODO
        return;
      case TypeEnum::UNKNOWN:
        return;
      default:
        break;
      }
    }
    fo.close();
  }
  catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void TextSearchReplace::phraseReplace(std::string &line, const std::string &targetString,
  const std::string &replaceString, std::ofstream &fo) {
  replaceImpl(line, targetString, replaceString, fo, false);
}

void TextSearchReplace::patternReplace(std::string &line, const std::string &targetString,
  const std::string &replaceString, std::ofstream &fo) {
  replaceImpl(line, targetString, replaceString, fo, true);
}

void TextSearchReplace::replaceImpl(std::string &line, const std::string &targetString,
  const std::string &replaceString, std::ofstream &fo, bool isPattern) {
  char *buffer = 0;
  std::regex re(targetString);

  if (isPattern) {
    line = regex_replaceAll(line, re, replaceString);
  }
  else {
    replaceAll(line, targetString, replaceString);
  }

  try {
    buffer = new char[line.length() + 1];
    strcpy(buffer, line.c_str());   //can't direct use the returned pointer of
    fo.write(buffer, line.length());//string::c_str() that can cause exception
    delete[] buffer;
    buffer = (char*)0;
  }
  catch (std::exception &e) {
    if (buffer) {
      delete[] buffer;
    }
    std::cerr << e.what() << std::endl;
  }
}

std::string& TextSearchReplace::replaceAll(std::string &subject, const std::string &targetString,
  const std::string &replaceString) {
  size_t idx = 0;
  size_t targetLen = targetString.length();

  //string& replace (size_t pos, size_t len, const string& str);
  //size_t find (const string& str, size_t pos = 0) const noexcept;
  while (true) {
    idx = subject.find(targetString, idx);
    if (idx == std::string::npos) {
      break;
    }
    subject.replace(idx, targetLen, replaceString);
    idx += targetLen;
  }

  return subject;
}

std::string TextSearchReplace::regex_replaceAll(std::string &subject, const std::regex &pattern,
  const std::string &replaceString) {
  std::smatch smt;
  std::string out;

  while (std::regex_search(subject, smt, pattern)) {
    out += smt.prefix();
    out += std::regex_replace(smt[0].str(), pattern, replaceString);
    subject = smt.suffix();
  }
  out += subject;

  return out;
}