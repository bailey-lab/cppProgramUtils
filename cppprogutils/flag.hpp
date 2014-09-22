#pragma once
/*
 * flag.hpp
 *
 *  Created on: Sep 22, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/utils/stringUtils.hpp"
#include "cppprogutils/common.hpp"

namespace cppprogutils {

class flag {
public:
	template<typename T>
	flag(const T & opt,
			const std::string & flags,
			const std::string & shortDescription,
			bool required):
		flags_(tokenizeString(flags,",")),
		shortDescription_(shortDescription),
		required_(required),
		set_(false),
		setValue_(to_string(opt)),
		defaultValue_(to_string(opt)),
		type_(estd::getTypeName(opt))
		{
	}

	std::vector<std::string> flags_;
	std::string shortDescription_;
	bool required_;
	bool set_;
	std::string setValue_;
	std::string defaultValue_;
	std::string type_;

	template<typename T>
	void setValue(const T & option){
		if(estd::getTypeName(option) != type_){
			std::cerr << "Bad type set for flag " << vectorToString(flags_,",") << "\n"
					<< "is of type " << type_ << " and trying to set with type: " << estd::getTypeName(option)
			<< "\n";
			exit(1);
		} else {
			setValue_ = to_string(option);
			set_ = true;
		}
	}


	std::string fullInfo(const std::string delim = "\t", bool header = false) const {
    if (header) {
      return "ShortDescription\tvalue\tdefaultValue";
    }
    return shortDescription_
    		+ delim + setValue_
    		+ delim + defaultValue_;
	}
	std::string helpInfo()const {
		return shortDescription_ + "; default=" + defaultValue_ + "; (" + type_ + ")";
	}

};

/**A class for holding all the parameters
 *
 */
class flagHolder {

 public:
  /** Default constructor for an empty holder
   *
   */
	flagHolder() {}

  /**A vector holding all the parameters
   *
   */
  std::map<std::string, flag> flags_;
  /**Add a parameter that was already constructed
   *
   * @param newParameter A parameter object to add to the holder
   */
  void addFlag(const flag& newFlag) {
  	flags_.emplace(vectorToString(newFlag.flags_,","), newFlag);
  }

  /** Output the information for all stored parameters
   *
   * @param out An std::ostream object to print the info to
   *
   */
  void outputParsFile(std::ostream& out) {
  	if(!flags_.empty()){
      flags_.begin()->second.fullInfo("\t", true);
      for (const auto & f : flags_) {
        out << f.second.fullInfo("\t") << std::endl;
      }
  	}
  }
};


}  // namespace cppprogutils
