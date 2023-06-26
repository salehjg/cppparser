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
  virtual bool matched(const CppObj* p)
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
