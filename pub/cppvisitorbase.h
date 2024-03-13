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
  virtual bool visit(CppVarType *p) = 0;
  virtual bool visit(CppCompound *p) = 0;
  virtual bool visit(CppUnRecogPrePro *p) = 0;
  virtual bool visit(CppUndef *p) = 0;
  virtual bool visit(CppUsingDecl *p) = 0;
  virtual bool visit(CppUsingNamespaceDecl *p) = 0;
  virtual bool visit(CppVar *p) = 0;
  virtual bool visit(CppVarList *p) = 0;
  virtual bool visit(CppCommonBlock<CppObjType::kIfBlock> *p) = 0;
  virtual bool visit(CppCommonBlock<CppObjType::kWhileBlock> *p) = 0;
  virtual bool visit(CppCommonBlock<CppObjType::kDoWhileBlock> *p) = 0;
  virtual bool visit(CppAsmBlock *p) = 0;
  virtual bool visit(CppBlob *p) = 0;
  virtual bool visit(CppDefine *p) = 0;
  virtual bool visit(CppDocComment *p) = 0;
  virtual bool visit(CppEnum *p) = 0;
  virtual bool visit(CppExpr *p) = 0;
  virtual bool visit(CppForBlock *p) = 0;
  virtual bool visit(CppLambda *p) = 0;
  virtual bool visit(CppDestructor *p) = 0;
  virtual bool visit(CppConstructor *p) = 0;
  virtual bool visit(CppFunction *p) = 0;
  virtual bool visit(CppFunctionPointer *p) = 0;
  virtual bool visit(CppTypeConverter *p) = 0;
  virtual bool visit(CppFwdClsDecl *p) = 0;
  virtual bool visit(CppHashError *p) = 0;
  virtual bool visit(CppHashIf *p) = 0;
  virtual bool visit(CppHashWarning *p) = 0;
  virtual bool visit(CppImport *p) = 0;
  virtual bool visit(CppInclude *p) = 0;
  virtual bool visit(CppLabel *p) = 0;
  virtual bool visit(CppMacroCall *p) = 0;
  virtual bool visit(CppNamespaceAlias *p) = 0;
  virtual bool visit(CppPragma *p) = 0;
  virtual bool visit(CppRangeForBlock *p) = 0;
  virtual bool visit(CppSwitchBlock *p) = 0;
  virtual bool visit(CppTryBlock *p) = 0;
  virtual bool visit(CppTypedefList *p) = 0;
  virtual bool visit(CppTypedefName *p) = 0;
};