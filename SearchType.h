/**
* This is a basic search unit
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-16
*/

#pragma once

#include <string>
#include "SearchReplaceEnums.h"

class SearchType {
public:
  SearchType() {}
  explicit SearchType(const TypeEnum typeEnum, const std::string &searchString) :
  m_typeEnum(typeEnum), m_searchString(searchString)
  {}

  TypeEnum getTypeEnum() {
    return m_typeEnum;
  }

  std::string getSearchString() {
    return m_searchString;
  }
private:
  TypeEnum m_typeEnum;
  std::string m_searchString;
};