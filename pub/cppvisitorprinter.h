//
// Created by saleh on 6/23/23.
//

#pragma once

/**
 * @file An AST visitor to print the tree nodes utilizing `CppWriter` class.
 * This class has no virtual method and could be used directly.
 */

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

  /**
   * @brief Converts the given pointer to an AST node to its equivalent string of code using `CppWriter` class.
   * @param p A pointer to an AST tree node.
   * @return The string of the equivalent code of the tree node.
   */
  static std::string astToString(CppObj* p)
  {
    CppWriter w;
    std::stringstream stm;
    w.emit(p, stm);
    stm.flush();
    return stm.str();
  }

  bool visit(CppVarType* p) override
  {
    DBG("CppVarType", p);
    return true;
  }
  bool visit(CppCompound* p) override
  {
    DBG("CppCompound", p);
    return true;
  }
  bool visit(CppUnRecogPrePro* p) override
  {
    DBG("CppUnRecogPrePro", p);
    return true;
  }
  bool visit(CppUndef* p) override
  {
    DBG("CppUndef", p);
    return true;
  }
  bool visit(CppUsingDecl* p) override
  {
    DBG("CppUsingDecl", p);
    return true;
  }
  bool visit(CppUsingNamespaceDecl* p) override
  {
    DBG("CppUsingNamespaceDecl", p);
    return true;
  }
  bool visit(CppVar* p) override
  {
    DBG("CppVar", p);
    return true;
  }
  bool visit(CppVarList* p) override
  {
    DBG("CppVarList", p);
    return true;
  }
  bool visit(CppCommonBlock<CppObjType::kIfBlock>* p) override
  {
    DBG("CppCommonBlock<CppObjType::kIfBlock>", p);
    return true;
  }
  bool visit(CppCommonBlock<CppObjType::kWhileBlock>* p) override
  {
    DBG("CppCommonBlock<CppObjType::kWhileBlock>", p);
    return true;
  }
  bool visit(CppCommonBlock<CppObjType::kDoWhileBlock>* p) override
  {
    DBG("CppCommonBlock<CppObjType::kDoWhileBlock>", p);
    return true;
  }
  bool visit(CppAsmBlock* p) override
  {
    DBG("CppAsmBlock", p);
    return true;
  }
  bool visit(CppBlob* p) override
  {
    DBG("CppBlob", p);
    return true;
  }
  bool visit(CppDefine* p) override
  {
    DBG("CppDefine", p);
    return true;
  }
  bool visit(CppDocComment* p) override
  {
    DBG("CppDocComment", p);
    return true;
  }
  bool visit(CppEnum* p) override
  {
    DBG("CppEnum", p);
    return true;
  }
  bool visit(CppExpr* p) override
  {
    DBG("CppExpr", p);
    return true;
  }
  bool visit(CppForBlock* p) override
  {
    DBG("CppForBlock", p);
    return true;
  }
  bool visit(CppLambda* p) override
  {
    DBG("CppLambda", p);
    return true;
  }
  bool visit(CppDestructor* p) override
  {
    DBG("CppDestructor", p);
    return true;
  }
  bool visit(CppConstructor* p) override
  {
    DBG("CppConstructor", p);
    return true;
  }
  bool visit(CppFunction* p) override
  {
    DBG("CppFunction", p);
    return true;
  }
  bool visit(CppFunctionPointer* p) override
  {
    DBG("CppFunctionPointer", p);
    return true;
  }
  bool visit(CppTypeConverter* p) override
  {
    DBG("CppTypeConverter", p);
    return true;
  }
  bool visit(CppFwdClsDecl* p) override
  {
    DBG("CppFwdClsDecl", p);
    return true;
  }
  bool visit(CppHashError* p) override
  {
    DBG("CppHashError", p);
    return true;
  }
  bool visit(CppHashIf* p) override
  {
    DBG("CppHashIf", p);
    return true;
  }
  bool visit(CppHashWarning* p) override
  {
    DBG("CppHashWarning", p);
    return true;
  }
  bool visit(CppImport* p) override
  {
    DBG("CppImport", p);
    return true;
  }
  bool visit(CppInclude* p) override
  {
    DBG("CppInclude", p);
    return true;
  }
  bool visit(CppLabel* p) override
  {
    DBG("CppLabel", p);
    return true;
  }
  bool visit(CppMacroCall* p) override
  {
    DBG("CppMacroCall", p);
    return true;
  }
  bool visit(CppNamespaceAlias* p) override
  {
    DBG("CppNamespaceAlias", p);
    return true;
  }
  bool visit(CppPragma* p) override
  {
    DBG("CppPragma", p);
    return true;
  }
  bool visit(CppRangeForBlock* p) override
  {
    DBG("CppRangeForBlock", p);
    return true;
  }
  bool visit(CppSwitchBlock* p) override
  {
    DBG("CppSwitchBlock", p);
    return true;
  }
  bool visit(CppTryBlock* p) override
  {
    DBG("CppTryBlock", p);
    return true;
  }
  bool visit(CppTypedefList* p) override
  {
    DBG("CppTypedefList", p);
    return true;
  }
  bool visit(CppTypedefName* p) override
  {
    DBG("CppTypedefName", p);
    return true;
  }
};