//
// Created by saleh on 3/13/24.
//

#pragma once

#include <sstream>
#include "cppparser.h"
#include "cppwriter.h"
#include "cppast.h"

inline CppObj* cloneAny(const CppObj *pSrc) {
    CppWriter writer;
    std::stringstream stm;

    writer.emit(pSrc, stm);
    stm.flush();

    CppParser parser;
    return parser.parseString(stm.str()).get();
}
