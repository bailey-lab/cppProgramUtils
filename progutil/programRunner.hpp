#pragma once
//
//  programRunner.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 9/27/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "progutil/utils.hpp"
#include "progutil/programSetUp.hpp"
namespace progutil {

class programRunner {
 protected:
  struct funcInfo {
    std::function<int(MapStrStr)> func;
    std::string title;
    bool alias;
  };

  const std::map<std::string, funcInfo> cmdToFunc_;

 public:
  const std::string nameOfProgram_;
  programRunner(std::map<std::string, funcInfo> cmdToFunc,
                std::string nameOfProgram)
      : cmdToFunc_(cmdToFunc), nameOfProgram_(nameOfProgram) {}

  virtual ~programRunner();

  virtual int run(int argc, char* argv[]);

  virtual int runProgram(MapStrStr inputCommands) const;

  virtual void listPrograms(std::ostream& out, const std::string& command = "",
                            const std::string& nameOfProgram = "programRunner")
      const;

  virtual bool containsProgram(const std::string& program) const;
  virtual std::pair<std::string, int> closestProgram(const std::string& program)
      const;
  virtual int massRunWithEnding(MapStrStr inputCommands);
  virtual int massRunWithEndingThreaded(MapStrStr inputCommands);

  int runByNumber(size_t num, MapStrStr inputCommands) const;
  int runByNumber(std::string numStr, MapStrStr inputCommands) const;

 protected:
  template <typename T>
  std::pair<std::string, funcInfo> addFunc(std::string title, T& func,
                                           bool alias, bool lower = true) {
    auto name = title;
    if (lower) {
      stringToLower(name);
    }
    return {name, {std::bind(&func, std::placeholders::_1), title, alias}};
  }

  void listCommands(std::ostream& out) const {
    size_t count = 0;
    for (const auto & e : cmdToFunc_) {
      if (!e.second.alias) {
      	++count;
				out << leftPadNumStr(count, cmdToFunc_.size()) << ") "
						<< e.second.title << std::endl;
      }
    }
  }
};
programRunner::~programRunner() {}

int programRunner::run(int argc, char *argv[]) {
  if (argc < 2) {
    listPrograms(std::cout);
    return 1;
  }
  std::stringstream commandLine;
  commandLineArguments::logRunArgumnets(commandLine, argc, argv);
  MapStrStr commands = commandLineArguments::convertCommandLineArguments(argc, argv);
  commands["-program"] = stringToLowerReturn(argv[1]);
  commands["-commandline"] = commandLine.str();

  if (commands["-program"] == "massrunwithending") {
    return massRunWithEnding(commands);
  }
  if (commands["-program"] == "massrunwithendingthreaded") {
    return massRunWithEndingThreaded(commands);
  }
  if (stringContainsAllDigits(commands["-program"])) {
    return runByNumber(commands["-program"], commands);
  }
  return runProgram(commands);
}

int programRunner::runByNumber(size_t num, MapStrStr inputCommands) const {
  size_t count = 0;
  for (const auto & e : cmdToFunc_) {
    if (!e.second.alias) {
    	++count;
      if (count == num) {
        const auto &fi = e.second;
        inputCommands["-program"] = fi.title;
        return fi.func(inputCommands);
      }
    }
  }
  std::cout << "Num: " << num
            << " is greater than size of command list: " << cmdToFunc_.size()
            << std::endl;
  return 1;
}

int programRunner::runByNumber(std::string numStr,
                               MapStrStr inputCommands) const {
  if (stringContainsAllDigits(numStr)) {
    return runByNumber(std::stoi(numStr), inputCommands);
  }
  std::cout << "NumStr isn't a number: " << numStr << std::endl;
  return 1;
}

int programRunner::massRunWithEnding(MapStrStr inputCommands) {
  std::string ending = "", program = "";
  programSetUp setUp(inputCommands);
  setUp.setOption(ending, "-ending", "FileEnding", true);
  setUp.setOption(program, "-run", "ProgramToRun", true);
  setUp.printWarnings(std::cout);
  if (setUp.failed_) {
    exit(1);
  }
  bool overWrite = false;
  bool exitOnFailureToWrite = true;
  // create run log
  std::ofstream runLog;
  std::string nameOfRunLog =
      "massRunLog_" + inputCommands["-run"] + "_" + getCurrentDate() + ".txt";
  openTextFile(runLog, nameOfRunLog, ".txt", overWrite, exitOnFailureToWrite);
  runLog << "Ran " << getCurrentDate() << std::endl;
  runLog <<  inputCommands.find("-commandline")->second << std::endl;
  // erase the massRunWithEnding flags to remove
  inputCommands.erase("-ending");
  inputCommands.erase("-program");
  inputCommands.erase("massrunwithending");
  // gather the neccessary files
  auto allFiles = listFilesInDir(".", false);
  std::map<std::string, std::pair<std::string, bool>> specificFiles;
  for (const auto &file : allFiles) {
    if (file.first.size() > ending.size()) {
      if (file.first.substr(file.first.size() - ending.size(), ending.size()) ==
          ending) {
        specificFiles.insert(file);
      }
    }
  }
  // run the command on each file
  for (const auto &file : specificFiles) {
    if (containsSubString(file.first, "massRunLog")) {
      continue;
    }
    MapStrStr currentCommands = inputCommands;
    for (auto &com : currentCommands) {
      com.second = replaceString(com.second, "THIS", file.first);
    }
    // rebuild the commandline for each new run
    VecStr toks = tokenizeString(currentCommands["-commandline"], "\n");
    std::string from = toks[0];
    std::stringstream currentCommandLine;
    currentCommandLine << from << std::endl;
    currentCommandLine << "sequenceTools " << currentCommands["-run"];
    currentCommands.erase("-run");
    for (auto &com : currentCommands) {
      if (com.first != "-commandline") {
        currentCommandLine << " " << com.first << " " << com.second;
      }
    }
    currentCommandLine << std::endl;
    // and now that the commandLine has been rebuilt for this run replace the
    // other one.
    currentCommands["-commandline"] = currentCommandLine.str();
    // set the program to be run
    currentCommands["-program"] = stringToLowerReturn(program);
    // log current run command
    std::cout << currentCommands["-commandline"] << std::endl;
    runLog << currentCommands["-commandline"] << std::endl;
    setUp.logRunTime(runLog);
    setUp.logRunTime(std::cout);
    std::cout << std::endl;
    // run the current command
    // std::cout<<currentCommands["-program"]<<std::endl;
    runProgram(currentCommands);
    std::cout << std::endl;
  }
  setUp.logRunTime(runLog);
  setUp.logRunTime(std::cout);
  return 0;
}

int programRunner::massRunWithEndingThreaded(MapStrStr inputCommands) {
  std::string ending = "", program = "";
  programSetUp setUp(inputCommands);
  setUp.setOption(ending, "-ending", "FileEnding", true);
  setUp.setOption(program, "-run", "ProgramToRun", true);
  uint32_t numThreads = 2;
  setUp.setOption(numThreads, "-numThreads,-threads", "numThreads");
  setUp.printWarnings(std::cout);
  if (setUp.failed_) {
    exit(1);
  }
  bool overWrite = false;
  bool exitOnFailureToWrite = true;
  // create run log
  std::ofstream runLog;
  std::string nameOfRunLog =
      "massRunLog_" + inputCommands["-run"] + "_" + getCurrentDate() + ".txt";
  openTextFile(runLog, nameOfRunLog, ".txt", overWrite, exitOnFailureToWrite);
  runLog << "Ran " << getCurrentDate() << std::endl;
  runLog <<  inputCommands.find("-commandline")->second << std::endl;
  // erase the massRunWithEnding flags to remove
  inputCommands.erase("-ending");
  inputCommands.erase("-program");
  inputCommands.erase("massrunwithending");
  if(inputCommands.find("-numThreads") != inputCommands.end() ){
  	inputCommands.erase("-numThreads");
  }
  if(inputCommands.find("-threads")  != inputCommands.end()){
  	inputCommands.erase("-threads");
  }
  // gather the necessary files
  auto allFiles = listFilesInDir(".", false);
  std::map<std::string, std::pair<std::string, bool>> specificFiles;
  for (const auto &file : allFiles) {
    if (file.first.size() > ending.size()) {
      if (file.first.substr(file.first.size() - ending.size(), ending.size()) ==
          ending) {
        specificFiles.insert(file);
      }
    }
  }
  // run the command on each file
  std::vector<MapStrStr> allCommands;
  for (const auto &file : specificFiles) {
    if (containsSubString(file.first, "massRunLog")) {
      continue;
    }
    MapStrStr currentCommands = inputCommands;
    for (auto &com : currentCommands) {
      com.second = replaceString(com.second, "THIS", file.first);
    }

    // rebuild the commandline for each new run
    VecStr toks = tokenizeString(currentCommands["-commandline"], "\n");
    std::string from = toks[0];
    std::stringstream currentCommandLine;
    currentCommandLine << from << std::endl;
    currentCommandLine << "sequenceTools " << currentCommands["-run"];
    currentCommands.erase("-run");
    for (auto &com : currentCommands) {
      if (com.first != "-commandline") {
        currentCommandLine << " " << com.first << " " << com.second;
      }
    }
    currentCommandLine << std::endl;
    // and now that the commandLine has been rebuilt for this run replace the
    // other one.
    currentCommands["-commandline"] = currentCommandLine.str();
    // set the program to be run
    currentCommands["-program"] = stringToLowerReturn(program);
    // log current run command
    std::cout << currentCommands["-commandline"] << std::endl;
    //runLog << currentCommands["-commandline"] << std::endl;
    //setUp.logRunTime(runLog);
    setUp.logRunTime(std::cout);
    std::cout << std::endl;
    // run the current command
    // std::cout<<currentCommands["-program"]<<std::endl;
    //runProgram(currentCommands);
    allCommands.emplace_back(currentCommands);
    std::cout << std::endl;
  }
  std::vector<uint32_t> comNums;
  uint32_t pos = 0;
  while (pos < allCommands.size()){
  	comNums.emplace_back(pos);
  	++pos;
  }
  std::vector<uint32_t> threadNums(numThreads);
  std::iota(threadNums.begin(), threadNums.end(), 0);

  for(const auto & comPos : comNums){
  	std::vector<std::thread> threads;
		for(const auto & t : threadNums){
			if((t + comPos) < allCommands.size()){
				threads.emplace_back(std::thread([&](MapStrStr com){ runProgram(com);}, allCommands[t + comPos]));
			}
		}
		for(auto& t : threads){
			t.join();
		}

  }
  setUp.logRunTime(runLog);
  setUp.logRunTime(std::cout);
  return 0;
}

bool programRunner::containsProgram(const std::string &program) const {
  return cmdToFunc_.find(program) != cmdToFunc_.end();
}
std::pair<std::string, int> programRunner::closestProgram(
    const std::string &program) const {
  std::pair<std::string, int> ans = {"", 0};
  for (const auto &prog : cmdToFunc_) {
    int32_t currentScore = needleScore(program, prog.first);
    if (currentScore > ans.second) {
      ans = {prog.first, currentScore};
    }
  }
  return ans;
}
int programRunner::runProgram(MapStrStr inputCommands) const {
  if (containsProgram(inputCommands["-program"])) {
    const auto &fi = cmdToFunc_.at(inputCommands["-program"]);
    return fi.func(inputCommands);
  }
  listPrograms(std::cout, inputCommands["-program"]);
  return 1;
}

void programRunner::listPrograms(std::ostream &out, const std::string &command,
                                 const std::string &nameOfProgram) const {
  if (command != "") {
    out << "Unrecognized command " << command << std::endl;
  }
  if (nameOfProgram == nameOfProgram_) {
    out << "Programs" << std::endl;
    out << "Use " << nameOfProgram_
        << " [PROGRAM] -help to see more details about each program"
        << std::endl;
    out << "Commands are not case sensitive" << std::endl;
  } else {
    out << nameOfProgram_ << std::endl;
  }
  listCommands(out);
}
}  // namespace progutil

