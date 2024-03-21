//
// Created by saleh on 3/13/24.
//

#pragma once

#include <cassert>
#include <sstream>
#include "cppparser.h"
#include "cppwriter.h"
#include "cppast.h"

inline CppObjPtr cloneCppObj(CppObj *pSrc, const std::string &tempFileBaseDir="/tmp/") {
    CppWriter writer;
    std::stringstream stm;

    writer.emit(pSrc, stm);
    stm.flush();

    CppParser parser;
    auto uniquePtr =  parser.parseString(tempFileBaseDir + "temp.cpp", stm.str());
    auto rawPtr = uniquePtr.release(); // give up the ownership, BAD IDEA!
    auto member = std::move(rawPtr->members()[0]);
    delete rawPtr; // avoid m.leak, BAD IDEA!
    return member;
}
