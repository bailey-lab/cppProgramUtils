#pragma once
/*
 * stringUtils.hpp
 *
 *  Created on: Aug 4, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/common.hpp"
namespace cppprogutils {

inline bool endsWith(const std::string& a, const std::string& b) {
  // http://stackoverflow.com/a/874160
  if (a.size() >= b.size()) {
    return (0 == a.compare(a.size() - b.size(), b.size(), b));
  }
  return false;
}

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

inline std::string boolToStr(bool convert) {
  if (convert) {
    return "true";
  } else {
    return "false";
  }
}

inline void strToUpper(std::string& str) {
  for (auto& c : str) {
    c = toupper(c);
  }
}

inline void strToLower(std::string& str) {
  for (auto& c : str) {
    c = tolower(c);
  }
}

inline std::string strToUpperRet(std::string str) {
  strToUpper(str);
  return str;
}

inline std::string strToLowerRet(std::string str) {
  strToLower(str);
  return str;
}

inline void strVecToLower(VecStr& vec) {
  for (auto& v : vec) {
    strToLower(v);
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
  std::vector<std::string> ret;
  if ("whitespace" == delim) {
    std::stringstream ss(str);
    while (!ss.eof()) {
      std::string tempStr;
      ss >> tempStr;
      ret.emplace_back(tempStr);
    }
  } else {
    if (str.find(delim) == std::string::npos) {
      ret.push_back(str);
    } else {
      std::size_t pos = str.find(delim, 0);
      std::size_t oldPos = -delim.size();
      while (pos != std::string::npos) {
        ret.emplace_back(
            str.substr(oldPos + delim.size(), pos - oldPos - delim.size()));
        oldPos = pos;
        pos = str.find(delim, pos + 1);
      }
      if (oldPos + delim.size() == str.size()) {
        if (addEmptyToEnd) {
          ret.emplace_back("");
        }
      } else {
        ret.emplace_back(str.substr(oldPos + delim.size(), str.size() - 1));
      }
    }
  }
  return ret;
}

inline bool containsSubString(const std::string& str,
                              const std::string& subString) {
  return (str.find(subString) != std::string::npos);
}

inline bool strAllDigits(const std::string& str) {
  for (const auto& c : str) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}


template <typename CON, typename FUN>
std::string longestToString(const CON& con, FUN f){
    auto t = [&f](const typename CON::value_type & e){ return to_string(f(e)); };
    auto longest = std::max_element(con.begin(), con.end(), [&t](const typename CON::value_type& a, const typename CON::value_type& b){
            return t(a).size() < t(b).size();
        });
    return t(*longest);
}

template <typename CON, typename FUN>
uint32_t paddingWidth(const CON& con, FUN f){
    return longestToString(con, f).size();
}

template<typename MAP, typename KEYFUNC, typename VALUEFUNC>
void compareColPaddings(const MAP& theMap,
		std::pair<uint32_t, uint32_t> & paddings, KEYFUNC kFunc, VALUEFUNC vFunc) {
	uint32_t keyMaxLen = paddingWidth(theMap, kFunc);
	uint32_t valueMaxLen = paddingWidth(theMap, vFunc);
	if (keyMaxLen > paddings.first) {
		paddings.first = keyMaxLen;
	}
	if (valueMaxLen > paddings.first) {
		paddings.second = keyMaxLen;
	}
}





}  //namespace cppprogutils

