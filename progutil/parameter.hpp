#pragma once
//
//  parameter.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 9/29/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "progutil/utils.hpp"
namespace progutil {

class parameter {
 public:
	/*
	 * Constructors
	 */
  // bool constructor
  parameter(const std::string& parName, bool inValue, bool commandLine) {
  	name_ = parName;
  	value_ = convertBoolToString(inValue);
    if (commandLine) {
    	defaultOrCommandline_ = "commandLine";
    } else {
    	defaultOrCommandline_ = "default";
    }
  }
  // string contstructor
  parameter(const std::string& parName, const std::string& inValue,
            bool commandLine) {
    if (inValue == "") {
    	value_ = "none";
    } else {
    	value_ = inValue;
    }
    name_ = parName;
    if (commandLine) {
    	defaultOrCommandline_ = "commandLine";
    } else {
    	defaultOrCommandline_ = "default";
    }
  }

  template<typename T>
  parameter(const std::string& parName, T inValue, bool commandLine) {
  	name_ = parName;
  	value_ = std::to_string(inValue);
		if (commandLine) {
			defaultOrCommandline_ = "commandLine";
		} else {
			defaultOrCommandline_ = "default";
		}
	}
  //members
  std::string name_;
  std::string value_;
  std::string defaultOrCommandline_;

  std::string outputInfoString(bool header = false) {
  	if(header){
  		return "name\tvalue\tdefaultOrCommandLine";
  	}
    return name_ + "\t" + value_ + "\t" + defaultOrCommandline_;
  }
};

class parametersHolder {

 public:
  parametersHolder() {}
  parametersHolder(const parameter& firstParameter)
      : pars(std::vector<parameter>(1, firstParameter)) {}

  std::vector<parameter> pars;
  void addParameter(const parameter& newParameter) {
    pars.push_back(newParameter);
  }
  template <typename T>
  void addParameter(const std::string& parName,
  									const T& inValue,
                    bool commandLine) {
    pars.push_back(parameter(parName, inValue, commandLine));
  }

  void outputParsFile(std::ostream& out) {
    out << "ParameterName\tvalue\tcommandLineOrDefault" << std::endl;
    for (auto& pIter : pars) {
      out << pIter.outputInfoString() << std::endl;
    }
  }
};
}  // namespace progutil

