#pragma once
/*
 * stringUtils.hpp
 *
 *  Created on: Aug 4, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/common.hpp"
namespace cppprogutils {

template <typename T>
std::string to_string(T obj) {
  std::stringstream ss;
  ss << obj;
  return ss.str();
}
template <>
inline std::string to_string<bool>(bool obj) {
	if(obj){
		return "true";
	}else{
		return "false";
	}
}

template <typename T>
std::string leftPadNumStr(T num, T highestNumber = 10) {
  std::stringstream ans;
  if (num == 0) {
    ans << std::string(log10(highestNumber), '0');
  } else {
    ans << std::string(((int)log10(highestNumber) - (int)log10(num)), '0');
  }
  ans << num;
  return ans.str();
}

inline std::string convertBoolToString(bool convert) {
  if (convert) {
    return "true";
  } else {
    return "false";
  }
}



inline void stringToUpper(std::string& str) {
  for (auto& c : str) {
    c = toupper(c);
  }
}

inline void stringToLower(std::string& str) {
  for (auto& c : str) {
    c = tolower(c);
  }
}

inline std::string stringToUpperReturn(std::string str) {
  stringToUpper(str);
  return str;
}

inline std::string stringToLowerReturn(std::string str) {
  stringToLower(str);
  return str;
}

inline void changeStringVectorToLowerCase(VecStr& vec) {
  for (auto& v : vec) {
    stringToLower(v);
  }
}

template <typename T>
std::string vectorToString(const std::vector<T>& vectorToConvert,
                           const std::string& delim = " ") {
  if (vectorToConvert.empty()) {
    return "";
  }
  std::stringstream tempStringStream;
  copy(vectorToConvert.begin(), vectorToConvert.end(),
       std::ostream_iterator<T>(tempStringStream, delim.c_str()));
  std::string returnString = tempStringStream.str().c_str();
  returnString.erase(returnString.size() - (int)delim.size());
  return returnString;
}

inline std::vector<std::string> tokenizeString(const std::string& str,
                                               const std::string& delim,
                                               bool addEmptyToEnd = false) {
  std::vector<std::string> output;
  if ("whitespace" == delim) {
    std::stringstream tempStream(str);
    while (!tempStream.eof()) {
      std::string tempName;
      tempStream >> tempName;
      output.emplace_back(tempName);
    }
  } else {
    if (str.find(delim.c_str()) == std::string::npos) {
      output.push_back(str);
    } else {
      std::size_t pos = str.find(delim, 0);
      std::size_t oldPos = -delim.size();
      while (pos != std::string::npos) {
        output.push_back(
            str.substr(oldPos + delim.size(), pos - oldPos - delim.size()));
        oldPos = pos;
        pos = str.find(delim, pos + 1);
      }
      if (oldPos + delim.size() == str.size()) {
        if (addEmptyToEnd) {
          output.push_back("");
        }
      } else {
        output.push_back(str.substr(oldPos + delim.size(), str.size() - 1));
      }
    }
  }
  return output;
}

inline bool containsSubString(const std::string& str,
                              const std::string& subString) {
  return (str.find(subString) != std::string::npos);
}

inline bool stringContainsAllDigits(const std::string& str) {
  for (const auto& c : str) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}


inline std::string centerText(const std::string& text, uint32_t maxWidth) {
  uint32_t halfWay = std::round(maxWidth / 2.0);
  uint32_t halfText = std::round(text.size() / 2.0);
  return std::string(halfWay - halfText, ' ') + text;
}

inline std::string boldText(const std::string& title,
                            const std::string& colorCode) {
  return "\033[1;" + colorCode + "m" + title + "\033[0m";
}

inline std::string boldBlackText(const std::string& title) {
  return boldText(title, "30");
}
inline std::string centerTitle(const std::string& text, uint32_t maxWidth) {
  return centerText(boldBlackText(text), maxWidth);
}
inline std::string addFlashingColor(const std::string& colorCode) {
  return "\033[38;5;" + colorCode + ";147;5" + "m";
}
inline std::string addFlashingColor(uint32_t colorCode) {
  return addFlashingColor(to_string(colorCode));
}
inline std::string addAnsiAtr(const std::string& atr) {
  return "\033[" + atr + "m";
}
inline std::string changeBackground(const std::string& colorCode) {
  return "\033[48;5;" + colorCode + "m";
}
inline std::string changeColor(const std::string& colorCode) {
  return "\033[38;5;" + colorCode + "m";
}
inline std::string changeColor(uint32_t colorCode) {
  return changeColor(to_string(colorCode));
}
inline std::string changeBackground(uint32_t colorCode) {
  return changeBackground(to_string(colorCode));
}

inline std::string endAllAttributes(const std::string& add = "") {
  std::string output = "\033[0m";
  if (add != "") {
    output.append("\033[" + add + "m");
  }
  return output;
}



}  //namespace cppprogutils

