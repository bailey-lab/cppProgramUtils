#pragma once
//
//  programSetUp.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 9/27/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "cppprogutils/utils.hpp"
#include "cppprogutils/commandLineArguments.hpp"
#include "cppprogutils/parameter.hpp"
#include "cppprogutils/runLog.hpp"

namespace cppprogutils {

class programSetUp {

 public:
  // constructors
  programSetUp(int argc, char *argv[]):
  	start_(std::chrono::high_resolution_clock::now()) {
    commands_ = commandLineArguments(argc, argv);
    commands_.arguments_["-program"] = argv[0];
    if (commands_.arguments_.find("-program") == commands_.arguments_.end()) {
      programName_ = "program";
    } else {
      programName_ = commands_["-program"];
    }
  }
  programSetUp(const commandLineArguments &inputCommands):
  	start_(std::chrono::high_resolution_clock::now()) {
    commands_ = inputCommands;
    if (commands_.arguments_.find("-program") == commands_.arguments_.end()) {
      programName_ = "program";
    } else {
      programName_ = commands_["-program"];
    }
  }

  programSetUp(const MapStrStr &inputCommands):
  	start_(std::chrono::high_resolution_clock::now()) {
    commands_ = commandLineArguments(inputCommands);
    if (commands_.arguments_.find("-program") == commands_.arguments_.end()) {
      programName_ = "program";
    } else {
      programName_ = commands_["-program"];
    }
  }

  virtual ~programSetUp() {}
  // members
  // map to hold the commands
  commandLineArguments commands_;

  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
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
  runLog rLog_;

  void startARunLog(const std::string & dirName){
  	rLog_.setFilenameAndOpen(dirName + "runLog_"+ replaceString(programName_, "./", "") + ".txt", start_);
  	rLog_.startRunLog(commands_.arguments_);
  }

  void writeParametersFile(const std::string &fileName, bool overWrite,
                           bool failOnWriteFailure) {
    std::ofstream parameterFile;
    openTextFile(parameterFile, fileName, ".txt", overWrite,
                 failOnWriteFailure);
    parameterFile << commands_.commandLine_ << std::endl;
    pars_.outputParsFile(parameterFile);
  }
  // check for valid commands
  void lookForInvalidOptions() {
    validOptions_.push_back("-program");
    if (commands_.arguments_.find("-program") != commands_.arguments_.end()) {
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
  void printWarnings(std::ostream &out) {
    for (const auto &warn : warnings_) {
      out << warn << std::endl;
    }
  }
  void printFlags(std::ostream &out) {
    std::sort(validOptions_.begin(), validOptions_.end());
    for (const auto &flag : validOptions_) {
      out << flag << std::endl;
    }
  }
  void finishSetUp(std::ostream &out) {
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

  // option setters, case insensitive for flags
  bool setBoolOptionFalse(bool &option, std::string flag,
                          const std::string &parName, bool required = false) {
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
  template <typename T>
  bool setOption(T &option, std::string flag, const std::string &parName,
                 bool required = false) {
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
  // flag processing
  VecStr processFlag(std::string &flag) { return tokenizeString(flag, ","); }
  double getRunTimeInSecs() {
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
  bool needsHelp(uint32_t minAmountOfArgs = 1) {
    return (commands_.containsFlagCaseInsensitive("-help") ||
            commands_.containsFlagCaseInsensitive("-h") ||
            commands_.numberOfCommands() <= minAmountOfArgs);
  }
  void writeOutCommandLineArguments(const MapStrStr &commandLineArgumentsMap,
                                    std::ostream &out) {
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
};
}  // namespace cppprogutils
