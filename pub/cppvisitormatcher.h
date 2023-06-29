//
// Created by saleh on 6/25/23.
//

#pragma once

/**
 * @file A visitor class to match only the given types of nodes.
 * Inherit from this class and override `matched` method to implement your own custom AST-matcher.
 */

#include <algorithm>
#include <vector>

#include "cppast.h"
#include "cppvisitorbase.h"
#include "cppvisitorprinter.h"

class CppVisitorMatcher : public CppVisitorBase
{
  const std::vector<CppObjType> typesToMatch;

public:
  /**
   * @brief The only ctor of the class.
   * @param typesToMatch A vector of the AST node types that are to be matched.
   */
  explicit CppVisitorMatcher(const std::vector<CppObjType>& typesToMatch)
    : typesToMatch(typesToMatch)
  {
  }

  /**
   * @brief The virtual method to be called when a tree node is matched.
   * Override this method to implement your very own AST-matcher.
   * @param p A pointer to the matched AST node.
   * @return Return true if you want continue traversing the tree, false otherwise.
   */
  virtual bool matched(CppObj* p)
  {
    std::cout << CppVisitorPrinter::astToString(p) << std::endl;
    return true;
  }

  ~CppVisitorMatcher() override = default;

private:
  /**
   * @brief Checks the given pointer to the current AST node against the node types that are to be matched.
   * Calls the `match` method when a node is matched and returns the value returned by that method to enforce user's
   * choice for an early abortion of traversing the rest of the AST.
   * @param p A pointer to an AST node.
   * @return true if the node did not match, otherwise, returns the value returned by CppVisitorMatcher::match.
   */
  inline bool check(CppObj* p)
  {
    if (std::find(typesToMatch.begin(), typesToMatch.end(), p->objType_) != typesToMatch.end())
    {
      return matched(p); // continue traversing the tree only if the user wants to.
    }
    return true; // continue traversing the tree to find a possible match.
  }

protected:
  bool visit(CppVarType* p) override
  {
    return check(p);
  }
  bool visit(CppCompound* p) override
  {
    return check(p);
  }
  bool visit(CppUnRecogPrePro* p) override
  {
    return check(p);
  }
  bool visit(CppUndef* p) override
  {
    return check(p);
  }
  bool visit(CppUsingDecl* p) override
  {
    return check(p);
  }
  bool visit(CppUsingNamespaceDecl* p) override
  {
    return check(p);
  }
  bool visit(CppVar* p) override
  {
    return check(p);
  }
  bool visit(CppVarList* p) override
  {
    return check(p);
  }
  bool visit(CppCommonBlock<CppObjType::kIfBlock>* p) override
  {
    return check(p);
  }
  bool visit(CppCommonBlock<CppObjType::kWhileBlock>* p) override
  {
    return check(p);
  }
  bool visit(CppCommonBlock<CppObjType::kDoWhileBlock>* p) override
  {
    return check(p);
  }
  bool visit(CppAsmBlock* p) override
  {
    return check(p);
  }
  bool visit(CppBlob* p) override
  {
    return check(p);
  }
  bool visit(CppDefine* p) override
  {
    return check(p);
  }
  bool visit(CppDocComment* p) override
  {
    return check(p);
  }
  bool visit(CppEnum* p) override
  {
    return check(p);
  }
  bool visit(CppExpr* p) override
  {
    return check(p);
  }
  bool visit(CppForBlock* p) override
  {
    return check(p);
  }
  bool visit(CppLambda* p) override
  {
    return check(p);
  }
  bool visit(CppDestructor* p) override
  {
    return check(p);
  }
  bool visit(CppConstructor* p) override
  {
    return check(p);
  }
  bool visit(CppFunction* p) override
  {
    return check(p);
  }
  bool visit(CppFunctionPointer* p) override
  {
    return check(p);
  }
  bool visit(CppTypeConverter* p) override
  {
    return check(p);
  }
  bool visit(CppFwdClsDecl* p) override
  {
    return check(p);
  }
  bool visit(CppHashError* p) override
  {
    return check(p);
  }
  bool visit(CppHashIf* p) override
  {
    return check(p);
  }
  bool visit(CppHashWarning* p) override
  {
    return check(p);
  }
  bool visit(CppImport* p) override
  {
    return check(p);
  }
  bool visit(CppInclude* p) override
  {
    return check(p);
  }
  bool visit(CppLabel* p) override
  {
    return check(p);
  }
  bool visit(CppMacroCall* p) override
  {
    return check(p);
  }
  bool visit(CppNamespaceAlias* p) override
  {
    return check(p);
  }
  bool visit(CppPragma* p) override
  {
    return check(p);
  }
  bool visit(CppRangeForBlock* p) override
  {
    return check(p);
  }
  bool visit(CppSwitchBlock* p) override
  {
    return check(p);
  }
  bool visit(CppTryBlock* p) override
  {
    return check(p);
  }
  bool visit(CppTypedefList* p) override
  {
    return check(p);
  }
  bool visit(CppTypedefName* p) override
  {
    return check(p);
  }
};
