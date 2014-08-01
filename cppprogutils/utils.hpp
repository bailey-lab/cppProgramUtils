#pragma once
/*
 * utils.hpp
 *
 *  Created on: Jul 19, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/common.hpp"
namespace cppprogutils {
template <typename NUM>
std::string to_string(NUM number) {
  std::stringstream tempStream;
  tempStream << number;
  return tempStream.str();
}

template <typename T>
std::string leftPadNumStr(T num, T highestNumber = 10) {
  std::stringstream ans;
  if (num == 0) {
    ans << std::string(log10(highestNumber), '0');
  } else {
    ans << std::string(((int)log10(highestNumber) - (int)log10(num)), '0');
  }
  ans << num;
  return ans.str();
}
inline std::string convertBoolToString(bool convert) {
  if (convert) {
    return "true";
  } else {
    return "false";
  }
}
inline double roundDecPlaces(double num, int decPlaces) {
  double rounder = pow(10, decPlaces);
  return (floor(num * rounder + 0.5) / rounder);
}

inline void stringToUpper(std::string& str) {
  for (auto& c : str) {
    c = toupper(c);
  }
}
inline void stringToLower(std::string& str) {
  for (auto& c : str) {
    c = tolower(c);
  }
}
inline std::string stringToUpperReturn(std::string str) {
  stringToUpper(str);
  return str;
}
inline std::string stringToLowerReturn(std::string str) {
  stringToLower(str);
  return str;
}

inline void changeStringVectorToLowerCase(VecStr& vec) {
  for (auto& v : vec) {
    stringToLower(v);
  }
}

inline std::string get_cwd() {
  // from http://stackoverflow.com/a/2869667
  const size_t chunkSize = 255;
  const int maxChunks =
      10240;  // 2550 KiBs of current path are more than enough

  char stackBuffer[chunkSize];  // Stack buffer for the "normal" case
  if (getcwd(stackBuffer, sizeof(stackBuffer)) != NULL) return stackBuffer;
  if (errno != ERANGE) {
    // It's not ERANGE, so we don't know how to handle it
    throw std::runtime_error("Cannot determine the current path.");
    // Of course you may choose a different error reporting method
  }
  // Ok, the stack buffer isn't long enough; fallback to heap allocation
  for (int chunks = 2; chunks < maxChunks; chunks++) {
    // With boost use scoped_ptr; in C++0x, use unique_ptr
    // If you want to be less C++ but more efficient you may want to use realloc
    std::unique_ptr<char> cwd(new char[chunkSize * chunks]);
    if (getcwd(cwd.get(), chunkSize * chunks) != NULL) return cwd.get();
    if (errno != ERANGE) {
      // It's not ERANGE, so we don't know how to handle it
      throw std::runtime_error("Cannot determine the current path.");
      // Of course you may choose a different error reporting method
    }
  }
  throw std::runtime_error(
      "Cannot determine the current path; the path is apparently unreasonably "
      "long");
  // return "";
}

template <typename T>
std::string vectorToString(const std::vector<T>& vectorToConvert,
                           const std::string& delim = " ") {
  if (vectorToConvert.empty()) {
    return "";
  }
  std::stringstream tempStringStream;
  copy(vectorToConvert.begin(), vectorToConvert.end(),
       std::ostream_iterator<T>(tempStringStream, delim.c_str()));
  std::string returnString = tempStringStream.str().c_str();
  returnString.erase(returnString.size() - (int)delim.size());
  return returnString;
}

inline std::vector<std::string> tokenizeString(const std::string& str,
                                               const std::string& delim,
                                               bool addEmptyToEnd = false) {
  std::vector<std::string> output;
  if ("whitespace" == delim) {
    std::stringstream tempStream(str);
    while (!tempStream.eof()) {
      std::string tempName;
      tempStream >> tempName;
      output.emplace_back(tempName);
    }
  } else {
    if (str.find(delim.c_str()) == std::string::npos) {
      output.push_back(str);
    } else {
      std::size_t pos = str.find(delim, 0);
      std::size_t oldPos = -delim.size();
      while (pos != std::string::npos) {
        output.push_back(
            str.substr(oldPos + delim.size(), pos - oldPos - delim.size()));
        oldPos = pos;
        pos = str.find(delim, pos + 1);
      }
      if (oldPos + delim.size() == str.size()) {
        if (addEmptyToEnd) {
          output.push_back("");
        }
      } else {
        output.push_back(str.substr(oldPos + delim.size(), str.size() - 1));
      }
    }
  }
  return output;
}

inline bool containsSubString(const std::string& str,
                              const std::string& subString) {
  return (str.find(subString) != std::string::npos);
}

inline bool stringContainsAllDigits(const std::string& str) {
  for (const auto& c : str) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}
inline std::string getTimeFormat(double timeInSecondsOriginal, bool wordy,
                                 int secondsDecimalPlaces) {
  std::stringstream duration;
  double timeInSeconds = timeInSecondsOriginal;
  if (timeInSeconds > 31536000) {
    int years = (int)timeInSeconds / 31536000;
    if (wordy) {
      duration << "yrs:" << years << ",";
    } else {
      duration << years << ":";
    }
    timeInSeconds = timeInSeconds - years * 31536000.0;
  }
  if (timeInSeconds > 86400) {
    int days = (int)timeInSeconds / 86400;
    if (wordy) {
      duration << "days:" << leftPadNumStr(days, 365) << ",";
    } else {
      duration << days << ":";
    }
    timeInSeconds = timeInSeconds - days * 86400.0;
  } else if (timeInSecondsOriginal > 86400) {
    if (wordy) {
      duration << "days:000,";
    } else {
      duration << "000:";
    }
  }
  if (timeInSeconds > 3600) {
    int hrs = (int)timeInSeconds / 3600;
    if (wordy) {
      duration << "hrs:" << leftPadNumStr(hrs, 24) << ",";
    } else {
      duration << leftPadNumStr(hrs, 24) << ":";
    }
    timeInSeconds = timeInSeconds - hrs * 3600.0;
  } else if (timeInSecondsOriginal > 3600.0) {
    if (wordy) {
      duration << "hrs:00,";
    } else {
      duration << "00:";
    }
  }
  if (timeInSeconds > 60) {
    int minutes = (int)timeInSeconds / 60;
    if (wordy) {
      duration << "mins:" << leftPadNumStr(minutes, 60) << ",";
    } else {
      duration << leftPadNumStr(minutes, 60) << ":";
    }

    timeInSeconds = timeInSeconds - minutes * 60.0;
  } else if (timeInSecondsOriginal > 60) {
    if (wordy) {
      duration << "mins:00,";
    } else {
      duration << "00:";
    }
  }
  if (timeInSeconds > 0) {
    if (timeInSecondsOriginal < 1) {
      if (wordy) {
        duration << "secs:" << roundDecPlaces(timeInSeconds,
                                              secondsDecimalPlaces);
      } else {
        duration << roundDecPlaces(timeInSeconds, secondsDecimalPlaces);
      }
    } else {
      if (wordy) {
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
    if (wordy) {
      duration << "secs:00";
    } else {
      duration << "00";
    }
  }
  return duration.str();
}
inline std::string centerText(const std::string& text, uint32_t maxWidth) {
  uint32_t halfWay = std::round(maxWidth / 2.0);
  uint32_t halfText = std::round(text.size() / 2.0);
  return std::string(halfWay - halfText, ' ') + text;
}

inline std::string boldText(const std::string& title,
                            const std::string& colorCode) {
  return "\033[1;" + colorCode + "m" + title + "\033[0m";
}
inline std::string boldBlackText(const std::string& title) {
  return boldText(title, "30");
}
inline std::string centerTitle(const std::string& text, uint32_t maxWidth) {
  return centerText(boldBlackText(text), maxWidth);
}
inline std::string addFlashingColor(const std::string& colorCode) {
  return "\033[38;5;" + colorCode + ";147;5" + "m";
}
inline std::string addFlashingColor(uint32_t colorCode) {
  return addFlashingColor(to_string(colorCode));
}
inline std::string addAnsiAtr(const std::string& atr) {
  return "\033[" + atr + "m";
}
inline std::string changeBackground(const std::string& colorCode) {
  return "\033[48;5;" + colorCode + "m";
}
inline std::string changeColor(const std::string& colorCode) {
  return "\033[38;5;" + colorCode + "m";
}
inline std::string changeColor(uint32_t colorCode) {
  return changeColor(to_string(colorCode));
}
inline std::string changeBackground(uint32_t colorCode) {
  return changeBackground(to_string(colorCode));
}

inline std::string endAllAttributes(const std::string& add = "") {
  std::string output = "\033[0m";
  if (add != "") {
    output.append("\033[" + add + "m");
  }
  return output;
}
template <class T>
void addOtherVec(std::vector<T>& reads, const std::vector<T>& otherVec) {
  reads.reserve(reads.size() + otherVec.size());
  reads.insert(reads.end(), otherVec.begin(), otherVec.end());
}
template <typename T>
bool vectorContains(const std::vector<T>& vec, const T& search) {
  for (const auto& iter : vec) {
    if (iter == search) {
      return true;
    }
  }
  return false;
}
// check to see if a file exists
inline bool fexists(const std::string& filename) {
  std::ifstream ifile(filename.c_str());
  if (ifile) {
    return true;
  } else {
    return false;
  }
}

inline void openTextFile(std::ofstream& file, std::string filename,
                         bool overWrite, bool append, bool exitOnFailure) {
  // std::ofstream file;
  if (fexists(filename) && !overWrite) {
    if (append) {
      file.open(filename.data(), std::ios::app);
    } else {
      std::cout << filename << " already exists" << std::endl;
      if (exitOnFailure) {
        exit(1);
      }
    }

  } else {
    file.open(filename.data());
    if (!file) {
      std::cout << "Error in opening " << filename << std::endl;
      if (exitOnFailure) {
        exit(1);
      }
    } else {
      // chmod(filename.c_str(), S_IRWU|S_IRGRP|S_IWGRP|S_IROTH);
      chmod(filename.c_str(),
            S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      // chmod(filename.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IWGRP|S_IROTH);
    }
  }
}
// with no header
inline void printTableOrganized(const std::vector<VecStr>& content,
                                std::ostream& out) {
  std::map<int, size_t> sizeMap;
  for (const auto& contentIter : content) {
    int count = 0;
    for (const auto& lineIter : contentIter) {
      if (sizeMap.find(count) == sizeMap.end()) {
        sizeMap.insert(std::make_pair(count, lineIter.size()));
      } else {
        if (sizeMap[count] < lineIter.size()) {
          sizeMap[count] = lineIter.size();
        }
      }
      ++count;
    }
  }
  for (const auto& contentIter : content) {
    int count = 0;
    for (const auto& lineIter : contentIter) {
      out << std::setw((int)sizeMap[count]) << std::left << lineIter;
      out << "\t";
      ++count;
    }
    out << std::endl;
  }
}
// with header
inline void printTableOrganized(const std::vector<VecStr>& content,
                                const VecStr& header, std::ostream& out) {
  std::map<int, size_t> sizeMap;
  {
    int count = 0;
    for (const auto& lineIter : header) {
      if (sizeMap.find(count) == sizeMap.end()) {
        sizeMap.insert(std::make_pair(count, lineIter.size()));
      } else {
        if (sizeMap[count] < lineIter.size()) {
          sizeMap[count] = lineIter.size();
        }
      }
      ++count;
    }
  }
  for (const auto& contentIter : content) {
    int count = 0;
    for (const auto& lineIter : contentIter) {
      if (sizeMap.find(count) == sizeMap.end()) {
        sizeMap.insert(std::make_pair(count, lineIter.size()));
      } else {
        if (sizeMap[count] < lineIter.size()) {
          sizeMap[count] = lineIter.size();
        }
      }
      ++count;
    }
  }
  {
    int count = 0;
    for (const auto& lineIter : header) {
      out << std::setw((int)sizeMap[count]) << std::left << lineIter;
      out << "\t";
      ++count;
    }
    out << std::endl;
  }
  for (const auto& contentIter : content) {
    int count = 0;
    for (const auto& lineIter : contentIter) {
      out << std::setw((int)sizeMap[count]) << std::left << lineIter;
      out << "\t";
      ++count;
    }
    out << std::endl;
  }
}
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

inline int getdir(const std::string& dir,
                  std::map<std::string, std::pair<std::string, bool>>& files) {
  DIR* dp;
  struct dirent* dirp;
  if ((dp = opendir(dir.c_str())) == NULL) {
    std::cout << "Error(" << errno << ") opening " << dir << std::endl;
    return errno;
  }
  int status = 0;
  while ((dirp = readdir(dp)) != NULL) {

    struct stat st_buf;
    std::stringstream tempName;
    tempName << dir << "/" << std::string(dirp->d_name).c_str();
    // status=stat(std::string(dirp->d_name).c_str(),&st_buf);
    status = stat(tempName.str().c_str(), &st_buf);
    if (std::string(dirp->d_name) == "." || std::string(dirp->d_name) == "..") {
    } else {
      if (S_ISDIR(st_buf.st_mode)) {
        files.insert(
            std::make_pair(tempName.str(), std::make_pair("directory", false)));
      } else {
        files.insert(
            std::make_pair(tempName.str(), std::make_pair("file", false)));
      }
    }
  }
  closedir(dp);
  return status;
}

inline std::map<std::string, std::pair<std::string, bool>> listFilesInDir(
    const std::string& directoryName, bool recursive) {
  std::map<std::string, std::pair<std::string, bool>> files;
  std::map<std::string, std::pair<std::string, bool>>::iterator fileIter;
  getdir(directoryName, files);
  if (recursive) {
    bool searching = true;
    while (searching) {
      for (fileIter = files.begin(); fileIter != files.end(); ++fileIter) {
        if (fileIter->second.first == "directory" && !fileIter->second.second) {
          getdir(fileIter->first, files);
          fileIter->second.second = true;
        }
      }
      for (fileIter = files.begin(); fileIter != files.end(); ++fileIter) {
        searching = false;
        if (fileIter->second.first == "directory" && !fileIter->second.second) {
          searching = true;
          break;
        }
      }
    }
  }
  return files;
}
inline std::string replaceString(std::string theString,
                                 const std::string& replaceSpace,
                                 const std::string& newSpace) {
  size_t spaceSize = replaceSpace.size();
  size_t currPos = theString.find(replaceSpace);
  while (currPos != std::string::npos) {
    theString.replace(currPos, spaceSize, newSpace);
    currPos = theString.find(replaceSpace, currPos + newSpace.size());
  }
  return theString;
}

struct scoreMatrixCell {
  int32_t upInherit;
  int32_t leftInherit;
  int32_t diagInherit;
  // for traceback: 'U' = up, 'L' = Left, 'D' = diagonal, 'B' either up or left
  char upInheritPtr;
  char leftInheritPtr;
  char diagInheritPtr;
};
inline int32_t needleMaximum(int32_t u, int32_t l, int32_t d, char& p) {
  if (u == l && u == d) {
    p = 'B';
    return u;
  } else if (u >= l && u >= d) {
    if (u == l) {
      p = 'B';
      return u;
    } else {
      p = 'U';
      return u;
    }
  } else if (l >= u && l >= d) {
    if (l == u) {
      p = 'B';
      return l;
    } else {
      p = 'L';
      return l;
    }
  } else {
    p = 'D';
    return d;
  }
}
inline int32_t needleScore(const std::string& objA, const std::string& objB) {
  uint32_t maxSize = 0;
  if (objA.size() > maxSize) {
    maxSize = objA.size();
  }
  if (objB.size() > maxSize) {
    maxSize = objB.size();
  }
  maxSize += 10;
  int32_t gapOpen = 5;
  int32_t gapExtend = 1;
  std::vector<std::vector<scoreMatrixCell>> ScoreMatrix =
      std::vector<std::vector<scoreMatrixCell>>(
          maxSize, std::vector<scoreMatrixCell>(maxSize));
  // std::cout << "doing needle reg" << std::endl;
  // empty the alignment strings and qualities vectors to reset for the new
  // alignment
  // std::cout<<"mark needle non simple"<<std::endl;
  /*if (currentSetUp_ != "needle") {
          initializeNeedle();
  }*/
  // Create the alignment score matrix to do the alignment, a column for each
  // letter in sequence b and a row for each letter in sequence a
  ScoreMatrix[0][0].leftInherit = 0;
  ScoreMatrix[0][0].upInherit = 0;
  ScoreMatrix[0][0].diagInherit = 0;
  ScoreMatrix[0][0].upInheritPtr = '\0';
  ScoreMatrix[0][0].leftInheritPtr = '\0';
  ScoreMatrix[0][0].diagInheritPtr = '\0';
  // initialize first column:
  for (uint32_t i = 1; i < maxSize; i++) {
    if (i == 1) {
      ScoreMatrix[i][0].upInherit = -gapOpen;
      ScoreMatrix[i][0].leftInherit = 0;
      ScoreMatrix[i][0].diagInherit = 0;
      ScoreMatrix[i][0].upInheritPtr = 'U';
      ScoreMatrix[i][0].leftInheritPtr = '\0';
      ScoreMatrix[i][0].diagInheritPtr = '\0';
    } else {
      ScoreMatrix[i][0].upInherit = ScoreMatrix[i - 1][0].upInherit - gapExtend;
      ScoreMatrix[i][0].leftInherit = 0;
      ScoreMatrix[i][0].diagInherit = 0;
      ScoreMatrix[i][0].upInheritPtr = 'U';
      ScoreMatrix[i][0].leftInheritPtr = '\0';
      ScoreMatrix[i][0].diagInheritPtr = '\0';
    }
  }
  // initialize first row:
  for (uint32_t j = 1; j < maxSize; j++) {
    if (j == 1) {
      ScoreMatrix[0][j].upInherit = 0;
      ScoreMatrix[0][j].leftInherit = -gapOpen;
      ScoreMatrix[0][j].diagInherit = 0;
      ScoreMatrix[0][j].upInheritPtr = '\0';
      ScoreMatrix[0][j].leftInheritPtr = 'L';
      ScoreMatrix[0][j].diagInheritPtr = '\0';
    } else {
      ScoreMatrix[0][j].upInherit = 0;
      ScoreMatrix[0][j].leftInherit =
          ScoreMatrix[0][j - 1].leftInherit - gapExtend;
      ScoreMatrix[0][j].diagInherit = 0;
      ScoreMatrix[0][j].upInheritPtr = '\0';
      ScoreMatrix[0][j].leftInheritPtr = 'L';
      ScoreMatrix[0][j].diagInheritPtr = '\0';
    }
  }
  // objectA=objA;
  // objectB=objB;

  // alignObjectA_.clear();
  // alignObjectB_.clear();
  // get the length of the strings to create the alignment score matrix
  uint32_t lena = objA.size() + 1;
  uint32_t lenb = objB.size() + 1;
  // std::cout<<"length of A"<<lena<<std::endl;
  // std::cout<<"length of B"<<lenb<<std::endl;
  for (uint32_t i = 1; i < lena; i++) {
    for (uint32_t j = 1; j < lenb; j++) {
      // std::cout<<"i: "<<i<<"j: "<<j<<std::endl;
      char ptrFlag;
      // first set the upInherit score. If we are in the first row, this will
      // be
      // the value of the above cell's leftInherit score minus a gap open
      // penalty.
      // Otherwise, it will be the max of the three scores in the cell above,
      // with the appropriate penalty applied (either a
      // parts.gapScores_.gapOpen or
      // gapExtend).
      if (i == 1) {
        if (j == lenb - 1) {
          ScoreMatrix[i][j].upInherit =
              ScoreMatrix[i - 1][j].leftInherit - gapOpen;
        } else {
          ScoreMatrix[i][j].upInherit =
              ScoreMatrix[i - 1][j].leftInherit - gapOpen;
        }
        ScoreMatrix[i][j].upInheritPtr = 'L';
      } else {
        if (j == lenb - 1) {
          ScoreMatrix[i][j].upInherit = needleMaximum(
              ScoreMatrix[i - 1][j].upInherit - gapExtend,
              ScoreMatrix[i - 1][j].leftInherit - gapOpen,
              ScoreMatrix[i - 1][j].diagInherit - gapOpen, ptrFlag);
          ScoreMatrix[i][j].upInheritPtr = ptrFlag;
        } else {
          ScoreMatrix[i][j].upInherit = needleMaximum(
              ScoreMatrix[i - 1][j].upInherit - gapExtend,
              ScoreMatrix[i - 1][j].leftInherit - gapOpen,
              ScoreMatrix[i - 1][j].diagInherit - gapOpen, ptrFlag);
          ScoreMatrix[i][j].upInheritPtr = ptrFlag;
        }
      }

      // next set the leftInherit score. If we are in the first column, this
      // will be the value of the left cell's upInherit score minus a gap open
      // penalty.
      // Otherwise, it will be the max score of the three scores in the cell
      // to
      // the left, with the appropriate penalty applied.
      if (j == 1) {
        if (i == lena - 1) {
          ScoreMatrix[i][j].leftInherit =
              ScoreMatrix[i][j - 1].upInherit - gapOpen;
        } else {
          ScoreMatrix[i][j].leftInherit =
              ScoreMatrix[i][j - 1].upInherit - gapOpen;
        }
        ScoreMatrix[i][j].leftInheritPtr = 'U';
      } else {
        if (i == lena - 1) {
          ScoreMatrix[i][j].leftInherit = needleMaximum(
              ScoreMatrix[i][j - 1].upInherit - gapOpen,
              ScoreMatrix[i][j - 1].leftInherit - gapExtend,
              ScoreMatrix[i][j - 1].diagInherit - gapOpen, ptrFlag);
          ScoreMatrix[i][j].leftInheritPtr = ptrFlag;
        } else {
          ScoreMatrix[i][j].leftInherit = needleMaximum(
              ScoreMatrix[i][j - 1].upInherit - gapOpen,
              ScoreMatrix[i][j - 1].leftInherit - gapExtend,
              ScoreMatrix[i][j - 1].diagInherit - gapOpen, ptrFlag);
          ScoreMatrix[i][j].leftInheritPtr = ptrFlag;
        }
      }
      // int match = scoringArray[objA.seqBase_.seq_[i -
      // 1]-'A'][objB.seqBase_.seq_[j - 1]-'A'];
      int match = 0;
      if (objA[i - 1] == objB[j - 1]) {
        match = 2;
      } else {
        match = -2;
      }

      if (i == 1) {
        ScoreMatrix[i][j].diagInherit =
            ScoreMatrix[i - 1][j - 1].leftInherit + match;
        ScoreMatrix[i][j].diagInheritPtr = 'L';
      } else if (j == 1) {
        ScoreMatrix[i][j].diagInherit =
            ScoreMatrix[i - 1][j - 1].upInherit + match;
        ScoreMatrix[i][j].diagInheritPtr = 'U';
      } else {
        ScoreMatrix[i][j].diagInherit =
            match + needleMaximum(ScoreMatrix[i - 1][j - 1].upInherit,
                                  ScoreMatrix[i - 1][j - 1].leftInherit,
                                  ScoreMatrix[i - 1][j - 1].diagInherit,
                                  ptrFlag);
        ScoreMatrix[i][j].diagInheritPtr = ptrFlag;
      }
    }
  }
  int icursor = lena - 1;
  int jcursor = lenb - 1;

  // tracerNext holds to where to go next in the matrix, will be (D) diagonal,
  // (U) up, or (L) left depending on the maximum score determined during the
  // matrix set up.
  char tracerNext = ' ';

  // get the alignment score from the  bottom right cell and set the tacer to
  // where to go next
  // keep tracing back until at the begining of either sequence
  // Traceback algorithm follows. Score is the max of all three scores stored
  // in
  // the bottom right cell.
  // Alignments are constructed by following the correct pointer backwards at
  // each stage.
  // Since alignment strings are constructed in reverse, we must call the
  // reverse() funcion after traceback.
  int32_t score =
      needleMaximum(ScoreMatrix[icursor][jcursor].upInherit,
                    ScoreMatrix[icursor][jcursor].leftInherit,
                    ScoreMatrix[icursor][jcursor].diagInherit, tracerNext);
  return score;
}

}  // cppprogutils
