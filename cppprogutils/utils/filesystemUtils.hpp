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

inline void openTextFile(std::ofstream& file, std::string filename,
                         bool overWrite, bool append, bool exitOnFailure) {
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
      chmod(filename.c_str(),
            S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }
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

