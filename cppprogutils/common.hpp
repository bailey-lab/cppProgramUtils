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
#include <vector>
#include <array>
#include <stdint.h>
#include <map>
#include <unordered_set>
#include <string>
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
}

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


template<class T>
struct TypeName
{
static std::string get() { return T::typeName(); }
};
// add more specializations for the build-in types
template<>
inline std::string TypeName<int16_t>::get() {return "int16_t";}
template<>
inline std::string TypeName<int32_t>::get() {return "int32_t";}
template<>
inline std::string TypeName<int64_t>::get() {return "int64_t";}
template<>
inline std::string TypeName<uint16_t>::get() {return "uint16_t";}
template<>
inline std::string TypeName<uint32_t>::get() {return "uint32_t";}
template<>
inline std::string TypeName<uint64_t>::get() {return "uint64_t";}
#ifndef __linux__
template<>
inline std::string TypeName<size_t>::get() {return "size_t";}
#endif
template<>
inline std::string TypeName<bool>::get() {return "bool";}
template<>
inline std::string TypeName<double>::get() {return "double";}
template<>
inline std::string TypeName<long double>::get() {return "long double";}
template<>
inline std::string TypeName<float>::get() {return "float";}
template<>
inline std::string TypeName<std::string>::get() {return "string";}

template<typename T>
inline std::string getTypeName(const T & obj){
	return TypeName<T>::get();
}


}  //namepsace estd
