//
// Created by saleh on 6/22/23.
//

#pragma once

/**
 * @file An abstract base class for creating a visitor of the AST nodes.
 * Inherit from this class and override the `visit` methods.
 */

#include "cppast.h"
#include "cppconst.h"

struct CppVarType;
struct CppCompound;
struct CppUnRecogPrePro;
struct CppUndef;
struct CppUsingDecl;
struct CppUsingNamespaceDecl;
struct CppVar;
struct CppVarList;
template <CppObjType _ObjType> struct CppCommonBlock;
struct CppAsmBlock;
struct CppBlob;
struct CppDefine;
struct CppDocComment;
struct CppEnum;
struct CppExpr;
struct CppForBlock;
struct CppLambda;
struct CppDestructor;
struct CppConstructor;
struct CppFunction;
struct CppFunctionPointer;
struct CppTypeConverter;
struct CppFwdClsDecl;
struct CppHashError;
struct CppHashIf;
struct CppHashWarning;
struct CppImport;
struct CppInclude;
struct CppLabel;
struct CppMacroCall;
struct CppNamespaceAlias;
struct CppPragma;
struct CppRangeForBlock;
struct CppSwitchBlock;
struct CppTryBlock;
struct CppTypedefList;
struct CppTypedefName;

class CppVisitorBase
{
public:
  virtual ~CppVisitorBase() = default;

  /**
   * @brief The visitor pattern's `visit` methods for the given types.
   * @param p A pointer to the matched AST node.
   * @return Return true if you want continue traversing the tree, false otherwise.
   */
  virtual bool visit(const CppVarType *p) = 0;
  virtual bool visit(const CppCompound *p) = 0;
  virtual bool visit(const CppUnRecogPrePro *p) = 0;
  virtual bool visit(const CppUndef *p) = 0;
  virtual bool visit(const CppUsingDecl *p) = 0;
  virtual bool visit(const CppUsingNamespaceDecl *p) = 0;
  virtual bool visit(const CppVar *p) = 0;
  virtual bool visit(const CppVarList *p) = 0;
  virtual bool visit(const CppCommonBlock<CppObjType::kIfBlock> *p) = 0;
  virtual bool visit(const CppCommonBlock<CppObjType::kWhileBlock> *p) = 0;
  virtual bool visit(const CppCommonBlock<CppObjType::kDoWhileBlock> *p) = 0;
  virtual bool visit(const CppAsmBlock *p) = 0;
  virtual bool visit(const CppBlob *p) = 0;
  virtual bool visit(const CppDefine *p) = 0;
  virtual bool visit(const CppDocComment *p) = 0;
  virtual bool visit(const CppEnum *p) = 0;
  virtual bool visit(const CppExpr *p) = 0;
  virtual bool visit(const CppForBlock *p) = 0;
  virtual bool visit(const CppLambda *p) = 0;
  virtual bool visit(const CppDestructor *p) = 0;
  virtual bool visit(const CppConstructor *p) = 0;
  virtual bool visit(const CppFunction *p) = 0;
  virtual bool visit(const CppFunctionPointer *p) = 0;
  virtual bool visit(const CppTypeConverter *p) = 0;
  virtual bool visit(const CppFwdClsDecl *p) = 0;
  virtual bool visit(const CppHashError *p) = 0;
  virtual bool visit(const CppHashIf *p) = 0;
  virtual bool visit(const CppHashWarning *p) = 0;
  virtual bool visit(const CppImport *p) = 0;
  virtual bool visit(const CppInclude *p) = 0;
  virtual bool visit(const CppLabel *p) = 0;
  virtual bool visit(const CppMacroCall *p) = 0;
  virtual bool visit(const CppNamespaceAlias *p) = 0;
  virtual bool visit(const CppPragma *p) = 0;
  virtual bool visit(const CppRangeForBlock *p) = 0;
  virtual bool visit(const CppSwitchBlock *p) = 0;
  virtual bool visit(const CppTryBlock *p) = 0;
  virtual bool visit(const CppTypedefList *p) = 0;
  virtual bool visit(const CppTypedefName *p) = 0;
};