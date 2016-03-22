#pragma once

/*
 * filesystemUtils.hpp
 *
 *  Created on: Aug 4, 2014
 *      Author: nickhathaway
 */


#include "cppprogutils/common.hpp"
namespace cppprogutils {

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

// check to see if a file exists
inline bool fexists(const std::string& filename) {
  std::ifstream ifile(filename.c_str());
  if (ifile) {
    return true;
  } else {
    return false;
  }
}


/**@brief  append to a str only if need
 *
 * @param str The string to append to
 * @param app What to append to str
 * @return A reference to the str
 */
inline std::string & appendAsNeeded(std::string & str, const std::string & app){
	if(!endsWith(str,app)){
		str.append(app);

	}
	return str;
}

/**@brief Return str appended if required
 *
 * @param str The str to append to
 * @param app What to append to the str
 * @return str appended with app only if it is not already appended
 */
inline std::string appendAsNeededRet(std::string str, const std::string & app){
	if(!endsWith(str,app)){
		str.append(app);
	}
	return str;
}

/**@brief Joiningng parent path with child path with a unix dir separator if needed
 *
 * Modeled after python's os.path.join
 *
 * @param par A parent path
 * @param child A file under parent path
 * @return Return par plus a unix directory separator if neccesary plus child
 */
inline std::string join(const std::string & par, const std::string & child){
	return appendAsNeededRet(par, "/") + child;
}

/**@brief Open a ofstream with filename and checking for file existence
 *
 * @param file the ofstream object to open
 * @param filename The name of the file to open
 * @param overWrite Whether the file should be overwritten if it already exists
 * @param append Whether the file should be appended if it already exists
 * @param exitOnFailure whether program should exit on failure to open the file
 * @todo probably should just remove exitOnFailure and throw an exception instead
 */
inline void openTextFile(std::ofstream& file, const std::string & filename,
                         bool overWrite, bool append, bool exitOnFailure) {

  if (fexists(filename) && !overWrite) {
    if (append) {
      file.open(filename.data(), std::ios::app);
    } else {
    	std::stringstream ss;
      ss << filename << " already exists";
      if (exitOnFailure) {
        throw std::runtime_error{ss.str()};
      }else{
      	std::cout << ss.str() << std::endl;
      }
    }
  } else {
    file.open(filename.data());
    if (!file) {
    	std::stringstream ss;
    	ss << "Error in opening " << filename;
      if (exitOnFailure) {
        throw std::runtime_error{ss.str()};
      }else{
      	std::cout << ss.str() << std::endl;
      }
    } else {
      chmod(filename.c_str(),
            S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }
  }
}

/**@brief Open a ofstream with filename and checking for file existence
 *
 * @param file the ofstream object to open
 * @param filename The name of the file to open
 * @param overWrite Whether the file should be overwritten if it already exists
 * @param append Whether the file should be appended if it already exists
 * @param exitOnFailure whether program should exit on failure to open the file
 * @todo probably should just remove exitOnFailure and throw an exception instead
 */
inline void openTextFile(std::ofstream& file, std::string filename, const std::string & extention,
                         bool overWrite, bool append, bool exitOnFailure) {
	appendAsNeeded(filename, extention);
	openTextFile(file, filename, overWrite, append, exitOnFailure);
}

/**@brief Get the streambuf of either an opened file or of std::cout if outFile is empty
 *
 * @param outFile The ofstream to open if needed
 * @param outFilename The name of the file, leave blank to get std::cout buffer
 * @param outFileExt The extention for the file
 * @param overWrite Whether to overwrite the file
 * @param append Whether to append to the file
 * @param exitOnFailure Whether if writing fails if the program should throw or just warn
 * @return A std::streambuf* or either an opened file or of std::cout
 */
inline std::streambuf* determineOutBuf(std::ofstream & outFile,
		const std::string & outFilename, const std::string outFileExt,
		bool overWrite, bool append, bool exitOnFailure) {
	if (outFilename != "") {
		openTextFile(outFile, outFilename,outFileExt, overWrite,
				append, exitOnFailure);
		return outFile.rdbuf();
	} else {
		return std::cout.rdbuf();
	}
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

}  // namespace cppprogutils

