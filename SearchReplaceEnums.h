/**
* These are enums convenient for searh and replace operation
* @author jerysun (jerysun0818@gmail.com)
* @version 1.00 2018-04-16
*/

#pragma once

// Used for customized search
enum class TypeEnum {
  PHRASE,
  PATTERN,
  WILDCARD,
  VARIABLE,
  UNKNOWN
};

enum class FileType {
  TEXT,
  XML,
  HTML,
  UNKNOWN
};

enum class XMLPartEnum {
  ELEMENT_NAME,   //Must be prefixed by <, don't use the > at the end. For example: <Server
  ATTRIBUTE,      //Must be in the format x="y"
  TEXT_NODE,
  UNKNOWN
};