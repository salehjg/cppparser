/*
The MIT License (MIT)

Copyright (c) 2018 Satya Das

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
                                                          the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
                                                                  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
                                                                  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
                                     CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
                                       */

#pragma once

#include "cppast.h"
#include "cppindent.h"

#include <string>

                                     //////////////////////////////////////////////////////////////////////////

                                     /**
* Responsible for emitting C/C++ source from CppAst data structure.
* Implementation of emitting various C/C++ objects should never change
* the style of code generated. Addition of new functionality and bug fixes
* are allowed but care must be taken not to change the style of emitted code.
                                      */
                                     class CppWriter
{
public:
  CppWriter();

  enum EmittingType
  {
    kRaw,    ///< No intelligence is applied and things are emitted as is.
    kHeader, ///< No function definition is emitted unless it is explicitly inline
    kSource  ///< No method declaration is emitted unless it is part of the class that is defined in the same file.
  };
  void         setEmittingType(EmittingType type);
  EmittingType getEmittingType() const;

public:
  // This only delegate to other emit() method based on type.
  virtual void emit(CppObj* cppObj, std::ostream& stm, CppIndent indentation = CppIndent()) const;
  virtual void emitVar(CppVar* varObj, std::ostream& stm, CppIndent indentation = CppIndent()) const;
  virtual void emitVarList(CppVarList* varListObj, std::ostream& stm, CppIndent indentation = CppIndent()) const;
  virtual void emitEnum(CppEnum* enmObj,
                        std::ostream&  stm,
                        bool           emitNewLine,
                        CppIndent      indentation = CppIndent()) const;
  virtual void emitTypedef(CppTypedefName* typedefName,
                           std::ostream&         stm,
                           CppIndent             indentation = CppIndent()) const;
  virtual void emitUsingDecl(CppUsingDecl* usingDecl,
                             std::ostream&       stm,
                             CppIndent           indentation = CppIndent()) const;
  virtual void emitTypedefList(CppTypedefList* typedefList,
                               std::ostream&         stm,
                               CppIndent             indentation = CppIndent()) const;
  virtual void emitFwdDecl(CppFwdClsDecl* fwdClsDeclObj,
                           std::ostream&        stm,
                           CppIndent            indentation = CppIndent()) const;
  virtual void emitMacroCall(CppMacroCall* macroCallObj,
                             std::ostream&       stm,
                             CppIndent           indentation = CppIndent()) const;
  virtual void emitCompound(CppCompound* compoundObj,
                            std::ostream&      stm,
                            CppIndent          indentation = CppIndent(),
                            bool               emitNewLine = true) const;
  virtual void emitFunction(CppFunction* funcObj,
                            std::ostream&      stm,
                            bool               emitNewLine,
                            CppIndent          indentation = CppIndent()) const;
  virtual void emitFunctionPtr(CppFunctionPointer* funcPtrObj,
                               std::ostream&             stm,
                               bool                      emitNewLine,
                               CppIndent                 indentation = CppIndent()) const;
  virtual void emitConstructor(CppConstructor* ctorObj,
                               std::ostream&         stm,
                               CppIndent             indentation = CppIndent()) const;
  virtual void emitDestructor(CppDestructor* dtorObj,
                              std::ostream&        stm,
                              CppIndent            indentation = CppIndent()) const;
  virtual void emitTypeConverter(CppTypeConverter* typeConverterObj,
                                 std::ostream&           stm,
                                 CppIndent               indentation = CppIndent()) const;
  virtual void emitDocComment(CppDocComment* docCommentObj,
                              std::ostream&        stm,
                              CppIndent            indentation = CppIndent()) const;

  virtual void emitExpr(CppExpr* exprObj, std::ostream& stm, CppIndent indentation = CppIndent()) const;
  virtual void emitDefine(CppDefine* defineObj, std::ostream& stm) const;
  virtual void emitUndef(CppUndef* undefObj, std::ostream& stm) const;
  virtual void emitInclude(CppInclude* includeObj, std::ostream& stm) const;
  virtual void emitHashIf(CppHashIf* hashIfObj, std::ostream& stm) const;
  virtual void emitHashIf(CppHashIf::CondType condType, const std::string& cond, std::ostream& stm) const;
  virtual void emitEndIf(std::ostream& stm) const;
  virtual void emitPragma(CppPragma* pragmaObj, std::ostream& stm) const;
  virtual void emitBlob(CppBlob* blobObj, std::ostream& stm, bool formatLineStarts, CppIndent indentation) const;

  virtual void emitVarType(CppVarType* varTypeObj, std::ostream& stm) const;
  virtual void emitParamList(CppParamVector* paramListObj, std::ostream& stm) const;
  virtual void emitExprAtom(CppExprAtom& exprObj, std::ostream& stm, CppIndent indentation = CppIndent()) const;

  virtual void emitIfBlock(CppIfBlock* ifBlock, std::ostream& stm, CppIndent indentation = CppIndent()) const;
  virtual void emitWhileBlock(CppWhileBlock* whileBlock,
                              std::ostream&        stm,
                              CppIndent            indentation = CppIndent()) const;
  virtual void emitDoBlock(CppDoWhileBlock* doBlock,
                           std::ostream&          stm,
                           CppIndent              indentation = CppIndent()) const;
  virtual void emitForBlock(CppForBlock* forBlock, std::ostream& stm, CppIndent indentation = CppIndent()) const;
  virtual void emitSwitchBlock(CppSwitchBlock* switchBlock,
                               std::ostream&         stm,
                               CppIndent             indentation = CppIndent()) const;

public:
  void emitVar(CppVar* varObj, std::ostream& stm, bool skipName) const;
  void emitParamList(CppParamVector* paramListObj, std::ostream& stm, bool skipName) const;
  void emitFunctionPtr(CppFunctionPointer* funcPtrObj, std::ostream& stm, bool skipName, bool emitNewLine) const;
  void emitFunction(CppFunction* funcObj, std::ostream& stm, bool skipParamName, bool emitNewLine) const;
  void emitConstructor(CppConstructor* ctorObj, std::ostream& stm, bool skipParamName) const;
  void emitTemplSpec(CppTemplateParamList* templSpec, std::ostream& stm, CppIndent indentation) const;

private:
  void emit(CppObj* cppObj, std::ostream& stm, CppIndent indentation, bool noNewLine) const;
  void emitVar(CppVar* varObj, std::ostream& stm, CppIndent indentation, bool skipName) const;
  void emitFunctionPtr(CppFunctionPointer* funcPtrObj,
                       std::ostream&             stm,
                       CppIndent                 indentation,
                       bool                      skipName,
                       bool                      emitNewLine) const;
  void emitFunction(CppFunction* funcObj,
                    std::ostream&      stm,
                    CppIndent          indentation,
                    bool               skipName,
                    bool               skipParamName,
                    bool               emitNewLine) const;
  void emitConstructor(CppConstructor* ctorObj,
                       std::ostream&         stm,
                       CppIndent             indentation,
                       bool                  skipParamName) const;

  void emitVarDecl(std::ostream& stm, CppVarDecl& varDecl, bool skipName) const;

private:
  mutable CppIndent preproIndent_;

  EmittingType emittingType_;
};

inline CppWriter::CppWriter()
  : emittingType_(kRaw)
{
}

inline void CppWriter::setEmittingType(EmittingType emittingType)
{
  emittingType_ = emittingType;
}

inline CppWriter::EmittingType CppWriter::getEmittingType() const
{
  return emittingType_;
}

inline void CppWriter::emitVar(CppVar* varObj, std::ostream& stm, bool skipName) const
{
  emitVar(varObj, stm, CppIndent(), skipName);
}

inline void CppWriter::emitFunctionPtr(CppFunctionPointer* funcPtrObj,
                                       std::ostream&             stm,
                                       bool                      skipName,
                                       bool                      emitNewLine) const
{
  if (funcPtrObj->attr() & kTypedef)
    return; // Typedef of funcptr without name makes no sense.
  emitFunction(funcPtrObj, stm, CppIndent(), skipName, skipName, emitNewLine);
}

inline void CppWriter::emitFunction(CppFunction* funcObj,
                                    std::ostream&      stm,
                                    bool               skipParamName,
                                    bool               emitNewLine) const
{
  emitFunction(funcObj, stm, CppIndent(), false, skipParamName, emitNewLine);
}

inline void CppWriter::emitConstructor(CppConstructor* ctorObj, std::ostream& stm, bool skipParamName) const
{
  emitConstructor(ctorObj, stm, CppIndent(), skipParamName);
}

inline std::ostream& operator<<(std::ostream& stm, CppIndent& indentation)
{
  indentation.emit(stm);
  return stm;
}

inline std::ostream& operator<<(std::ostream& stm, CppAccessType accessType)
{
  switch (accessType)
  {
    case CppAccessType::kPublic:
      stm << "public";
      break;
    case CppAccessType::kProtected:
      stm << "protected";
      break;
    case CppAccessType::kPrivate:
      stm << "private";
      break;

    default:
      break;
  }
  return stm;
}

inline std::ostream& operator<<(std::ostream& stm, CppCompoundType cmpndType)
{
  switch (cmpndType)
  {
    case CppCompoundType::kNamespace:
      stm << "namespace";
      break;
    case CppCompoundType::kClass:
      stm << "class";
      break;
    case CppCompoundType::kStruct:
      stm << "struct";
      break;
    case CppCompoundType::kUnion:
      stm << "union";
      break;

    default:
      break;
  }
  return stm;
}
