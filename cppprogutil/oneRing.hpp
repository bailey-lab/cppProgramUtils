#pragma once
//
//  oneRing.hpp
//  sequenceTools
//
//  Created by Nicholas Hathaway on 11/22/13.
//  Copyright (c) 2013 Nicholas Hathaway. All rights reserved.
//

#include "cppprogutil/programRunner.hpp"
namespace cppprogutil {

class oneRing : public programRunner {

  const std::map<std::string, programRunner> rings_;

 protected:
  std::pair<std::string, programRunner> addRing(programRunner runner,
                                                bool lower = true) {
    auto name = runner.nameOfProgram_;
    if (lower) {
      stringToLower(name);
    }
    return {name, runner};
  }

  void listCommands(std::ostream &out) {}

 public:
  oneRing(std::map<std::string, programRunner> rings,
          std::map<std::string, funcInfo> cmdToFunc, std::string nameOfProgram)
      : programRunner(cmdToFunc, nameOfProgram), rings_(rings) {}

  virtual ~oneRing() {};
  virtual int runProgram(MapStrStr inputCommands) const;

  virtual void listPrograms(std::ostream &out, const std::string &command = "",
                            const std::string &nameOfProgram = "oneRing") const;
};
int oneRing::runProgram(MapStrStr inputCommands) const {
  std::string prog = inputCommands["-program"];
  if (containsProgram(prog)) {
    const auto &fi = cmdToFunc_.at(prog);
    return fi.func(inputCommands);
  }

  // If given as PROGRAM_NUM will run the PROGRAM's NUM command
  std::string progName = "";
  std::string progNumber = "";
  if (prog.find("_") != std::string::npos) {
    VecStr toks = tokenizeString(prog, "_");
    if (toks.size() == 2) {
      progName = toks[0];
      progNumber = toks[1];
      if (stringContainsAllDigits(progNumber)) {
        // inputCommands.erase(prog);
        if (rings_.find(progName) != rings_.end()) {
          return rings_.at(progName).runByNumber(progNumber, inputCommands);
        }
      }
    }
  }
  if (rings_.find(prog) != rings_.end()) {
    rings_.at(prog).listPrograms(std::cout, "", "none");
    return 1;
  }
  std::pair<std::string, int> closestProgram = {"", 0};
  for (auto &ring : rings_) {
    if (ring.second.containsProgram(prog)) {
      return ring.second.runProgram(inputCommands);
    } else {
      std::pair<std::string, int> currentClosestProgram =
          ring.second.closestProgram(prog);
      if (currentClosestProgram.second > closestProgram.second) {
        closestProgram = currentClosestProgram;
      }
    }
  }
  listPrograms(std::cout, inputCommands["-program"]);
  std::cout << "Input command " << prog << std::endl;
  std::cout << "Did you mean  " << closestProgram.first << "?" << std::endl;
  return 1;
}

void oneRing::listPrograms(std::ostream &out, const std::string &command,
                           const std::string &nameOfProgram) const {
  if (command != "") {
    out << "Unrecognized command " << command << std::endl;
  }
  out << "Programs" << std::endl;
  out << "Use " << nameOfProgram_
      << " [PROGRAM] -help to see more details about each program" << std::endl;
  out << "Commands are not case sensitive" << std::endl;

  for (auto &ring : rings_) {
    ring.second.listPrograms(std::cout, "", nameOfProgram);
  }
}
} // cppprogutil

