/**
* This is the class definition of the implementation of
* the interface SearchReplace for text files processing,
* using std::iftream and std::oftream to reduce the
* memory consumption.
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-17
*/

#pragma once
#include "SearchReplace.h"
#include "SearchReplaceEnums.h"
#include <regex>

class TextSearchReplace : public SearchReplace
{
public:
  TextSearchReplace() {}
  explicit TextSearchReplace(const std::string &path, const SearchType &searchType) :
    m_path(path), m_searchType(searchType)
  {}
  virtual ~TextSearchReplace() {}

  std::string getPath() const {
    return m_path;
  }

  const SearchType& getSearchType() const {
    return m_searchType;
  }

  virtual bool fSearch(std::ifstream &in, const SearchType &searchType) const;
  virtual void fReplace(std::ifstream &in, const SearchType &searchType,
                        const std::string &replaceString);

  void phraseReplace(std::string &line, const std::string &targetString,
                     const std::string &replaceString, std::ofstream &fo);

  void patternReplace(std::string &line, const std::string &targetString,
                      const std::string &replaceString, std::ofstream &fo);

  static std::string& replaceAll(std::string &subject, const std::string &targetString,
                                 const std::string &replaceString);

  static std::string regex_replaceAll(std::string &subject, const std::regex &pattern,
                                      const std::string &replaceString);

private:
  void replaceImpl(std::string &line, const std::string &targetString,
                   const std::string &replaceString, std::ofstream &fo, bool isPattern);
private:
  const std::string m_path;
  const SearchType m_searchType;
};