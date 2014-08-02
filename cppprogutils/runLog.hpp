#pragma once
/*
 * runLog.hpp
 *
 *  Created on: Aug 2, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/common.hpp"
#include "cppprogutils/utils.hpp"
namespace cppprogutils{
class runLog {
 public:
  // constructors
  runLog(const std::string &fileName)
      : fileName_(fileName),
        start_(std::chrono::high_resolution_clock::now()) {
    openTextFile(runLogFile_, fileName_, ".txt", true, false);
  }
  runLog(const std::string & fileName,
  		std::chrono::time_point<std::chrono::high_resolution_clock> start):
  			fileName_(fileName), start_(start){
  	openTextFile(runLogFile_, fileName_, ".txt", true, false);
  }
  runLog(){

  }
private:
  // Members
  std::string fileName_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
public:
  std::ofstream runLogFile_;

  void setFilenameAndOpen(const std::string & fileName){
  	fileName_ = fileName;
  	openTextFile(runLogFile_, fileName_, ".txt", true, false);
  	start_ = std::chrono::high_resolution_clock::now();
  }
  void setFilenameAndOpen(const std::string & fileName,
  		std::chrono::time_point<std::chrono::high_resolution_clock> start){
  	fileName_ = fileName;
  	openTextFile(runLogFile_, fileName_, ".txt", true, false);
  	start_ = start;
  }

  void startRunLog(const MapStrStr &inputCommands) {
  	if(runLogFile_){
      runLogFile_ << "Ran on " << getCurrentDate() << std::endl;
      auto commandlineArgs = inputCommands.find("-commandline");
      if (commandlineArgs != inputCommands.end()){
      	runLogFile_ <<  commandlineArgs->second << std::endl;
      }
  	}else{
  		std::cerr << "\033[1;31m";
  		std::cerr << "\033[1;196min runLog.startRunLog()\n";
  		std::cerr << "\033[1;196mRunLog object's runLogfile not open yet\n";
  		std::cerr << "\033[1;196mwhile attempting start runLog\033[0m\n";
  		std::cerr << "\033[0m";
  	}

  }
  void logTime(uint32_t decPlaces = 6) {
  	if(runLogFile_){
    	runLogFile_ << getTimeFormat(std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now() - start_).count() /
      static_cast<double>(std::chrono::high_resolution_clock::period::den),true, decPlaces)
      		<< "\n";
  	}else{
  		std::cerr << "\033[1;31m";
  		std::cerr << "in runLog.logTime()\n";
  		std::cerr << "RunLog object's runLogfile not open yet\n";
  		std::cerr << "while attempting to log time\n";
  		std::cerr << "\033[0m";
  	}

  }
  /*
   * Destructor, log the end time
   */
  ~ runLog(){
  	logTime(6);
  }
};
template<typename T>
runLog& operator<< (runLog& rLog, const T& obj) {
	if(rLog.runLogFile_){
		rLog.runLogFile_ << obj;
	}else{
		std::cerr << "\033[1;31m";
		std::cerr << "RunLog object's runLogfile not open yet\n";
		std::cerr << "while attempting to use << operator on runLog\n";
		std::cerr << "\033[0m";
	}
	return rLog;
}


}
