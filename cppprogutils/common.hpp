#pragma once
/*

 * common.hpp
 *
 *  Created on: Jul 19, 2014
 *      Author: nickhathaway
 */

#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <cstddef>
#include <utility>
#include <iterator>
#include <iostream>
#include <ostream>
#include <fstream>
#include <array>
#include <stdint.h>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <sstream>
#include <chrono>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <exception>
#include <stdexcept>
#include <memory>
#include <thread>
#include <sys/stat.h>
#include <iomanip>
#include <functional>

namespace cppprogutils {

typedef std::map<std::string, std::string> MapStrStr;
typedef std::vector<std::string> VecStr;

namespace estd {
//  added to std:: stou, stous, stos because they are not in the standard
// library
//  so that they have similar behavior to the other string conversions
inline uint32_t stou(const std::string& str, size_t* idx = 0, int base = 10) {
  auto firstConverion = std::stol(str, idx, base);
  if (firstConverion < 0 ||
      firstConverion > std::numeric_limits<uint32_t>::max()) {
    throw std::out_of_range{"stou"};
  }
  return static_cast<uint32_t>(firstConverion);
}
inline uint16_t stous(const std::string& str, size_t* idx = 0, int base = 10) {
  auto firstConverion = std::stol(str, idx, base);
  if (firstConverion < 0 ||
      firstConverion > std::numeric_limits<uint16_t>::max()) {
    throw std::out_of_range{"stous"};
  }
  return static_cast<uint16_t>(firstConverion);
}
inline int16_t stos(const std::string& str, size_t* idx = 0, int base = 10) {
  auto firstConverion = std::stoi(str, idx, base);
  if (firstConverion < std::numeric_limits<int16_t>::min() ||
      firstConverion > std::numeric_limits<int16_t>::max()) {
    throw std::out_of_range{"stos"};
  }
  return static_cast<int16_t>(firstConverion);
}
}  //namepsace estd
/**@brief Getting type by taking advantage of info available in __PRETTY_FUNCTION__
 *
 * @return The type
 */
template<typename T>
std::string typeStr(){
	std::string pf(__PRETTY_FUNCTION__);
	auto tEqualPos = pf.rfind("T = ");
	auto semiColPos = pf.find(";", tEqualPos);
	auto closeBracPos = pf.rfind("]");
	if(tEqualPos != std::string::npos &&
			closeBracPos != std::string::npos &&
			closeBracPos > tEqualPos){
		//this is for the difference in gcc and clang output of __PRETTY_FUNCTION__
		if(semiColPos != std::string::npos){
			return pf.substr(tEqualPos + 4, semiColPos - 4 - tEqualPos);
		}else{
			return pf.substr(tEqualPos + 4, closeBracPos - 4 - tEqualPos);
		}
	}else{
		return "indeterminate";
	}
}

/**@brief Getting type of a given object by taking advantage of info available in __PRETTY_FUNCTION__
 *
 * @param val The object to get the type of
 * @return The type
 */
template<typename T>
std::string typeStr(const T & val){
	return typeStr<T>();
}


/**@brief A struct to get a string of what the value of a object is
 *
 */
struct TypeName
{

	template<typename U>
	static std::string get(const U & val){
		return typeStr(val);
	}

	template<typename U>
	static std::string get(){
		return typeStr<U>();
	}

};


// add more specializations for the build-in types
template<>
inline std::string TypeName::get<int16_t>() {return "int16_t";}
template<>
inline std::string TypeName::get<int32_t>() {return "int32_t";}
template<>
inline std::string TypeName::get<int64_t>() {return "int64_t";}
template<>
inline std::string TypeName::get<uint16_t>() {return "uint16_t";}
template<>
inline std::string TypeName::get<uint32_t>() {return "uint32_t";}
#ifndef __linux__
template<>
inline std::string TypeName::get<uint64_t>() {return "uint64_t";}
#endif
template<>
inline std::string TypeName::get<size_t>() {return "size_t";}
template<>
inline std::string TypeName::get<bool>() {return "bool";}
template<>
inline std::string TypeName::get<double>() {return "double";}
template<>
inline std::string TypeName::get<long double>() {return "long double";}
template<>
inline std::string TypeName::get<float>() {return "float";}
template<>
inline std::string TypeName::get<std::string>() {return "std::string";}

/**@brief A function to return the type of the obj input in string fomat
 *
 * @param obj The obj to get the type info from
 * @return a string of the object type
 */
template<typename T>
inline std::string getTypeName(const T & obj){
	return TypeName::get<T>();
}



}  //namepsace cppprogutils
