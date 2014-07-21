#pragma once
//
//  programSetUp.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 9/27/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "progutil/utils.hpp"
#include "progutil/commandLineArguments.hpp"
#include "progutil/parameter.hpp"

namespace progutil {

class programSetUp {

 public:
  // constructors
  programSetUp(int argc, char *argv[]);
  programSetUp(const commandLineArguments &inputCommands);
  programSetUp(const MapStrStr &inputCommands);
  //members
  // map to hold the commands
  commandLineArguments commands_;

  std::chrono::time_point<std::chrono::high_resolution_clock> start_;;
  // valid options
  VecStr validOptions_;
  VecStr warnings_;
  bool failed_ = false;
  // for outputting
  uint32_t width_ = 80;
  uint32_t indent_ = 4;
  // parameters for logging
  parametersHolder pars_;
  std::string programName_;

  void writeParametersFile(const std::string &fileName, bool overWrite,
                           bool failOnWriteFailure);
  // check for valid commands
  void lookForInvalidOptions();
  void printWarnings(std::ostream &out);
  void printFlags(std::ostream &out);
  void finishSetUp(std::ostream &out);

  // option setters, case insensitive for flags
  bool setBoolOptionFalse(bool &option, std::string flag,
                          const std::string &parName, bool required = false);
  template <typename T>
  bool setOption(T &option, std::string flag, const std::string &parName,
                 bool required = false);
  template <typename T>
  bool setOption(T &option, std::string flag, const std::string &parName, std::function<T(std::string)> fun,
                 bool required = false){
    VecStr flagToks = processFlag(flag);
    validOptions_.emplace_back(flag);
    bool found = false;
    for (const auto &fTok : flagToks) {
      if (commands_.lookForOption(option, fTok, fun)) {
        // validOptions_.push_back(fTok);
        pars_.addParameter(parName, option, true);
        found = true;
        break;
      } else {
        found = false;
      }
    }

    if (required && !found) {
      std::stringstream tempStream;
      tempStream << boldBlackText("Need to have ")
                 << boldText(vectorToString(tokenizeString(flag, ","), " or "),
                             "31")
                 << boldBlackText(" see ") +
                        boldText(commands_["-program"] + " -help ", "31") +
                        boldBlackText("for more details");
      warnings_.emplace_back(tempStream.str());
      failed_ = true;
    }
    if (!found) {
      pars_.addParameter(parName, option, false);
    }
    return found;
  }
  // flag processing
  VecStr processFlag(std::string &flag);
  double getRunTimeInSecs(){
  	return std::chrono::duration_cast<std::chrono::nanoseconds>(
  	             std::chrono::high_resolution_clock::now() - start_).count() /
  	         static_cast<double>(std::chrono::high_resolution_clock::period::den);
  }
  std::string getRunTime() {
  	return "(" + getTimeFormat(getRunTimeInSecs(), true, 2) + ")";
  }
  void logRunTime(std::ostream &out) {
  	out << getRunTime() << std::endl;
    return;
  }
  bool needsHelp(uint32_t minAmountOfArgs = 1);
  void writeOutCommandLineArguments(const MapStrStr &commandLineArgumentsMap,
                                    std::ostream &out);
};

template <typename T>
bool programSetUp::setOption(T &option, std::string flag,
                             const std::string &parName, bool required) {
  VecStr flagToks = processFlag(flag);
  validOptions_.emplace_back(flag);
  bool found = false;
  for (const auto &fTok : flagToks) {
    if (commands_.lookForOption(option, fTok)) {
      // validOptions_.push_back(fTok);
      pars_.addParameter(parName, option, true);
      found = true;
      break;
    } else {
      found = false;
    }
  }

  if (required && !found) {
    std::stringstream tempStream;
    tempStream << boldBlackText("Need to have ")
               << boldText(vectorToString(tokenizeString(flag, ","), " or "),
                           "31")
               << boldBlackText(" see ") +
                      boldText(commands_["-program"] + " -help ", "31") +
                      boldBlackText("for more details");
    warnings_.emplace_back(tempStream.str());
    failed_ = true;
  }
  if (!found) {
    pars_.addParameter(parName, option, false);
  }
  return found;
}
// constructor
programSetUp::programSetUp(int argc, char *argv[]) {
  commands_ = commandLineArguments(argc, argv);
  commands_.arguments_["-program"] = argv[0];
  if(commands_.arguments_.find("-program") == commands_.arguments_.end()){
  	programName_ = "program";
  }else{
  	programName_ = commands_["-program"];
  }
  start_ = std::chrono::high_resolution_clock::now();
}
programSetUp::programSetUp(const commandLineArguments &inputCommands) {
  commands_ = inputCommands;
  if(commands_.arguments_.find("-program") == commands_.arguments_.end()){
  	programName_ = "program";
  }else{
  	programName_ = commands_["-program"];
  }
  start_ = std::chrono::high_resolution_clock::now();
}
programSetUp::programSetUp(const MapStrStr &inputCommands) {
  commands_ = commandLineArguments(inputCommands);
  if(commands_.arguments_.find("-program") == commands_.arguments_.end()){
  	programName_ = "program";
  }else{
  	programName_ = commands_["-program"];
  }
  start_ = std::chrono::high_resolution_clock::now();
}

void programSetUp::lookForInvalidOptions() {
  validOptions_.push_back("-program");
  if(commands_.arguments_.find("-program") != commands_.arguments_.end()){
  	validOptions_.push_back(commands_["-program"]);
  }
  validOptions_.push_back("-commandline");
  changeStringVectorToLowerCase(validOptions_);
  VecStr expandedOptions;
  for (const auto &op : validOptions_) {
    addOtherVec(expandedOptions, tokenizeString(op, ","));
  }

  for (const auto &com : commands_.arguments_) {
    if (!vectorContains(expandedOptions, com.first)) {
      warnings_.emplace_back(
          boldText("Unrecognized option, " + com.first + " not using", "31"));
    }
  }
}
void programSetUp::printWarnings(std::ostream &out) {
  for (const auto &warn : warnings_) {
    out << warn << std::endl;
  }
}
void programSetUp::printFlags(std::ostream &out) {
  std::sort(validOptions_.begin(), validOptions_.end());
  for (const auto &flag : validOptions_) {
    out << flag << std::endl;
  }
}
void programSetUp::finishSetUp(std::ostream &out) {
  if (commands_.containsFlagCaseInsensitive("-getFlags") ||
      commands_.containsFlagCaseInsensitive("-flags")) {
    printFlags(out);
    exit(1);
  }
  lookForInvalidOptions();
  printWarnings(out);
  if (failed_) {
    exit(1);
  }
}
bool programSetUp::setBoolOptionFalse(bool &option, std::string flag,
                                      const std::string &parName,
                                      bool required) {
  VecStr flagToks = processFlag(flag);
  bool found = false;
  validOptions_.push_back(flag);
  for (const auto &t : flagToks) {
    if (commands_.lookForOptionFalse(option, t)) {
      // option=true;
      // validOptions_.push_back(t);
      pars_.addParameter(parName, option, true);
      found = true;
      break;

    } else {
      found = false;
    }
  }

  if (required && !found) {
    std::stringstream tempStream;
    tempStream << boldBlackText("Need to have ")
               << boldText(vectorToString(tokenizeString(flag, ","), " or "),
                           "31")
               << boldBlackText(" see ") +
                      boldText(commands_["-program"] + " -help ", "31") +
                      boldBlackText("for more details");
    warnings_.emplace_back(tempStream.str());
    failed_ = true;
  }
  if (!found) {
    pars_.addParameter(parName, option, false);
  }
  return found;
}

VecStr programSetUp::processFlag(std::string &flag) {
  return tokenizeString(flag, ",");
}

void programSetUp::writeParametersFile(const std::string &fileName,
                                       bool overWrite,
                                       bool failOnWriteFailure) {
  std::ofstream parameterFile;
  openTextFile(parameterFile, fileName, ".txt", overWrite, failOnWriteFailure);
  parameterFile << commands_.commandLine_ << std::endl;
  pars_.outputParsFile(parameterFile);
}

bool programSetUp::needsHelp(uint32_t minAmountOfArgs) {
  return (commands_.containsFlagCaseInsensitive("-help") ||
          commands_.containsFlagCaseInsensitive("-h") ||
          commands_.numberOfCommands() <= minAmountOfArgs);
}

void programSetUp::writeOutCommandLineArguments(
    const MapStrStr &commandLineArgumentsMap, std::ostream &out) {
  int optionCount = 0;

  VecStr columnNames = {"OptionNum", "Flag", "Option"};
  std::vector<VecStr> content;
  for (const auto &mContent : commandLineArgumentsMap) {
    if (mContent.first != "-commandline" && mContent.first != "-program") {
      content.emplace_back(VecStr{to_string(optionCount) + ")",
                                          mContent.first, mContent.second});
      ++optionCount;
    }
  }
  printTableOrganized(content, columnNames, out);
}

}  // namespace progutil
