/**
* This is a command line utility to demonstrate the usage of
* the search and replace functionality applied to large text
* and xml files.
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-18
*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include <memory>
#include "TextSearchReplace.h"
#include "XMLSearchReplace.h"
#include "str_util.h"

int main()
{
  std::string path;
  std::string searchString;
  int type = 0;
  FileType fileType = FileType::UNKNOWN;
  XMLPartEnum xmlPartEnum = XMLPartEnum::UNKNOWN;
  size_t idx = std::string::npos;

  std::cout << "Please input the file path:\n";
  while (true) {
    std::cin >> path;
    path = str_util::trim(path);

    if ((idx = path.find_last_of('.')) == std::string::npos ||
      idx == path.length() - 1) {
      std::cout << "Please input the file path:\n";
      continue;
    }
    else {
      break;
    }
  }

  std::string extensionName = path.substr(++idx);
  str_util::lowercase(extensionName);
  // check the file type based on the extension name
  if (str_util::lowercase(extensionName).find("txt") != std::string::npos) {
    fileType = FileType::TEXT;
  }
  else if (extensionName.find("xml") != std::string::npos) {
    fileType = FileType::XML;
    std::cout << "Please input the XML part type(1: element name 2. attribute 3. text node), just input the number:\n";

    while (true) {
      std::cin >> type;
      while (std::cin.fail()) {
        std::cerr << "Error!You must input a number : 1 or 2 or 3!\n";
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> type;
      }

      if (type == 1 || type == 2 || type == 3) {
        switch (type) {
        case 1:
          xmlPartEnum = XMLPartEnum::ELEMENT_NAME;
          break;
        case 2:
          xmlPartEnum = XMLPartEnum::ATTRIBUTE;
          break;
        case 3:
          xmlPartEnum = XMLPartEnum::TEXT_NODE;
          break;
        default:
          break;
        }
        break;
      }
      else {
        std::cout << "Please input the XML part type(1: element name 2. attribute 3. text node), just input the number:\n";
      }
    }
  }
  else {
    fileType = FileType::TEXT;//TODO, now just set it as default if it's not xml
  }

  std::cout << "Please input the string you want to search:\n";
  std::cin >> searchString;
  searchString = str_util::trim(searchString);

  switch (xmlPartEnum) {
  case XMLPartEnum::ELEMENT_NAME:
    while (searchString.find("<") == std::string::npos) {
      std::cerr << "Error! The xml element name must be prefixed by a <\n"
        << "Please input the string you want to search:\n";
      std::cin >> searchString;
      searchString = str_util::trim(searchString);
    }

    if (searchString[searchString.length() - 1] == '>') { //user friendly for open tag
      searchString = searchString.substr(0, searchString.length() - 1);
    }
    break;
  case XMLPartEnum::ATTRIBUTE:
    while (searchString.find("=") == std::string::npos) {
      std::cerr << "Error! The xml attribute must contain a =\n"
        << "Please input the string you want to search:\n";
      std::cin >> searchString;
      searchString = str_util::trim(searchString);
    }
    break;
  default:
    break;
  }

  std::cout << "Please input the search type(1: Text phrease 2. Pattern), just input the number:\n";
  while (true) {
    std::cin >> type;
    while (std::cin.fail()) {
      std::cerr << "Error! You must input a number either 1 or 2!\n";
      std::cin.clear();
      std::cin.ignore(256, '\n');
      std::cin >> type;
    }

    if (type == 1 || type == 2) {
      break;
    }
    else {
      std::cout << "Please input the search type(1: Text phrease 2. Pattern), just input the number:\n";
    }
  }

  SearchType searchType(type == 1 ? TypeEnum::PHRASE : TypeEnum::PATTERN, searchString);

  std::cout << "Please input the replacement string(if you input no, that means you skip the replace operation) :\n";
  std::string replaceString;
  std::cin >> replaceString;
  replaceString = str_util::trim(replaceString);

  if (replaceString.compare("no")) {
    switch (xmlPartEnum) {
    case XMLPartEnum::ELEMENT_NAME:
      while (replaceString.find("<") == std::string::npos) {
        std::cerr << "Error! The xml element name must be prefixed by a <\n"
          << "Please input the replacement string if you want:\n";
        std::cin >> replaceString;
        replaceString = str_util::trim(replaceString);
      }
      break;
    case XMLPartEnum::ATTRIBUTE:
      while (replaceString.find("=") == std::string::npos) {
        std::cerr << "Error! The xml attribute must contain a =\n"
          << "Please input the replacement string if you want:\n";
        std::cin >> replaceString;
        replaceString = str_util::trim(replaceString);
      }
      break;
    default:
      break;
    }
  }

  std::unique_ptr<SearchReplace> searchReplace;
  if (fileType == FileType::TEXT) {
    searchReplace.reset(new TextSearchReplace(path, searchType));
  }
  else if (fileType == FileType::XML) {
    searchReplace.reset(new XMLSearchReplace(path, searchType, xmlPartEnum));
  }
  else {
    return -1;
  }

  bool found = false;
  // Do search
  try {
    std::ifstream in(path);
    if (found = searchReplace->fSearch(in, searchType)) {
      std::cout << "\"" << searchType.getSearchString() << "\" is found in file: " << path << std::endl;
    }
    else {
      std::cout << "\"" << searchType.getSearchString() << "\" is NOT found in file: " << path << std::endl;
    }
  }
  catch (std::exception &e) {
    std::cerr << e.what();
  }

  // Do replace
  if (found && replaceString.find("no") == std::string::npos) {
    try {
      std::ifstream in(path);
      searchReplace->fReplace(in, searchType, replaceString);
      std::cout << "Replacement is done, please check the new generated file: " << searchReplace->getOutPath(path) << std::endl;
    }
    catch (std::exception &e) {
      std::cerr << e.what();
    }
  }
}