#pragma once
/*
 * utils.hpp
 *
 *  Created on: Jul 19, 2014
 *      Author: nickhathaway
 */

#include "cppprogutils/common.hpp"


namespace cppprogutils {

/**@b Print out the contents of a map with column adjusted, should only be called on maps of std::string,std::string but templated so
 * different it can be used on multiple map types
 *
 * @param theMap The map to print
 * @param out The stream to print o
 */
template <typename MAP>
void mapOutColAdjust(const MAP& theMap,
                         std::ostream& out) {
	uint64_t keyMaxLen = 0;
	uint64_t valueMaxLen = 0;
  for (const auto& mValue : theMap) {
    if(mValue.first.size() > keyMaxLen){
    	keyMaxLen = mValue.first.size();
    }
    if(mValue.second.size() > valueMaxLen){
    	valueMaxLen = mValue.second.size();
    }
  }
  for (const auto& mValue : theMap) {
    out << std::right << std::setw(keyMaxLen) << mValue.first << "     "
    		<< std::left << std::setw(valueMaxLen) << mValue.second << "\n";
  }
}

/**@b Get a vector of all the values stored in theMap
 *
 * @param theMap The map to get the values from
 * @return A vector of the mapped values
 */
template <typename MAP>
static std::vector<typename MAP::mapped_type> getVecOfMapValues(const MAP& theMap) {
  std::vector<typename MAP::mapped_type> ret;
  for (const auto & mValue : theMap) {
    ret.push_back(mValue.second);
  }
  return ret;
}

/**@b Get a vector of all the keys stored in theMap
 *
 * @param theMap The map to get the keys from
 * @return A vector of the key values
 */
template <typename MAP>
static std::vector<typename MAP::key_type> getVecOfMapKeys(const MAP& theMap) {
  std::vector<typename MAP::key_type> ret;
  for (const auto & mValue : theMap) {
    ret.push_back(mValue.first);
  }
  return ret;
}


inline double roundDecPlaces(double num, int decPlaces) {
  double rounder = pow(10, decPlaces);
  return (floor(num * rounder + 0.5) / rounder);
}

template <class T>
void addOtherVec(std::vector<T>& vec, const std::vector<T>& otherVec) {
  vec.reserve(vec.size() + otherVec.size());
  vec.insert(vec.end(), otherVec.begin(), otherVec.end());
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




// with header
inline void printTableOrganized(const std::vector<VecStr>& content,
                                const VecStr& header, std::ostream& out) {
  std::map<int, size_t> sizeMap;
  if(!header.empty()){
    int count = 0;
    for (const auto& h : header) {
      if (sizeMap.find(count) == sizeMap.end()) {
        sizeMap.insert(std::make_pair(count, h.size()));
      } else {
        if (sizeMap[count] < h.size()) {
          sizeMap[count] = h.size();
        }
      }
      ++count;
    }
  }
  for (const auto & row : content) {
    int count = 0;
    for (const auto& col : row) {
      if (sizeMap.find(count) == sizeMap.end()) {
        sizeMap.insert(std::make_pair(count, col.size()));
      } else {
        if (sizeMap[count] < col.size()) {
          sizeMap[count] = col.size();
        }
      }
      ++count;
    }
  }
  if(!header.empty()){
    int count = 0;
    for (const auto& h : header) {
      out << std::setw(sizeMap[count]) << std::left << h;
      out << "\t";
      ++count;
    }
    out << std::endl;
  }
  for (const auto& row : content) {
    int count = 0;
    for (const auto& col : row) {
      out << std::setw(sizeMap[count]) << std::left << col;
      out << "\t";
      ++count;
    }
    out << std::endl;
  }
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

}  // namespace cppprogutils
