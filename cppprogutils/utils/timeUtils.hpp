#pragma once
/*
 * timeUtils.hpp
 *
 *  Created on: Aug 4, 2014
 *      Author: nickhathaway
 */


#include "cppprogutils/utils/utils.hpp"
#include "cppprogutils/utils/stringUtils.hpp"

namespace cppprogutils {
inline std::string getCurrentDate() {
  time_t t = time(0);  // get time now
  struct tm* now = localtime(&t);
  std::stringstream timeStream;
  timeStream << (now->tm_year + 1900) << '-' << leftPadNumStr((now->tm_mon + 1))
             << '-' << leftPadNumStr(now->tm_mday) << '_'
             << leftPadNumStr(now->tm_hour) << '.'
             << leftPadNumStr(now->tm_min);
  return timeStream.str();
}

inline std::string getTimeFormat(double timeInSecondsOriginal, bool verbose,
                                 int secondsDecimalPlaces) {
  std::stringstream duration;
  double timeInSeconds = timeInSecondsOriginal;
  if (timeInSeconds > 31536000) {
    int years = (int)timeInSeconds / 31536000;
    if (verbose) {
      duration << "yrs:" << years << ",";
    } else {
      duration << years << ":";
    }
    timeInSeconds = timeInSeconds - years * 31536000.0;
  }
  if (timeInSeconds > 86400) {
    int days = (int)timeInSeconds / 86400;
    if (verbose) {
      duration << "days:" << leftPadNumStr(days, 365) << ",";
    } else {
      duration << days << ":";
    }
    timeInSeconds = timeInSeconds - days * 86400.0;
  } else if (timeInSecondsOriginal > 86400) {
    if (verbose) {
      duration << "days:000,";
    } else {
      duration << "000:";
    }
  }
  if (timeInSeconds > 3600) {
    int hrs = (int)timeInSeconds / 3600;
    if (verbose) {
      duration << "hrs:" << leftPadNumStr(hrs, 24) << ",";
    } else {
      duration << leftPadNumStr(hrs, 24) << ":";
    }
    timeInSeconds = timeInSeconds - hrs * 3600.0;
  } else if (timeInSecondsOriginal > 3600.0) {
    if (verbose) {
      duration << "hrs:00,";
    } else {
      duration << "00:";
    }
  }
  if (timeInSeconds > 60) {
    int minutes = (int)timeInSeconds / 60;
    if (verbose) {
      duration << "mins:" << leftPadNumStr(minutes, 60) << ",";
    } else {
      duration << leftPadNumStr(minutes, 60) << ":";
    }

    timeInSeconds = timeInSeconds - minutes * 60.0;
  } else if (timeInSecondsOriginal > 60) {
    if (verbose) {
      duration << "mins:00,";
    } else {
      duration << "00:";
    }
  }
  if (timeInSeconds > 0) {
    if (timeInSecondsOriginal < 1) {
      if (verbose) {
        duration << "secs:" << roundDecPlaces(timeInSeconds,
                                              secondsDecimalPlaces);
      } else {
        duration << roundDecPlaces(timeInSeconds, secondsDecimalPlaces);
      }
    } else {
      if (verbose) {
        duration << "secs:"
                 << leftPadNumStr(
                        roundDecPlaces(timeInSeconds, secondsDecimalPlaces),
                        60.0);
      } else {
        duration << leftPadNumStr(
                        roundDecPlaces(timeInSeconds, secondsDecimalPlaces),
                        60.0);
      }
    }
  } else {
    if (verbose) {
      duration << "secs:00";
    } else {
      duration << "00";
    }
  }
  return duration.str();
}
namespace sch = std::chrono;

inline double getTimeDiff(sch::time_point<sch::high_resolution_clock> start){
	return sch::duration_cast<sch::nanoseconds>(
			sch::high_resolution_clock::now() - start).count() /
	       static_cast<double>(sch::high_resolution_clock::period::den);
}
inline double getTimeDiff(sch::time_point<sch::high_resolution_clock> start,
		sch::time_point<sch::high_resolution_clock> end){
	return sch::duration_cast<sch::nanoseconds>(end - start).count() /
	              static_cast<double>(sch::high_resolution_clock::period::den);
}


/**@brief Class for keeping time using the highest resolution clock available from std::chrono
 *
 *\todo Add more raw logging to simply get ticks or nanosecond run information
 *
 */
class stopWatch {
public:

	/**@brief Default constructor to time now
	 *
	 */
	stopWatch():start_(sch::high_resolution_clock::now()),
	currentLap_(sch::high_resolution_clock::now()){
		currentLapName_ = "1";
	}
	/**@brief Construct with the given time point
	 *
	 */
	stopWatch(sch::time_point<sch::high_resolution_clock> startingPoint)
	:start_(startingPoint),
	currentLap_(startingPoint){
		currentLapName_ = "1";
	}
	//members
	/**@brief Start time, logging with the highest resolution available
	 *
	 */
	sch::time_point<sch::high_resolution_clock> start_;
	/**@brief Current time for new lap, logging with the highest resolution available
	 *
	 */
	sch::time_point<sch::high_resolution_clock> currentLap_;
	/**@brief Name of currentLap_
	 *
	 */
	std::string currentLapName_;
	/**@brief A vector of pairs with first a string with lap name and second the lap time, not a map to preserve order
	 *
	 *
	 */
	std::vector<std::pair<std::string, double>> lapTimes_;
	//functions
	/**@brief Get time in seconds since start
	 * @return A double for time in seconds
	 */
	double totalTime(){
		return getTimeDiff(start_);
	}
	/**@brief Get formatted time since start formatted
	 *
	 * @param decPlaces How many decimal places to round to
	 * @return A string with the time formatted for hrs, min, secs etc.
	 */
	std::string totalTimeFormatted(int32_t decPlaces = 6){
		return getTimeFormat(getTimeDiff(start_),true, decPlaces);
	}
	/**@brief Get time in seconds since last point
	 * @return A double for time in seconds
	 */
	double timeLap(){
		return getTimeDiff(currentLap_);
	}
	/**@brief Get formatted time since last point formatted
	 *
	 * @param decPlaces How many decimal places to round to
	 * @return A string with the time formatted for hrs, min, secs etc.
	 */
	std::string timeLapFormatted(int32_t decPlaces = 6){
		return getTimeFormat(getTimeDiff(currentLap_),true, decPlaces);
	}
	/**@brief
	 * @param withName If a name is given, current lap time will be logged with that name
	 */

	/**@brief Start a new lap, log the time since last time point and reset the currentLap_ to now
	 *
	 * @param newLapName A name for the new lap, defaults to current lap number
	 * @param lastLapName A name for the last lap if a change is desired otherwise is logged as currentLapName_;
	 */
	void startNewLap(const std::string & newLapName = "",
			const std::string & lastLapName = ""){
		auto timeNew = sch::high_resolution_clock::now();
		if(lastLapName == ""){
			lapTimes_.emplace_back(currentLapName_, getTimeDiff(currentLap_, timeNew));
		}else{
			lapTimes_.emplace_back(lastLapName, getTimeDiff(currentLap_, timeNew));
		}

		if(newLapName == ""){
			currentLapName_ = to_string(lapTimes_.size() + 1);
		}else{
			currentLapName_ = newLapName;
		}
		currentLap_ = timeNew;
	}

	//output
	/**@brief Log the lap times to out in two columns separated by tab
	 * @param out The std::ostream object to log the times to
	 * @param formatted A bool whether the time should be
	 * @param decPlaces The number of decimal places to print if formatted
	 *
	 */
	void logLapTimes(std::ostream & out,
			bool formatted, int32_t decPlaces,
			bool endLastLap){
		if(endLastLap){
			startNewLap();
		}
		if(formatted){
			std::vector<VecStr> content;
			VecStr header {"lap", "time"};
			for(const auto & lt : lapTimes_){
				content.emplace_back(VecStr{lt.first,
					getTimeFormat(lt.second, true, decPlaces)});
			}
			printTableOrganized(content, header, out);
		}else{
			for(const auto & lt : lapTimes_){
				out << lt.first << "\t" << lt.second << "\n";
			}
		}
	}

};

} //namespace cppprogutils
