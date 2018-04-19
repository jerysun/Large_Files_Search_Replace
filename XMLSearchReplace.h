/**
* This is an implementation of the interface SearchReplace
* for large XML files processing, using iftream, oftream and
* buffer to reduce the memory consumption, using XMLPartEnum
* to guarantee only the proper part will be searched and rep-
* laced.
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-18
*/

#pragma once
#include "TextSearchReplace.h"

class XMLSearchReplace : public TextSearchReplace
{
public:
  XMLSearchReplace() {}
  explicit XMLSearchReplace(const std::string &path, const SearchType &searchType,
    const XMLPartEnum xmlPartEnum);
  virtual ~XMLSearchReplace() {}
  
  virtual bool fSearch(std::ifstream &in, const SearchType &searchType) const;
  virtual void fReplace(std::ifstream &in, const SearchType &searchType,
    const std::string &replaceString);

  XMLPartEnum getXMLPartEnum() const {
    return m_xmlPartEnum;
  }

private:
  void lineReplace(std::string &line, std::ofstream &fo);
private:
  const XMLPartEnum m_xmlPartEnum = XMLPartEnum::UNKNOWN;
};

