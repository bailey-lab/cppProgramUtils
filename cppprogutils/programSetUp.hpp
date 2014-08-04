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

/**Class for helping set up current program with flag parsing, logging run
 *times, getting help messages
 *
 */
class programSetUp {

 public:
  // constructors
  /**@brief Construct the setUp with the generic argc and argv
   *
* @param argc The number of arguments
* @param argv The array of char pointers of the arguments
*
   */
  programSetUp(int argc, char *argv[])
      : commands_(commandLineArguments(argc, argv)),
        start_(std::chrono::high_resolution_clock::now()) {
    commands_.arguments_["-program"] = argv[0];
    // get rid of the ./ if program is being called from current dir, it can
    // mess things up latter
    programName_ = replaceString(argv[0], "./", "");
  }

  /**@brief Construct the setUp with the commandLineArguments class converted
*from argc and argv
   *
* @param inputCommands The commandLineArguments object likely converted from
*argc and argv
*
   */
  programSetUp(const commandLineArguments &inputCommands)
      : commands_(inputCommands),
        start_(std::chrono::high_resolution_clock::now()) {
    auto progSearch = commands_.arguments_.find("-program");
    // if no program name (though there really should be) just call it program
    if (commands_.arguments_.end() == progSearch) {
      programName_ = "program";
    } else {
      programName_ = progSearch->second;
    }
  }

  /**@brief Construct the setUp with a map of string pairs converted from argc
*and argv
   *
* @param inputCommands A map of string pairs object likely converted from argc
*and argv
*
   */
  programSetUp(const MapStrStr &inputCommands)
      : commands_(commandLineArguments(inputCommands)),
        start_(std::chrono::high_resolution_clock::now()) {
    auto progSearch = commands_.arguments_.find("-program");
    // if no program name (though there really should be) just call it program
    if (commands_.arguments_.end() == progSearch) {
      programName_ = "program";
    } else {
      programName_ = progSearch->second;
    }
  }

  virtual ~programSetUp() {}

  // members
  // map to hold the commands
  /**@brief A commandLineArguments class to store the objects and help with flag
   *parsing, see commandLineArguments for more details
   *
   */
  commandLineArguments commands_;

  /**@brief A std::chrono::time_point object to store the start of the program
   *so run time can be logged
   *
   */
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

 protected:
  // valid options
  /**@brief A vector of strings of valid options to keep track of what is
   *allowed for current setUp
   *
   */
  VecStr validOptions_;

  /**@brief A vector of strings of warnings to be printed at the end of setUp
   *
   */
  VecStr warnings_;

 public:
  /**@brief A bool to indicated whether the set up has failed somewhere
   *
   */
  bool failed_ = false;

  // for outputting
  /**@brief The maximum width to allow for messages
   *
   */
  uint32_t width_ = 80;

  /**@brief The indent amount for wrapping messages
   *
   */
  uint32_t indent_ = 4;

  // parameters for logging
  /**@brief A container for parameters set during set up, see parametersHolder
   *for more details
   *
   */
  parametersHolder pars_;

  /**@brief The name of the program associated with the current setUp object
   *
   */
  std::string programName_;

  /**@brief A runLog object for optional logging of run time info, won't be
   *started unless programSetUp::startARunLog is called
   *
   */
  runLog rLog_;

  /**@brief A function to start a runLog in the named directory
   * @param dirName The name of the directory to start the runLog, runLog name
   *will be runLog_[NAME_OF_PROGRAM]
   *
   */
  void startARunLog(const std::string &dirName) {
    rLog_.setFilenameAndOpen(
        dirName + "runLog_" + replaceString(programName_, "./", "") + ".txt",
        start_);
    rLog_.startRunLog(commands_.arguments_);
  }

  /**@brief A function to write info on the parameters set during the set up
   * process
   * @param fileName The name of the parameter
   * @param overWrite A bool indicating whether to overwrite any file with that
   * name
   * @param failOnWriteFailure A bool whether to exit if the parameters file
   * couldn't be written
   */
  void writeParametersFile(const std::string &fileName, bool overWrite,
                           bool failOnWriteFailure) {
    std::ofstream parameterFile;
    openTextFile(parameterFile, fileName, ".txt", overWrite,
                 failOnWriteFailure);
    parameterFile << commands_.commandLine_ << std::endl;
    pars_.outputParsFile(parameterFile);
  }

  // check for valid commands
  /**@brief Called at the end of set up to look for any invalid options but
   *comparing current arguments to validOptions_
   *
   */
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

  /**@brief Print any warnings incurred during set up to out
   * @param out The std::ostream out object to print to
   *
   */
  void printWarnings(std::ostream &out) {
    for (const auto &warn : warnings_) {
      out << warn << std::endl;
    }
  }

  /**@brief Print flags but looking at validOptions_ so needs to be called at
   * the end of set up so that options can be placed into validOptions_
   * @param out The std::ostream out object to print to
   */
  void printFlags(std::ostream &out) {
    std::sort(validOptions_.begin(), validOptions_.end());
    for (const auto &flag : validOptions_) {
      out << flag << std::endl;
    }
  }

  /**@brief Once all options have been looked for call finishSetUp() so that any
   *warnings or help messages can be print and to find out if set up was
   *successful
   * @param out The std::ostream out object to print to
   *
   */
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
  /**@brief Look for option and set the bool option to false if flag is found
   * @param option The bool option to set
   * @param flag The flag to be searched for
   * @param parName The name under which to store the option being searched for
   * @param required A bool indicating if the option is required and set up
   *should be stopped if not found, default is false
   *
   * @return Returns true if option is found or false if option is not found
   *
   */
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

  /**@brief A templated function to look for options and implementation for
   *setting the option is handled by commandLineArguments so look there for more
   *details
   * @param option The option to set
   * @param flag The flag to be searched for
   * @param parName The name under which to store the option being searched for
   * @param required A bool indicating if the option is required and set up
   *should be stopped if not found, default is false
   *
   * @return Returns true if option is found or false if option is not found
   *
   */
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
 protected:
  /**@brief Function to process current flag to handle multiple flags being
   * given
   * @param flag The flag for current option, will be split on commas (,) so
   * that multiple flags can be given for one option
   * @return A string vector with the flags split on (,), if only one flag will
   * be a vector of size of one
   */
  VecStr processFlag(std::string &flag) { return tokenizeString(flag, ","); }

 public:
  /**@brief Get the current run time since time point start_ in seconds
   * @return A double with the run time converted into seconds
   *
   */
  double getRunTimeInSecs() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::high_resolution_clock::now() - start_).count() /
           static_cast<double>(std::chrono::high_resolution_clock::period::den);
  }

  /**@brief Get a string formated run time in seconds rounded to the nearest
   * hundreth
   * @return A string wiht run time formated to tell time for hrs, minutes,
   * seconds etc.
   */
  std::string getRunTime() {
    return "(" + getTimeFormat(getRunTimeInSecs(), true, 2) + ")";
  }

  /**@brief Function to log the run time to a std::ostream
   * @param out The std::ostream to log the info to
   */
  void logRunTime(std::ostream &out) {
    out << getRunTime() << std::endl;
    return;
  }

  /**@brief A function to find out if help is need or required, called with a
   * minimum arguments parameter to indicated the num of args to print out if no
   * help flag is indicated
   * @param minAmountOfArgs Number of arguments to print help, defualts to one
   * so if only the program name is given it indicates help is needed
   * @return return true if a help message should be printed or false if no help
   * is needed
   */
  bool needsHelp(uint32_t minAmountOfArgs = 1) {
    return (commands_.containsFlagCaseInsensitive("-help") ||
            commands_.containsFlagCaseInsensitive("-h") ||
            commands_.numberOfCommands() <= minAmountOfArgs);
  }

  /**@brief A Function to print out all the current arguments in an organized
   *table
   * @param commandLineArgumentsMap A map of strings pairs of the current
   *arguments
   * @param out The std::ostream object to print the info to
   *
   */
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
