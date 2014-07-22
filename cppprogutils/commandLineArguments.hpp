#pragma once
//
//  commandLineArguments.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 9/29/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "cppprogutils/utils.hpp"
namespace cppprogutils {

class commandLineArguments {

 public:
  commandLineArguments() {}
  commandLineArguments(int argc, char* argv[])
      : arguments_(convertCommandLineArguments(argc, argv)) {
    std::stringstream hold;
    logRunArgumnets(hold, argc, argv);
    commandLine_ = hold.str();
  }
  commandLineArguments(const MapStrStr& inputCommands)
      : arguments_(inputCommands) {
    lookForOption(commandLine_, "-commandline");
  }
  //members

  MapStrStr arguments_;
  std::string commandLine_;

 private:
  std::string& lowerCaseGetArguments(const std::string& str) ;

 public:
  std::string& operator[](const std::string& str);
  // string
  bool lookForOption(std::string& option, const std::string& flag);
  // bool
  bool lookForOption(bool& option, const std::string& flag) ;
  // bool false
  bool lookForOptionFalse(bool& option, const std::string& flag) ;
  // int16_t
  bool lookForOption(int16_t& option, const std::string& flag) ;
  // int32_t
  bool lookForOption(int32_t& option, const std::string& flag) ;
  // int64_t
  bool lookForOption(int64_t& option, const std::string& flag);
  // size_t
  bool lookForOption(size_t& option, const std::string& flag) ;
  // uint16_t
  bool lookForOption(uint16_t& option, const std::string& flag) ;
  // uint32_t
  bool lookForOption(uint32_t& option, const std::string& flag) ;
  // uint64_t
  bool lookForOption(uint64_t& option, const std::string& flag) ;
  // double
  bool lookForOption(double& option, const std::string& flag);
  // long double
  bool lookForOption(long double& option, const std::string& flag);
  // float
  bool lookForOption(float& option, const std::string& flag);

  //own conversion
  template<typename T>
  bool lookForOption(T& option, const std::string & flag, std::function<T(std::string)> fun){
  	std::cout << "calling this" << std::endl;
    if (containsFlagCaseInsensitive(flag)) {
      option = fun(lowerCaseGetArguments(flag));
      return true;
    } else {
      return false;
    }
  }



  bool containsFlagCaseSensitive(const std::string& flag);
  bool containsFlagCaseInsensitive(const std::string& flag);

  static void logRunArgumnets(std::ostream& out, int argc, char* argv[]);
  static void logRunArgumnets(std::ostream& out, const std::string& commandLine);
  static MapStrStr convertCommandLineArguments(int argc, char* argv[]);

  uint32_t numberOfCommands() ;
};

std::string& commandLineArguments::lowerCaseGetArguments(const std::string& str) {
  if (arguments_[stringToLowerReturn(str)] == "") {
    std::cout << "Argument " << str
              << " requires an option but none was given" << std::endl;
    exit(1);
  }
  return arguments_[stringToLowerReturn(str)];
}

std::string& commandLineArguments::operator[](const std::string& str) {
  return lowerCaseGetArguments(str);
}

// string
bool commandLineArguments::lookForOption(std::string& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = lowerCaseGetArguments(flag);
    return true;
  } else {
    return false;
  }
}
// bool
bool commandLineArguments::lookForOption(bool& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = true;
    return true;
  } else {
    return false;
  }
}
// bool false
bool commandLineArguments::lookForOptionFalse(bool& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = false;
    return true;
  } else {
    return false;
  }
}

// int16_t
bool commandLineArguments::lookForOption(int16_t& option, const std::string& flag){
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// int32_t
bool commandLineArguments::lookForOption(int32_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// int_64t
bool commandLineArguments::lookForOption(int64_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stol(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// uint16_t
bool commandLineArguments::lookForOption(uint16_t& option, const std::string& flag){
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// uint32_t
bool commandLineArguments::lookForOption(uint32_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}

// uint64_t
bool commandLineArguments::lookForOption(uint64_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoull(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// size_t
bool commandLineArguments::lookForOption(size_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoull(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}

// double
bool commandLineArguments::lookForOption(double& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stod(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// long double
bool commandLineArguments::lookForOption(long double& option, const std::string& flag){
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stold(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// float
bool commandLineArguments::lookForOption(float& option, const std::string& flag){
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stof(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
bool commandLineArguments::containsFlagCaseSensitive(const std::string& flag) {
  if (arguments_.find(flag) != arguments_.end()) {
    return true;
  } else {
    return false;
  }
}
bool commandLineArguments::containsFlagCaseInsensitive(const std::string& flag) {
  if (arguments_.find(stringToLowerReturn(flag)) != arguments_.end()) {
    return true;
  } else {
    return false;
  }
}


uint32_t commandLineArguments::numberOfCommands() {
  int offSet = 0;
  if (arguments_.find("-program") != arguments_.end()) {
    ++offSet;
  }
  if (arguments_.find("-commandline") != arguments_.end()) {
    ++offSet;
  }
  return arguments_.size() - offSet;
}

void commandLineArguments::logRunArgumnets(std::ostream& out, int argc, char* argv[]){
  std::string directory = get_cwd();

  out << "Command ran from " << directory << std::endl;
  for (int args = 0; args < argc; ++args) {
    out << argv[args] << " ";
  }
  out << std::endl << std::endl;
}
void commandLineArguments::logRunArgumnets(std::ostream& out, const std::string& commandLine){
	out << commandLine;
}

MapStrStr commandLineArguments::convertCommandLineArguments(int argc, char* argv[]){
  MapStrStr storage;
  for (int i = 1; i < argc; i++) {
    std::string nextParamRaw = argv[i];
    uint32_t equCount = std::count(nextParamRaw.begin(), nextParamRaw.end(), '=');
    if(equCount > 0){
    	if(equCount > 1){
    		std::cout << changeColor("196") << std::endl;
    		std::cout << "Error can only have one equal sign when setting flag" << std::endl;
    		std::cout << "eg. -flag=this , can't have -flag=this=that" << std::endl;
    		std::cout << "not " << nextParamRaw << endAllAttributes() << std::endl;
    		exit(1);
    	}else{
    		VecStr toks = tokenizeString(nextParamRaw, "=");
    		storage.insert(std::make_pair(stringToLowerReturn(toks[0]), toks[1]));
    	}
    }else{
      std::string nextParam = stringToLowerReturn(argv[i]);
      if (storage.find(nextParam) != storage.end()) {
        std::cout << "Error, already have " << nextParam << std::endl;
        std::cout << "Check if you entered it in twice" << std::endl;
        exit(1);
      }

      if (argv[i][0] == '-') {
        if (i + 1 >= argc) {
          storage.insert(std::make_pair(stringToLowerReturn(argv[i]), ""));
        } else {
          if (argv[i + 1][0] == '-') {
            storage.insert(std::make_pair(stringToLowerReturn(argv[i]), ""));
          } else {
            storage.insert(
                std::make_pair(stringToLowerReturn(argv[i]), argv[i + 1]));
            ++i;
          }
        }
      } else {
        storage.insert(std::make_pair(stringToLowerReturn(argv[i]), ""));
      }
    }

  }
  return storage;
}
}  // namespace cppprogutils


