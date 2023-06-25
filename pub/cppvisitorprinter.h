//
// Created by saleh on 6/23/23.
//

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "cppvisitorbase.h"
#include "cppwriter.h"

#define DBG_ENABLED
#ifdef DBG_ENABLED
  #define DBG(NAME, PTR) \
      std::cout << NAME << ":" << std::endl; \
      std::cout << astToString(PTR) << std::endl; \
      std::cout << "------------------------------------------"<< std::endl;
#else
#  define DBG(NAME) void;
#endif

class CppVisitorPrinter : public CppVisitorBase
{
public:
  ~CppVisitorPrinter() override = default;

  static std::string astToString(const CppObj* p)
  {
    CppWriter w;
    std::stringstream stm;
    w.emit(p, stm);
    stm.flush();
    return stm.str();
  }

  bool visit(const CppVarType* p) override
  {
    DBG("CppVarType", p);
    return true;
  }
  bool visit(const CppCompound* p) override
  {
    DBG("CppCompound", p);
    return true;
  }
  bool visit(const CppUnRecogPrePro* p) override
  {
    DBG("CppUnRecogPrePro", p);
    return true;
  }
  bool visit(const CppUndef* p) override
  {
    DBG("CppUndef", p);
    return true;
  }
  bool visit(const CppUsingDecl* p) override
  {
    DBG("CppUsingDecl", p);
    return true;
  }
  bool visit(const CppUsingNamespaceDecl* p) override
  {
    DBG("CppUsingNamespaceDecl", p);
    return true;
  }
  bool visit(const CppVar* p) override
  {
    DBG("CppVar", p);
    return true;
  }
  bool visit(const CppVarList* p) override
  {
    DBG("CppVarList", p);
    return true;
  }
  bool visit(const CppCommonBlock<CppObjType::kIfBlock>* p) override
  {
    DBG("CppCommonBlock<CppObjType::kIfBlock>", p);
    return true;
  }
  bool visit(const CppCommonBlock<CppObjType::kWhileBlock>* p) override
  {
    DBG("CppCommonBlock<CppObjType::kWhileBlock>", p);
    return true;
  }
  bool visit(const CppCommonBlock<CppObjType::kDoWhileBlock>* p) override
  {
    DBG("CppCommonBlock<CppObjType::kDoWhileBlock>", p);
    return true;
  }
  bool visit(const CppAsmBlock* p) override
  {
    DBG("CppAsmBlock", p);
    return true;
  }
  bool visit(const CppBlob* p) override
  {
    DBG("CppBlob", p);
    return true;
  }
  bool visit(const CppDefine* p) override
  {
    DBG("CppDefine", p);
    return true;
  }
  bool visit(const CppDocComment* p) override
  {
    DBG("CppDocComment", p);
    return true;
  }
  bool visit(const CppEnum* p) override
  {
    DBG("CppEnum", p);
    return true;
  }
  bool visit(const CppExpr* p) override
  {
    DBG("CppExpr", p);
    return true;
  }
  bool visit(const CppForBlock* p) override
  {
    DBG("CppForBlock", p);
    return true;
  }
  bool visit(const CppLambda* p) override
  {
    DBG("CppLambda", p);
    return true;
  }
  bool visit(const CppDestructor* p) override
  {
    DBG("CppDestructor", p);
    return true;
  }
  bool visit(const CppConstructor* p) override
  {
    DBG("CppConstructor", p);
    return true;
  }
  bool visit(const CppFunction* p) override
  {
    DBG("CppFunction", p);
    return true;
  }
  bool visit(const CppFunctionPointer* p) override
  {
    DBG("CppFunctionPointer", p);
    return true;
  }
  bool visit(const CppTypeConverter* p) override
  {
    DBG("CppTypeConverter", p);
    return true;
  }
  bool visit(const CppFwdClsDecl* p) override
  {
    DBG("CppFwdClsDecl", p);
    return true;
  }
  bool visit(const CppHashError* p) override
  {
    DBG("CppHashError", p);
    return true;
  }
  bool visit(const CppHashIf* p) override
  {
    DBG("CppHashIf", p);
    return true;
  }
  bool visit(const CppHashWarning* p) override
  {
    DBG("CppHashWarning", p);
    return true;
  }
  bool visit(const CppImport* p) override
  {
    DBG("CppImport", p);
    return true;
  }
  bool visit(const CppInclude* p) override
  {
    DBG("CppInclude", p);
    return true;
  }
  bool visit(const CppLabel* p) override
  {
    DBG("CppLabel", p);
    return true;
  }
  bool visit(const CppMacroCall* p) override
  {
    DBG("CppMacroCall", p);
    return true;
  }
  bool visit(const CppNamespaceAlias* p) override
  {
    DBG("CppNamespaceAlias", p);
    return true;
  }
  bool visit(const CppPragma* p) override
  {
    DBG("CppPragma", p);
    return true;
  }
  bool visit(const CppRangeForBlock* p) override
  {
    DBG("CppRangeForBlock", p);
    return true;
  }
  bool visit(const CppSwitchBlock* p) override
  {
    DBG("CppSwitchBlock", p);
    return true;
  }
  bool visit(const CppTryBlock* p) override
  {
    DBG("CppTryBlock", p);
    return true;
  }
  bool visit(const CppTypedefList* p) override
  {
    DBG("CppTypedefList", p);
    return true;
  }
  bool visit(const CppTypedefName* p) override
  {
    DBG("CppTypedefName", p);
    return true;
  }
};