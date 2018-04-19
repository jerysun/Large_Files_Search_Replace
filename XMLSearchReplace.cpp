/**
* This is an implementation of the interface SearchReplace
* for large XML files processing, using iftream, oftream and
* buffer to reduce the memory consumption, using XMLPartEnum
* to guarantee only the proper part will be searched and rep-
* laced.
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-18
*/

#include "stdafx.h"
#include "XMLSearchReplace.h"
#include <iostream>
#include <fstream>
#include <regex>

XMLSearchReplace::XMLSearchReplace(const std::string &path, const SearchType &searchType,
  const XMLPartEnum xmlPartEnum) : TextSearchReplace(path, searchType),
                                   m_xmlPartEnum(xmlPartEnum)
{
}

bool XMLSearchReplace::fSearch(std::ifstream &in, const SearchType &searchType) const {
  const std::string searchString = const_cast<SearchType&>(searchType).getSearchString();

  if (m_xmlPartEnum == XMLPartEnum::UNKNOWN || searchString.empty()) {
    return false;
  }

  switch (m_xmlPartEnum) {
  case XMLPartEnum::ELEMENT_NAME:
    if (searchString.find("<") == std::string::npos) {
      return false;
    }
    break;
  case XMLPartEnum::ATTRIBUTE:
    if (searchString.find("=") == std::string::npos) {
      return false;
    }
    break;
  case XMLPartEnum::UNKNOWN:
    return false;
  default:
    break;
  }

  return TextSearchReplace::fSearch(in, searchType);
}

void XMLSearchReplace::fReplace(std::ifstream &in, const SearchType &searchType,
  const std::string &replaceString) {
    const std::string searchString = const_cast<SearchType&>(searchType).getSearchString();
    
      if (m_xmlPartEnum == XMLPartEnum::UNKNOWN ||
        searchString.empty() || replaceString.empty()) {
        return;
      }

      std::string newEndTag;
      std::string oldEndTag;
      switch (m_xmlPartEnum) {
      case XMLPartEnum::ELEMENT_NAME:
        if (searchString.find("<") == std::string::npos ||
            replaceString.find("<") == std::string::npos) {
          return;
        }

        newEndTag = replaceString;
        oldEndTag = searchString;
        newEndTag.insert(1, "/");
        oldEndTag.insert(1, "/");
        break;
      case XMLPartEnum::ATTRIBUTE:
        if (searchString.find("=") == std::string::npos || 
            replaceString.find("=") == std::string::npos) {
          return;
        }
        break;
      case XMLPartEnum::UNKNOWN:
        return;
      default:
        break;
      }

      TypeEnum typeEnum = const_cast<SearchType&>(searchType).getTypeEnum();

      try {
        std::ofstream fo(getOutPath(getPath()));
        char line[256] = { 0 };
        size_t len = sizeof line;
        std::string str;

        while (in.getline(line, len)) {
          switch (typeEnum) {
          case TypeEnum::PHRASE:
            str += line;
            if (str.length() < len - 1) {
              str += "\n";
              if (str.find(searchString) != std::string::npos) {
                phraseReplace(str, searchString, replaceString, fo);
              }
              else if (!oldEndTag.empty() && !newEndTag.empty() && 
                       str.find(oldEndTag) != std::string::npos) {
                phraseReplace(str, oldEndTag, newEndTag, fo);
              }
              else {
                lineReplace(str, fo);
              }
              str = "";
              break;
            } else {
              continue;
            }
          case TypeEnum::PATTERN:
            str += line;
            if (str.length() < len - 1) {
              str += "\n";
              if (std::regex_search(str, std::regex(searchString))) {
                patternReplace(str, searchString, replaceString, fo);
              }
              else if (!oldEndTag.empty() && !newEndTag.empty()) {
                if (std::regex_search(str, std::regex(oldEndTag))) {
                  patternReplace(str, oldEndTag, newEndTag, fo);
                }
                else {
                  lineReplace(str, fo);
                }
              }
              else {
                lineReplace(str, fo);
              }

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
      }
      catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
      }
}

void XMLSearchReplace::lineReplace(std::string &line, std::ofstream &fo) {
  char *buffer = new char[line.length() + 1];
  strcpy(buffer, line.c_str());

  try {
    fo.write(buffer, line.length());
    delete[] buffer;
    buffer = (char*)0;
  }
  catch (std::exception &e) {
    if (buffer) {
      delete[] buffer;
    }

    throw e;
  }
}