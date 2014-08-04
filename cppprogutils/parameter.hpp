#pragma once
//
//  parameter.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 9/29/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "cppprogutils/utils.h"
namespace cppprogutils {

class parameter {
 public:
  /** Constructor for bool parameter
   *
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
  /** Constructor for string parameter
   *
   */
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
  /** Templated constructor for numeric types or anything that can be converted
   *by to_string
   *
   */
  template <typename T>
  parameter(const std::string& parName, T inValue, bool commandLine) {
    name_ = parName;
    value_ = to_string(inValue);
    if (commandLine) {
      defaultOrCommandline_ = "commandLine";
    } else {
      defaultOrCommandline_ = "default";
    }
  }
  // members
  /**Name of paramter
   *
   */
  std::string name_;
  /**Stored value
   *
   */
  std::string value_;
  /**Whether it was set by command line or was default
   *
   */
  std::string defaultOrCommandline_;
  /**Output members in a delimited string
   * @param header Whether to get the header for the output information, default
   *false
   *
   * @return A tab delimited string of the information stored or the header
   */
  std::string outputInfoString(bool header = false) {
    if (header) {
      return "name\tvalue\tdefaultOrCommandLine";
    }
    return name_ + "\t" + value_ + "\t" + defaultOrCommandline_;
  }
};

/**A class for holding all the parameters
 *
 */
class parametersHolder {

 public:
  /** Default constructor for an empty holder
   *
   */
  parametersHolder() {}
  /**Constructor by giving the first parameter
   *
   * @param firstParameter The first parameter to be stored in the holder
   *
   */
  parametersHolder(const parameter& firstParameter)
      : pars(std::vector<parameter>(1, firstParameter)) {}

  /**A vector holding all the parameters
   *
   */
  std::vector<parameter> pars;
  /**Add a parameter that was already constructed
   *
   * @param newParameter A parameter object to add to the holder
   */
  void addParameter(const parameter& newParameter) {
    pars.emplace_back(newParameter);
  }
  /**Add a parameter by giving the information needed to constructor a parameter
   *
   * @param parName Name of parameter
   * @param inValue The parameter value
   * @param commandLine Bool, true if set at the command line, false if it is
   *the default value
   */
  template <typename T>
  void addParameter(const std::string& parName, const T& inValue,
                    bool commandLine) {
    pars.emplace_back(parameter(parName, inValue, commandLine));
  }
  /** Output the information for all stored parameters
   *
   * @param out An std::ostream object to print the info to
   *
   */
  void outputParsFile(std::ostream& out) {
    out << "ParameterName\tvalue\tcommandLineOrDefault" << std::endl;
    for (auto& pIter : pars) {
      out << pIter.outputInfoString() << std::endl;
    }
  }
};
}  // namespace cppprogutils
