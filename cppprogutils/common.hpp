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

namespace std{
//  added to std:: stou, stous, stos because they are not in the standard library
//  so that they have similar behavior to the other string conversions
inline uint32_t stou(const string& str, size_t* idx = 0, int base = 10){
	auto firstConverion = std::stol(str, idx, base);
	if(firstConverion < 0 || firstConverion > std::numeric_limits<uint32_t>::max()){
		throw out_of_range {"stou"};
	}
	return static_cast<uint32_t>(firstConverion);
}
inline uint16_t stous(const string& str, size_t* idx = 0, int base = 10){
	auto firstConverion = std::stol(str, idx, base);
	if(firstConverion < 0 || firstConverion > std::numeric_limits<uint16_t>::max()){
		throw out_of_range {"stous"};
	}
	return static_cast<uint16_t>(firstConverion);
}
inline int16_t stos(const string& str, size_t* idx = 0, int base = 10){
	auto firstConverion = std::stoi(str, idx, base);
	if(firstConverion < std::numeric_limits<int16_t>::min()
			|| firstConverion > std::numeric_limits<int16_t>::max()){
		throw out_of_range {"stos"};
	}
	return static_cast<int16_t>(firstConverion);
}
}
