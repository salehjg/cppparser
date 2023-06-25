//
// Created by saleh on 6/25/23.
//

#pragma once

#include <algorithm>
#include <vector>

#include "cppast.h"
#include "cppvisitorbase.h"
#include "cppvisitorprinter.h"

class CppVisitorMatcher : public CppVisitorBase
{
  const std::vector<CppObjType> typesToMatch;

public:
  explicit CppVisitorMatcher(const std::vector<CppObjType>& typesToMatch)
    : typesToMatch(typesToMatch)
  {
  }

  virtual bool matched(const CppObj* p)
  {
    std::cout << CppVisitorPrinter::astToString(p) << std::endl;
    return true;
  }

  ~CppVisitorMatcher() override = default;

private:
  inline bool check(const CppObj* p)
  {
    if (std::find(typesToMatch.begin(), typesToMatch.end(), p->objType_) != typesToMatch.end())
    {
      return matched(p); // continue traversing the tree only if the user wants to.
    }
    return true; // continue traversing the tree to find a possible match.
  }

protected:
  bool visit(const CppVarType* p) override
  {
    return check(p);
  }
  bool visit(const CppCompound* p) override
  {
    return check(p);
  }
  bool visit(const CppUnRecogPrePro* p) override
  {
    return check(p);
  }
  bool visit(const CppUndef* p) override
  {
    return check(p);
  }
  bool visit(const CppUsingDecl* p) override
  {
    return check(p);
  }
  bool visit(const CppUsingNamespaceDecl* p) override
  {
    return check(p);
  }
  bool visit(const CppVar* p) override
  {
    return check(p);
  }
  bool visit(const CppVarList* p) override
  {
    return check(p);
  }
  bool visit(const CppCommonBlock<CppObjType::kIfBlock>* p) override
  {
    return check(p);
  }
  bool visit(const CppCommonBlock<CppObjType::kWhileBlock>* p) override
  {
    return check(p);
  }
  bool visit(const CppCommonBlock<CppObjType::kDoWhileBlock>* p) override
  {
    return check(p);
  }
  bool visit(const CppAsmBlock* p) override
  {
    return check(p);
  }
  bool visit(const CppBlob* p) override
  {
    return check(p);
  }
  bool visit(const CppDefine* p) override
  {
    return check(p);
  }
  bool visit(const CppDocComment* p) override
  {
    return check(p);
  }
  bool visit(const CppEnum* p) override
  {
    return check(p);
  }
  bool visit(const CppExpr* p) override
  {
    return check(p);
  }
  bool visit(const CppForBlock* p) override
  {
    return check(p);
  }
  bool visit(const CppLambda* p) override
  {
    return check(p);
  }
  bool visit(const CppDestructor* p) override
  {
    return check(p);
  }
  bool visit(const CppConstructor* p) override
  {
    return check(p);
  }
  bool visit(const CppFunction* p) override
  {
    return check(p);
  }
  bool visit(const CppFunctionPointer* p) override
  {
    return check(p);
  }
  bool visit(const CppTypeConverter* p) override
  {
    return check(p);
  }
  bool visit(const CppFwdClsDecl* p) override
  {
    return check(p);
  }
  bool visit(const CppHashError* p) override
  {
    return check(p);
  }
  bool visit(const CppHashIf* p) override
  {
    return check(p);
  }
  bool visit(const CppHashWarning* p) override
  {
    return check(p);
  }
  bool visit(const CppImport* p) override
  {
    return check(p);
  }
  bool visit(const CppInclude* p) override
  {
    return check(p);
  }
  bool visit(const CppLabel* p) override
  {
    return check(p);
  }
  bool visit(const CppMacroCall* p) override
  {
    return check(p);
  }
  bool visit(const CppNamespaceAlias* p) override
  {
    return check(p);
  }
  bool visit(const CppPragma* p) override
  {
    return check(p);
  }
  bool visit(const CppRangeForBlock* p) override
  {
    return check(p);
  }
  bool visit(const CppSwitchBlock* p) override
  {
    return check(p);
  }
  bool visit(const CppTryBlock* p) override
  {
    return check(p);
  }
  bool visit(const CppTypedefList* p) override
  {
    return check(p);
  }
  bool visit(const CppTypedefName* p) override
  {
    return check(p);
  }
};
