#pragma once
/*
 * runLog.hpp
 *
 *  Created on: Aug 2, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/common.hpp"
#include "cppprogutils/utils.hpp"
namespace cppprogutils {
class runLog {
 public:
  /**Constructor with the runLog filename and start time as now
   *
   * @param fileName The name of the runLog file to log the information to
   *
   */
  runLog(const std::string& fileName)
      : fileName_(fileName), start_(std::chrono::high_resolution_clock::now()) {
    openTextFile(runLogFile_, fileName_, ".txt", true, false);
  }
  /**Constructor with the runLog filename and the given start
   *
   * @param fileName The name of the runLog file to log the information to
   * @param start Use the given start time for time start
   *
   */
  runLog(const std::string& fileName,
         std::chrono::time_point<std::chrono::high_resolution_clock> start)
      : fileName_(fileName), start_(start) {
    openTextFile(runLogFile_, fileName_, ".txt", true, false);
  }
  /** Empty constructor, file won't be opened until setFilenameAndOpen() is
   *called
   *
   */
  runLog() {}

 private:
  // Members
  /**Name of the runLog file
   *
   */
  std::string fileName_;
  /**The start time stored as a std::chrono::time_point
   *
   */
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

 public:
  /**The std::ofstream object to log the info to
   *
   */
  std::ofstream runLogFile_;
  /**Open the runLog file with this name, and the start time will be now, will
   *fail if runLog already opened
   *@param fileName The filename for the runLog
   */
  void setFilenameAndOpen(const std::string& fileName) {
    if (runLogFile_) {
      std::cerr << "\033[1;31m";
      std::cerr << "in runLog.setFilenameAndOpen()\n";
      std::cerr << "Run log already started\n";
      std::cerr << "\033[0m";
      exit(1);
    } else {
      fileName_ = fileName;
      openTextFile(runLogFile_, fileName_, ".txt", true, false);
      start_ = std::chrono::high_resolution_clock::now();
    }
  }
  /**Open the runLog file with this name and given start time, will fail if
   * runLog already opened
   * @param fileName The filename for the runLog
   * @param start Use given start time for logging time
   */
  void setFilenameAndOpen(
      const std::string& fileName,
      std::chrono::time_point<std::chrono::high_resolution_clock> start) {
    if (runLogFile_) {
      std::cerr << "\033[1;31m";
      std::cerr << "in runLog.setFilenameAndOpen()\n";
      std::cerr << "Run log already started\n";
      std::cerr << "\033[0m";
      exit(1);
    } else {
      fileName_ = fileName;
      openTextFile(runLogFile_, fileName_, ".txt", true, false);
      start_ = start;
    }
  }
  /** Put starting stamp on runLog by putting date and the command given for the
   *program, will fail if runLog not started
   *
   * @param inputCommands The commands for the current program that contains the
   *commandline for logging
   *
   */
  void startRunLog(const MapStrStr& inputCommands) {
    if (runLogFile_) {
      runLogFile_ << "Ran on " << getCurrentDate() << std::endl;
      auto commandlineArgs = inputCommands.find("-commandline");
      if (commandlineArgs != inputCommands.end()) {
        runLogFile_ << commandlineArgs->second << std::endl;
      }
    } else {
      std::cerr << "\033[1;31m";
      std::cerr << "in runLog.startRunLog()\n";
      std::cerr << "RunLog object's runLogfile not open yet\n";
      std::cerr << "while attempting start runLog\033[0m\n";
      std::cerr << "\033[0m";
    }
  }
  /** Log the current time difference from when the time point start_, will fail
   *if runLog not started
   *
   */
  void logTime(uint32_t decPlaces = 6) {
    if (runLogFile_) {
      runLogFile_
          << getTimeFormat(
                 std::chrono::duration_cast<std::chrono::nanoseconds>(
                     std::chrono::high_resolution_clock::now() - start_)
                         .count() /
                     static_cast<double>(
                         std::chrono::high_resolution_clock::period::den),
                 true, decPlaces) << "\n";
    } else {
      std::cerr << "\033[1;31m";
      std::cerr << "in runLog.logTime()\n";
      std::cerr << "RunLog object's runLogfile not open yet\n";
      std::cerr << "while attempting to log time\n";
      std::cerr << "\033[0m";
    }
  }
  /**Destructor, log the end time
   *
   */
  ~runLog() { logTime(6); }
};
/**Input operator to log information to the runLogFile_ for the given runLog,
 *will fail if runLog not started
 *
 */
template <typename T>
runLog& operator<<(runLog& rLog, const T& obj) {
  if (rLog.runLogFile_) {
    rLog.runLogFile_ << obj;
  } else {
    std::cerr << "\033[1;31m";
    std::cerr << "RunLog object's runLogfile not open yet\n";
    std::cerr << "while attempting to use << operator on runLog\n";
    std::cerr << "\033[0m";
  }
  return rLog;
}
}
