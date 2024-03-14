/*
The MIT License (MIT)

Copyright (c) 2018 Satya Das

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
                                                       the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
                                                               FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
                                    */

#pragma once

/**
 * @file Defines classes to represent C++ AST objects, like expression, variable definition, class definition, etc.
 * To keep things simple, followings are the design principles:
 *  - classes will have struct like syntax.
 *  - public members will be .
 *  - non- members will have accessor and mutator.
 * The idea behind this is to have immutable kind of objects but since that is not achievable in reality,
 * the mutation of object needs to be controlled through use of mutating method.
 */

/*
x : not used anywhere.
* : only used off AST header.
##: used in AST header.
--: reviewed for nullptr handling

##CppExprPtr --
##AttribSpecifier --
##AttribSpecifierSequence --
##CppCompoundPtr --
##CppObjPtr --
##CppVarTypePtr --
##CppTemplateParamListPtr --
##CppVarPtr --
##CppVarListPtr --
##CppInheritanceListPtr --
##CppCompoundPtr --
##CppFuncThrowSpecPtr --
##CppParamVectorPtr --
##CppVarTypePtr --
##CppEnumItemListPtr --
##CppSwitchBodyPtr --
##CppCatchBlockPtr --
##CppCatchBlocks --


*CppIncludeEPtr
*CppVarTypeEPtr
*CppConstVarTypeEPtr
*CppVarEPtr
*CppConstVarEPtr
*CppVarListEPtr
*CppCompoundEPtr
*CppConstCompoundEPtr
*CppVarTypeEPtr
*CppFunctionEPtr
*CppConstFunctionEPtr
*CppExprEPtr
*CppConstExprEPtr
*CppLabelEPtr


xCppDefineEPtr
xCppUndefEPtr
xCppHashIfEPtr
xCppPragmaEPtr
xCppHashErrorEPtr
xCppHashWarningEPtr
xCppUnRecogPreProEPtr
xCppTypedefNameEPtr
xCppTypedefListEPtr
xCppMacroCallEPtr
xCppFwdClsDeclEPtr
xCppFunctionPointerEPtr
xCppConstructorEPtr
xCppDestructorEPtr
xCppTypeConverterEPtr
xCppUsingNamespaceDeclEPtr
xCppUsingDeclEPtr
xCppConstUsingDeclEPtr
xCppNamespaceAliasEPtr
xCppDocCommentEPtr
xCppEnumEPtr
xCppIfBlockEPtr
xCppWhileBlockEPtr
xCppDoWhileBlockEPtr
xCppForBlockEPtr
xCppSwitchBlockEPtr

*/

#include "cppconst.h"
#include "cppeasyptr.h"
#include "cppvisitorbase.h"
#include "typemodifier.h"

#include "string-utils.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

//////////////////////////////////////////////////////////////////////////
#define VISIT_COND(CALL)                                                                                               \
  if (!CALL)                                                                                                           \
    return;

struct CppCompound;

/**
 * An abstract class that is used as base class of all other classes.
 */
struct CppObj
{
  CppObjType    objType_;
  CppAccessType accessType_; ///< All objects do not need this.

  CppObj(CppObjType type, CppAccessType accessType)
    : objType_(type)
    , accessType_(accessType)
    , owner_(nullptr)
  {
  }

  CppCompound* owner()
  {
    return owner_;
  }
  void owner(CppCompound* o)
  {
    //assert(owner_ == nullptr);  ///TODO: Mem leak
    owner_ = o;
  }

  virtual void accept(CppVisitorBase* v) = 0;

  virtual ~CppObj() { }

private:
  CppCompound* owner_;
};

/**
 * \brief A stream of text that represents some content in a C++ program.
 */
struct CppBlob : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kBlob;
  std::string                 blob_;

  CppBlob(std::string blob)
    : CppObj(CppObjType::kBlob, CppAccessType::kUnknown)
    , blob_(std::move(trimBlob(blob)))
  {
  }

  void accept(CppVisitorBase* v) override { }
};

struct CppDefine : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashDefine;

  enum DefType
  {
    kRename,
    kConstNumDef,
    kConstStrDef,
    kConstCharDef,
    kComplexMacro,
  };
  DefType     defType_;
  std::string name_;
  std::string defn_; ///< This will contain everything after name.

  CppDefine(DefType defType, std::string name, std::string defn = std::string())
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , defType_(defType)
    , name_(std::move(name))
    , defn_(std::move(defn))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppDefineEPtr = CppEasyPtr<CppDefine>;

struct CppUndef : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashUndef;

  std::string name_;

  CppUndef(std::string name)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , name_(std::move(name))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppUndefEPtr = CppEasyPtr<CppUndef>;

struct CppInclude : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashInclude;

  std::string name_;

  CppInclude(std::string name)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , name_(std::move(name))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

struct CppImport : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashImport;

  std::string name_;

  CppImport(std::string name)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , name_(std::move(name))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppIncludeEPtr = CppEasyPtr<CppInclude>;

/**
 * \brief Represents all variants of #if preprocessors.
 * It includes #if, #ifdef, #ifndef.
 * It also includes #else, #elif, and #endif.
 */
struct CppHashIf : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashIf;

  enum CondType
  {
    kIf,
    kIfDef,
    kIfNDef,
    kElse,
    kElIf,
    kEndIf
  };

  CondType    condType_;
  std::string cond_;

  CppHashIf(CondType condType, std::string cond = std::string())
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , condType_(condType)
    , cond_(std::move(cond))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppHashIfEPtr = CppEasyPtr<CppHashIf>;

struct CppPragma : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashPragma;

  std::string defn_;

  CppPragma(std::string defn)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , defn_(std::move(defn))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppPragmaEPtr = CppEasyPtr<CppPragma>;

struct CppHashError : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashError;

  std::string err_;

  CppHashError(std::string err)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , err_(std::move(err))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppHashErrorEPtr = CppEasyPtr<CppHashError>;

struct CppHashWarning : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kHashWarning;

  std::string err_;

  CppHashWarning(std::string err)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , err_(std::move(err))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppHashWarningEPtr = CppEasyPtr<CppHashWarning>;

/**
 * Any other C/C++ preprocessor for which there is no class defined.
 */
struct CppUnRecogPrePro : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kUnRecogPrePro;

  std::string name_;
  std::string defn_;

  CppUnRecogPrePro(std::string name, std::string defn)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , name_(std::move(name))
    , defn_(std::move(defn))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

struct CppExpr;
using CppExprPtr              = std::unique_ptr<CppExpr>;
using AttribSpecifier         = CppExprPtr;
using AttribSpecifierArray    = std::vector<AttribSpecifier>;
using AttribSpecifierSequence = std::unique_ptr<AttribSpecifierArray>;

/**
 * @brief A mixin class to allow objects to have attribute specifier sequence
 * as described at https://en.cppreference.com/w/cpp/language/attributes.
 */
struct AttribSpecified
{
  AttribSpecifierSequence& attribSpecifierSequence()
  {
    return attributeSpecifiers_;
  }
  void attribSpecifierSequence(AttribSpecifierArray* attribSpecifierArray)
  {
    attributeSpecifiers_ = AttribSpecifierSequence(attribSpecifierArray);
  }

private:
  AttribSpecifierSequence attributeSpecifiers_; // C++11 Attribute specifier sequence
};

using CppUnRecogPreProEPtr = CppEasyPtr<CppUnRecogPrePro>;
using CppCompoundPtr       = std::unique_ptr<CppCompound>;
using CppObjPtr            = std::unique_ptr<CppObj>;

struct CppFunctionPointer;
struct CppEnum;

struct CppVarType : public CppObj, public AttribSpecified
{
  static constexpr CppObjType kObjectType = CppObjType::kVarType;

  bool paramPack_ {false};

  CppVarType(std::string baseType, CppTypeModifier modifier = CppTypeModifier());
  CppVarType(CppAccessType accessType, std::string baseType, CppTypeModifier modifier);
  CppVarType(CppAccessType accessType, CppCompound* compound, CppTypeModifier modifier);
  CppVarType(CppAccessType accessType, CppFunctionPointer* fptr, CppTypeModifier modifier);
  CppVarType(CppAccessType accessType, CppEnum* enumObj, CppTypeModifier modifier);
  CppVarType(CppVarType& varType);

  std::string& baseType()
  {
    return baseType_;
  }
  void baseType(std::string _baseType)
  {
    baseType_ = std::move(_baseType);
  }
  CppObj* compound()
  {
    return compound_.get();
  }
  std::uint32_t typeAttr()
  {
    return typeAttr_;
  }
  void typeAttr(std::uint32_t attr)
  {
    typeAttr_ = attr;
  }
  void addAttr(std::uint32_t attr)
  {
    if ((attr & CppIdentifierAttrib::kConst) == 0)
      typeAttr_ |= attr;
    else
      typeModifier_.constBits_ |= 1;
  }

  CppTypeModifier& typeModifier()
  {
    return typeModifier_;
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (compound_ != nullptr)
      compound_->accept(v);
  }

private:
  CppVarType(CppAccessType accessType, std::string baseType, std::uint32_t typeAttr, CppTypeModifier modifier)
    : CppObj(kObjectType, accessType)
    , baseType_(cleanseIdentifier(baseType))
    , typeModifier_(modifier)
    , typeAttr_(typeAttr)
  {
  }

private:
  std::string     baseType_; // This is the basic data type of var e.g. for ' int*& pi' base-type is int.
  CppObjPtr       compound_;
  CppTypeModifier typeModifier_;
  std::uint32_t   typeAttr_ {0}; // Attribute associated with type, e.g. static, extern, extern "C", , volatile.
};

using CppVarTypeEPtr = CppEasyPtr<CppVarType>;
using CppArraySizes  = std::vector<CppExprPtr>;

struct CppVarDecl
{
  CppVarDecl(std::string name)
    : name_(std::move(name))
  {
  }

  CppVarDecl(std::string name, CppExpr* assign, AssignType assignType = AssignType::kNone);

  std::string& name()
  {
    return name_;
  }
  void name(std::string _name)
  {
    name_ = std::move(_name);
  }

  CppExpr* assignValue()
  {
    return assignValue_.get();
  }
  AssignType assignType()
  {
    return assignType_;
  }
  void assign(CppExpr* assignVal, AssignType assignType)
  {
    assert(assignType_ == AssignType::kNone);
    assignValue_.reset(assignVal);
    assignType_ = assignType;
  }

  CppExpr* bitField()
  {
    return bitField_.get();
  }
  void bitField(CppExpr* _bitField)
  {
    bitField_.reset(_bitField);
  }

  CppArraySizes& arraySizes()
  {
    return arraySizes_;
  }
  void addArraySize(CppExpr* arraySize)
  {
    arraySizes_.emplace_back(arraySize);
  }

private:
  std::string   name_;
  CppExprPtr    assignValue_; // Value assigned at declaration.
  AssignType    assignType_ {AssignType::kNone};
  CppExprPtr    bitField_;
  CppArraySizes arraySizes_;
};

using CppVarTypePtr = std::unique_ptr<CppVarType>;

struct CppTemplateParam;
using CppTemplateParamList    = std::vector<std::unique_ptr<CppTemplateParam>>;
using CppTemplateParamListPtr = std::unique_ptr<CppTemplateParamList>;

/**
 * Class to represent C++ variable definition.
 * A variable can be global, local or member of a struct, class, namespace, or union.
 * It can also be a function parameter.
 */
struct CppVar : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kVar;

  CppVar(CppVarType* varType, CppVarDecl varDecl)
    : CppObj(kObjectType, varType->accessType_)
    , varType_(varType)
    , varDecl_(std::move(varDecl))
  {
  }

  CppVar(CppVarTypePtr varType, CppVarDecl varDecl)
    : CppObj(kObjectType, varType->accessType_)
    , varType_(std::move(varType))
    , varDecl_(std::move(varDecl))
  {
  }

  CppVar(CppAccessType accessType, CppFunctionPointer* fptr, CppTypeModifier modifier)
    : CppObj(kObjectType, accessType)
    , varType_(new CppVarType(accessType, fptr, modifier))
    , varDecl_(std::string())
  {
  }

  CppVarType* varType()
  {
    return varType_.get();
  }
  void varType(CppVarTypePtr _varType)
  {
    varType_ = std::move(_varType);
  }

  std::string& name()
  {
    return varDecl_.name();
  }

  std::uint32_t typeAttr()
  {
    return varType_->typeAttr();
  }
  void addAttr(std::uint32_t attr)
  {
    varType_->addAttr(attr);
  }

  CppVarDecl& varDecl()
  {
    return varDecl_;
  }

  CppExpr* assignValue()
  {
    return varDecl_.assignValue();
  }
  AssignType assignType()
  {
    return varDecl_.assignType();
  }
  void assign(CppExpr* assignVal, AssignType assignType)
  {
    varDecl_.assign(assignVal, assignType);
  }

  CppExpr* bitField()
  {
    return varDecl_.bitField();
  }
  void bitField(CppExpr* _bitField)
  {
    varDecl_.bitField(_bitField);
  }

  CppArraySizes& arraySizes()
  {
    return varDecl_.arraySizes();
  }
  void addArraySize(CppExpr* arraySize)
  {
    varDecl_.addArraySize(arraySize);
  }

  std::string& apidecor()
  {
    return apidecor_;
  }
  void apidecor(std::string _apidecor)
  {
    apidecor_ = std::move(_apidecor);
  }

  CppTemplateParamList* templateParamList()
  {
    return templSpec_.get();
  }
  void templateParamList(CppTemplateParamList* templParamList)
  {
    templSpec_.reset(templParamList);
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (varType_ != nullptr)
      VISIT_COND(v->visit(varType_.get()));
    // `CppVarDecl` is not an AST node, it is a member of `CppVar`.
  }

private:
  CppVarTypePtr           varType_;
  CppVarDecl              varDecl_;
  std::string             apidecor_; // It holds things like WINAPI, __declspec(dllexport), etc.
  CppTemplateParamListPtr templSpec_;
};

using CppVarPtr  = std::unique_ptr<CppVar>;
using CppVarEPtr = CppEasyPtr<CppVar>;

struct CppVarDeclInList : public CppTypeModifier, public CppVarDecl
{
  CppVarDeclInList(CppTypeModifier modifier, CppVarDecl varDecl)
    : CppTypeModifier(modifier)
    , CppVarDecl(std::move(varDecl))
  {
  }
};

using CppVarDeclList = std::vector<CppVarDeclInList>;

/**
 * \brief List of variables declared in a line without repeating its type, e.g. int i, j; is a var-list.
 */
struct CppVarList : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kVarList;

  CppVarPtr firstVar_;

  CppVarList(CppVar* firstVar, CppVarDeclInList varDecl)
    : CppObj(kObjectType, firstVar->accessType_)
    , firstVar_(firstVar)
  {
    addVarDecl(std::move(varDecl));
  }
  void addVarDecl(CppVarDeclInList varDecl)
  {
    varDeclList_.push_back(std::move(varDecl));
  }
  CppVarPtr& firstVar()
  {
    return firstVar_;
  }
  CppVarDeclList& varDeclList()
  {
    return varDeclList_;
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (firstVar_ != nullptr)
      VISIT_COND(v->visit(firstVar_.get()));
    // `CppVarDeclList` is not an AST node, it is a member of `CppVarList`
  }

private:
  CppVarDeclList varDeclList_;
};

struct CppTypedefName : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kTypedefName;

  CppVarPtr var_;

  CppTypedefName(CppVar* var)
    : CppObj(kObjectType, var->accessType_)
    , var_(var)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (var_ != nullptr)
      VISIT_COND(v->visit(var_.get()));
  }
};

using CppTypedefNameEPtr = CppEasyPtr<CppTypedefName>;

using CppVarListPtr  = std::unique_ptr<CppVarList>;
using CppVarListEPtr = CppEasyPtr<CppVarList>;

struct CppTypedefList : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kTypedefNameList;

  CppVarListPtr varList_;

  CppTypedefList(CppVarList* varList)
    : CppObj(kObjectType, varList->accessType_)
    , varList_(varList)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (varList_ != nullptr)
      VISIT_COND(v->visit(varList_.get()));
  }
};

using CppTypedefListEPtr = CppEasyPtr<CppTypedefList>;

struct CppMacroCall : CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kMacroCall;

  std::string macroCall_;

  CppMacroCall(std::string macroCall, CppAccessType accessType)
    : CppObj(kObjectType, accessType)
    , macroCall_(std::move(macroCall))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppMacroCallEPtr = CppEasyPtr<CppMacroCall>;

//////////////////////////////////////////////////////////////////////////

struct CppInheritInfo
{
  std::string   baseName;
  CppAccessType inhType;
  bool          isVirtual {false};

  CppInheritInfo(std::string _baseName, CppAccessType _inhType, bool virtualInheritance = false)
    : baseName(std::move(_baseName))
    , inhType(_inhType)
    , isVirtual(virtualInheritance)
  {
  }
};

struct CppFunctionPointer;
/**
 * Parameter types that are used to define a template class or function.
 */
struct CppTemplateParam
{
  // If not nullptr then template param is not of type typename/class
  std::unique_ptr<CppObj> paramType_;
  std::string             paramName_;

  CppTemplateParam(std::string paramName)
    : paramType_(nullptr)
    , paramName_(std::move(paramName))
  {
  }

  CppTemplateParam(CppVarType* paramType, std::string paramName)
    : paramType_(paramType)
    , paramName_(std::move(paramName))
  {
  }

  CppTemplateParam(CppFunctionPointer* paramType, std::string paramName);

  CppObj* defaultArg()
  {
    return defaultArg_.get();
  }

  void defaultArg(CppObj* defParam)
  {
    assert(!defaultArg_);
    defaultArg_.reset(defParam);
  }

private:
  CppObjPtr defaultArg_; //< Can be CppVarType or CppExpr
};

struct CppFwdClsDecl : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kFwdClsDecl;

  CppCompoundType cmpType_;
  std::string     name_;
  std::string     apidecor_;

  CppFwdClsDecl(CppAccessType   accessType,
                std::string     name,
                std::string     apidecor,
                CppCompoundType cmpType = CppCompoundType::kNoCompound)
    : CppObj(kObjectType, accessType)
    , cmpType_(cmpType)
    , name_(std::move(name))
    , apidecor_(std::move(apidecor))
    , attr_(0)
  {
  }

  CppFwdClsDecl(CppAccessType accessType, std::string name, CppCompoundType cmpType = CppCompoundType::kNoCompound)
    : CppFwdClsDecl(accessType, name, std::string(), cmpType)
  {
  }

  std::uint32_t attr()
  {
    return attr_;
  }
  void addAttr(std::uint32_t _attr)
  {
    attr_ |= _attr;
  }

  CppTemplateParamList* templateParamList()
  {
    return templSpec_.get();
  }
  void templateParamList(CppTemplateParamList* templParamList)
  {
    templSpec_.reset(templParamList);
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }

private:
  CppTemplateParamListPtr templSpec_;
  std::uint32_t           attr_ {0};
};

using CppFwdClsDeclEPtr = CppEasyPtr<CppFwdClsDecl>;

using CppInheritanceList    = std::list<CppInheritInfo>;
using CppInheritanceListPtr = std::unique_ptr<CppInheritanceList>;
using CppObjPtrArray        = std::vector<std::unique_ptr<CppObj>>;

struct CppConstructor;
struct CppDestructor;

/**
 * All classes, structs, unions, and namespaces can be classified as a Compound object.
 * Besides that followings too are compound objects:
 *  - An entire C/C++ source file.
 *  - A block of statements inside { }.
 *  - Body of function definition.
 */
struct CppCompound : public CppObj, public AttribSpecified
{
  static constexpr CppObjType kObjectType = CppObjType::kCompound;

  CppCompound(std::string name, CppAccessType accessType, CppCompoundType type)
    : CppObj(kObjectType, accessType)
    , name_(std::move(name))
    , compoundType_(type)
    , inheritanceList_(nullptr)
  {
  }

  CppCompound(CppAccessType accessType, CppCompoundType type = CppCompoundType::kUnknownCompound)
    : CppCompound(std::string(), accessType, type)
  {
  }

  CppCompound(std::string name, CppCompoundType type)
    : CppCompound(name, CppAccessType::kUnknown, type)
  {
  }

  CppCompound(CppCompoundType type)
    : CppCompound(std::string(), type)
  {
  }

  std::string& name()
  {
    return name_;
  }
  void name(std::string _name)
  {
    name_ = std::move(_name);
  }
  std::string justName()
  {
    auto itr = name_.rfind(':');
    if (itr == name_.npos)
      return name();
    return name().substr(itr + 1);
  }

  CppCompoundType compoundType()
  {
    return compoundType_;
  }
  void compoundType(CppCompoundType compoundType)
  {
    // Change of compound type is largely not allowed.
    // Although while parsing 'extern "C"' the inner part may be first parsed as block
    // which gets promoted as extern-c-block.
    assert((compoundType_ == CppCompoundType::kUnknownCompound) || (compoundType_ == CppCompoundType::kBlock));
    compoundType_ = compoundType;
  }

  CppObjPtrArray& members()
  {
    return members_;
  }

  std::string& apidecor()
  {
    return apidecor_;
  }
  void apidecor(std::string apidecor)
  {
    apidecor_ = std::move(apidecor);
  }

  CppTemplateParamList* templateParamList()
  {
    return templSpec_.get();
  }
  void templateParamList(CppTemplateParamListPtr _templateParamList)
  {
    templSpec_ = std::move(_templateParamList);
  }
  void templateParamList(CppTemplateParamList* _templateParamList)
  {
    templSpec_.reset(_templateParamList);
  }

  CppInheritanceListPtr& inheritanceList()
  {
    return inheritanceList_;
  }
  void inheritanceList(CppInheritanceListPtr _inheritanceList)
  {
    inheritanceList_ = std::move(_inheritanceList);
  }
  void inheritanceList(CppInheritanceList* _inheritanceList)
  {
    inheritanceList_.reset(_inheritanceList);
  }

  void addMember(CppObj* mem)
  {
    mem->owner(this);
    members_.emplace_back(mem);
    assignSpecialMember(mem);
  }
  void addMember(CppObjPtr &&mem)
  {
    mem->owner(this);
    members_.push_back(std::move(mem));
    assignSpecialMember(members_.back().get());
  }
  void addMemberAtFront(CppObj* mem)
  {
    mem->owner(this);
    members_.emplace(members_.begin(), mem);
    assignSpecialMember(mem);
  }

  CppObjPtr deassocMemberAt(size_t idx)
  {
    assert(idx < members_.size());
    auto ret = std::move(members_[idx]);
    members_.erase(members_.begin() + idx);

    return ret;
  }

  void addBaseClass(std::string baseName, CppAccessType inheritType)
  {
    if (inheritanceList_ == nullptr)
      inheritanceList_.reset(new CppInheritanceList);
    inheritanceList_->emplace_back(baseName, inheritType);
  }

  bool            hasPublicVirtualMethod();
  bool            hasPureVirtual();
  CppConstructor* copyCtor()
  {
    return copyCtor_;
  }
  CppConstructor* moveCtor()
  {
    return moveCtor_;
  }
  std::vector<CppConstructor*>& ctors()
  {
    return ctors_;
  }
  CppDestructor* dtor()
  {
    return dtor_;
  }
  bool triviallyConstructable();

  void addAttr(std::uint32_t _attr)
  {
    attr_ |= _attr;
  }

  bool hasAttr(std::uint32_t _attr)
  {
    return (attr_ & _attr) == _attr;
  }

  bool hasASingleBlobMember()
  {
    return (members_.size() == 1) && (members_.front()->objType_ == CppBlob::kObjectType);
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    for (auto& c : ctors_)
    {
      VISIT_COND(v->visit(c));
    }
    if (copyCtor_ != nullptr)
    {
      VISIT_COND(v->visit(copyCtor_));
    }
    if (copyCtor_ != nullptr)
    {
      VISIT_COND(v->visit(moveCtor_));
    }
    if (copyCtor_ != nullptr)
    {
      VISIT_COND(v->visit(dtor_));
    }
    for (auto& m : members_)
    {
      m->accept(v);
    }
  }

private:
  void assignSpecialMember(CppObj* mem);

private:
  std::string             name_;
  CppCompoundType         compoundType_;
  CppObjPtrArray          members_; // Objects arranged in sequential order from top to bottom.
  CppInheritanceListPtr   inheritanceList_;
  std::string             apidecor_;
  CppTemplateParamListPtr templSpec_;
  std::uint32_t           attr_ {0}; // e.g. final

  std::vector<CppConstructor*> ctors_;
  CppConstructor*              copyCtor_ {nullptr};
  CppConstructor*              moveCtor_ {nullptr};
  CppDestructor*               dtor_ {nullptr};

  mutable TriStateBool hasVirtual_     = TriStateBool::Unknown;
  mutable TriStateBool hasPureVirtual_ = TriStateBool::Unknown;
};

using CppCompoundEPtr = CppEasyPtr<CppCompound>;

struct CppFunctionPointer;

using CppIdentifierList   = std::vector<std::string>;
using CppFuncThrowSpec    = CppIdentifierList;
using CppCompoundPtr      = std::unique_ptr<CppCompound>;
using CppFuncThrowSpecPtr = std::unique_ptr<CppFuncThrowSpec>;

struct CppFuncLikeBase : public CppObj
{
  CppFuncThrowSpec* throwSpec()
  {
    return throwSpec_.get();
  }
  void throwSpec(CppFuncThrowSpec* _throwSpec)
  {
    throwSpec_.reset(_throwSpec);
  }

  CppCompound* defn()
  {
    return defn_.get();
  }
  void defn(CppCompound* _defn)
  {
    defn_.reset(_defn);
  }

protected:
  CppFuncLikeBase(CppObjType type, CppAccessType accessType)
    : CppObj(type, accessType)
  {
  }

private:
  CppCompoundPtr      defn_; // If it is nullptr then this object is just for declaration.
  CppFuncThrowSpecPtr throwSpec_;
};

/**
 * \brief Base class of constructor, destructor, and functions.
 */
struct CppFunctionBase : public CppFuncLikeBase
{
  std::string name_;

  std::uint32_t attr()
  {
    return attr_;
  }
  void addAttr(std::uint32_t _attr)
  {
    attr_ |= _attr;
  }
  bool hasAttr(std::uint32_t _attr)
  {
    return ((attr_ & _attr) == _attr);
  }

  std::string& decor1()
  {
    return decor1_;
  }
  void decor1(std::string _decor)
  {
    decor1_ = std::move(_decor);
  }

  std::string& decor2()
  {
    return decor2_;
  }
  void decor2(std::string _decor)
  {
    decor2_ = std::move(_decor);
  }

  CppTemplateParamList* templateParamList()
  {
    return templSpec_.get();
  }
  void templateParamList(CppTemplateParamList* templParamList)
  {
    templSpec_.reset(templParamList);
  }

protected:
  CppFunctionBase(CppObjType type, CppAccessType accessType, std::string name, std::uint32_t attr)
    : CppFuncLikeBase(type, accessType)
    , name_(std::move(name))
    , attr_(attr)
  {
  }

private:
  std::uint32_t           attr_;   // e.g.: , static, virtual, inline, constexpr, etc.
  std::string             decor1_; // e.g. __declspec(dllexport)
  std::string             decor2_; // e.g. __stdcall
  CppTemplateParamListPtr templSpec_;
};

using CppParamVector    = std::vector<CppObjPtr>;
using CppParamVectorPtr = std::unique_ptr<CppParamVector>;

struct CppFuncCtorBase : public CppFunctionBase
{
  bool hasParams()
  {
    return params_ && !params_->empty();
  }

  CppParamVector* params()
  {
    return params_.get();
  }

protected:
  CppFuncCtorBase(CppObjType      type,
                  CppAccessType   accessType,
                  std::string     name,
                  CppParamVector* params,
                  std::uint32_t   attr)
    : CppFunctionBase(type, accessType, std::move(name), attr)
    , params_(params)
  {
  }

protected:
  CppParamVectorPtr params_;
};

using CppVarTypePtr  = std::unique_ptr<CppVarType>;
using CppVarTypeEPtr = CppEasyPtr<CppVarType>;

struct CppFunction : public CppFuncCtorBase
{
  static constexpr CppObjType kObjectType = CppObjType::kFunction;

  CppVarTypePtr retType_;

  CppFunction(CppAccessType   accessType,
              std::string     name,
              CppVarType*     retType,
              CppParamVector* params,
              std::uint32_t   attr)
    : CppFuncCtorBase(kObjectType, accessType, std::move(name), params, attr)
    , retType_(retType)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (retType_ != nullptr)
      VISIT_COND(v->visit(retType_.get()));
    if (params_ != nullptr)
    {
      for (auto& p : *params_)
      {
        p->accept(v);
      }
    }
    defn()->accept(v);
  }

protected:
  CppFunction(CppObjType      type,
              CppAccessType   accessType,
              std::string     name,
              CppVarType*     retType,
              CppParamVector* params,
              std::uint32_t   attr)
    : CppFuncCtorBase(type, accessType, std::move(name), params, attr)
    , retType_(retType)
  {
  }
};

struct CppLambda : public CppFuncLikeBase
{
  static constexpr CppObjType kObjectType = CppObjType::kLambda;

  CppExprPtr        captures_;
  CppParamVectorPtr params_;
  CppVarTypePtr     retType_;
  CppCompoundPtr    defn_;

  CppLambda(CppExpr* captures, CppParamVector* params, CppCompound* defn, CppVarType* retType = nullptr)
    : CppFuncLikeBase(kObjectType, CppAccessType::kUnknown)
    , captures_(captures)
    , params_(params)
    , retType_(retType)
    , defn_(defn)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (captures_ != nullptr)
      VISIT_COND(v->visit(captures_.get()));
    if (params_ != nullptr)
    {
      for (auto& p : *params_)
      {
        p->accept(v);
      }
    }
    if (retType_ != nullptr)
      VISIT_COND(v->visit(retType_.get()));
    if (defn_ != nullptr)
      defn_->accept(v);
  }
};

using CppFunctionEPtr = CppEasyPtr<CppFunction>;

/**
* Function pointer type definition using typedef, e.g. 'typedef void (*funcPtr)(int);'

* It has all the attributes of a function object and so it is simply derived from CppFunction.
*/
struct CppFunctionPointer : public CppFunction
{
  static constexpr CppObjType kObjectType = CppObjType::kFunctionPtr;

  std::string ownerName_;

  CppFunctionPointer(CppAccessType   accessType,
                     std::string     name,
                     CppVarType*     retType,
                     CppParamVector* params,
                     std::uint32_t   attr,
                     std::string     ownerName = std::string())
    : CppFunction(kObjectType, accessType, std::move(name), retType, params, attr)
    , ownerName_(std::move(ownerName))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (params_ != nullptr)
    {
      for (auto& p : *params_)
      {
        p->accept(v);
      }
    }
    VISIT_COND(v->visit(retType_.get()));
  }
};

using CppFunctionPointerEPtr = CppEasyPtr<CppFunctionPointer>;

/**
 * Class data member initialization as part of class constructor.
 */
using CppMemInit = std::pair<std::string, CppExpr*>;

/**
 * Entire member initialization list.
 */
struct CppMemInits
{
  bool memInitListIsABlob_;
  union
  {
    std::list<CppMemInit>* memInitList;
    CppBlob*               blob;
  };
};

inline CppMemInits makeEmptyCppMemInitList()
{
  return CppMemInits {false, {nullptr}};
}

inline CppMemInits makeCppMemInitList(std::list<CppMemInit>* memInitList)
{
  CppMemInits memInits = makeEmptyCppMemInitList();
  memInits.memInitList = memInitList;

  return memInits;
}

inline CppMemInits makeCppMemInitList(CppBlob* blob)
{
  CppMemInits memInits         = makeEmptyCppMemInitList();
  memInits.memInitListIsABlob_ = true;
  memInits.blob                = blob;

  return memInits;
}

struct CppConstructor : public CppFuncCtorBase
{
  static constexpr CppObjType kObjectType = CppObjType::kConstructor;

  CppMemInits memInits_;

  CppConstructor(CppAccessType   accessType,
                 std::string     name,
                 CppParamVector* params,
                 CppMemInits     memInitList,
                 std::uint32_t   attr)
    : CppFuncCtorBase(kObjectType, accessType, name, params, attr)
    , memInits_(memInitList)
  {
  }

  ~CppConstructor() override
  {
    if (memInits_.memInitListIsABlob_)
      delete memInits_.blob;
    else
      delete memInits_.memInitList;
  }

  bool isCopyConstructor();
  bool isMoveConstructor();

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (params_ != nullptr)
    {
      for (auto& p : *params_)
      {
        p->accept(v);
      }
    }
  }

private:
  mutable TriStateBool isCopyConstructor_ = TriStateBool::Unknown;
  mutable TriStateBool isMoveConstructor_ = TriStateBool::Unknown;
};

using CppConstructorEPtr = CppEasyPtr<CppConstructor>;

struct CppDestructor : public CppFunctionBase
{
  static constexpr CppObjType kObjectType = CppObjType::kDestructor;

  CppDestructor(CppAccessType accessType, std::string name, std::uint32_t attr)
    : CppFunctionBase(kObjectType, accessType, name, attr)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppDestructorEPtr = CppEasyPtr<CppDestructor>;

struct CppTypeConverter : public CppFunctionBase
{
  static constexpr CppObjType kObjectType = CppObjType::kTypeConverter;

  CppVarTypePtr to_;

  CppTypeConverter(CppVarType* type, std::string name)
    : CppFunctionBase(kObjectType, type->accessType_, std::move(name), 0)
    , to_(type)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (to_ != nullptr)
      VISIT_COND(v->visit(to_.get()));
  }
};

using CppTypeConverterEPtr = CppEasyPtr<CppTypeConverter>;

struct CppUsingNamespaceDecl : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kUsingNamespaceDecl;

  std::string name_;

  CppUsingNamespaceDecl(std::string name)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , name_(std::move(name))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppUsingNamespaceDeclEPtr = CppEasyPtr<CppUsingNamespaceDecl>;

struct CppUsingDecl : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kUsingDecl;

  std::string name_;
  CppObjPtr   cppObj_;

  CppUsingDecl(std::string name, CppVarType* varType)
    : CppObj(kObjectType, varType->accessType_)
    , name_(std::move(name))
    , cppObj_(varType)
  {
  }

  CppUsingDecl(std::string name, CppFunctionPointer* fptr)
    : CppObj(kObjectType, fptr->accessType_)
    , name_(std::move(name))
    , cppObj_(fptr)
  {
  }

  CppUsingDecl(std::string name, CppCompound* compound)
    : CppObj(kObjectType, compound->accessType_)
    , name_(std::move(name))
    , cppObj_(compound)
  {
  }

  CppUsingDecl(std::string name, CppAccessType accessType)
    : CppObj(kObjectType, accessType)
    , name_(std::move(name))
  {
  }

  CppTemplateParamList* templateParamList()
  {
    return templSpec_.get();
  }
  void templateParamList(CppTemplateParamList* templParamList)
  {
    templSpec_.reset(templParamList);
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (cppObj_ != nullptr)
      cppObj_->accept(v);
    // `templSpec_` is not an AST node, it is a member of `CppUsingDecl`
  }

private:
  CppTemplateParamListPtr templSpec_;
};

using CppUsingDeclEPtr = CppEasyPtr<CppUsingDecl>;

struct CppNamespaceAlias : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kNamespaceAlias;

  std::string name_;
  std::string alias_;

  CppNamespaceAlias(std::string name, std::string alias)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , name_(std::move(name))
    , alias_(std::move(alias))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppNamespaceAliasEPtr = CppEasyPtr<CppNamespaceAlias>;

struct CppDocComment : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kDocComment;

  std::string doc_; ///< Entire comment text

  CppDocComment(std::string doc, CppAccessType objAccessType = CppAccessType::kUnknown)
    : CppObj(kObjectType, objAccessType)
    , doc_(std::move(doc))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppDocCommentEPtr = CppEasyPtr<CppDocComment>;

struct CppExpr;
/**
 * An individual expression.
 */
struct CppExprAtom
{
  union
  {
    std::string* atom;
    CppExpr*     expr;
    CppLambda*   lambda;
    CppVarType*  varType; //!< For type cast, and sizeof expression.
  };

  enum
  {
    kInvalid,
    kAtom,
    kExpr,
    kLambda,
    kVarType
  } type;

  bool isExpr()
  {
    return (type & kExpr) == kExpr;
  }

  CppExprAtom(char* sz, size_t l)
    : atom(new std::string(sz, l))
    , type(kAtom)
  {
  }
  CppExprAtom(char* sz)
    : atom(new std::string(sz))
    , type(kAtom)
  {
  }
  CppExprAtom(std::string tok)
    : atom(new std::string(std::move(tok)))
    , type(kAtom)
  {
  }
  CppExprAtom(CppExpr* e)
    : expr(e)
    , type(kExpr)
  {
  }
  CppExprAtom(CppLambda* l)
    : lambda(l)
    , type(kLambda)
  {
  }
  CppExprAtom(CppVarType* vType)
    : varType(vType)
    , type(kVarType)
  {
  }
  CppExprAtom()
    : atom(nullptr)
    , type(kInvalid)
  {
  }
  /**
   * It is expected to be called explicitly to destroy an CppExprAtom object.
   */
  void destroy();
};

/**
 * \brief An expression in a C/C++ program.
 *
 * In C/C++ an expression is a complicated beast that needs to be tamed to parse it correctly.
 * An expression can be as simple as a word, e.g. x, 5 are valid expressions.
 * It can be a function call.
 * It can be an initialization list for an array or struct. e.g. POINT pt = {100, 500};
 * It can be a list of expressions used to call a function e.g. gcd(36, 42);
 * new/delete statements are also expressions.
 * It can be any of the arithmetic expression, e.g. a+b.
 * To make things simple we treat a return statement as an expression.
 *
 * structs CppExprAtom, and CppExpr are required to tame this difficult beast called expression in C/C++.
 */
struct CppExpr : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kExpression;

  enum Flag
  {
    kReturn = 0x01,
    kNew    = 0x02,
    // kNewArray		= 0x04, // This is not needed.
    kDelete       = 0x08,
    kDeleteArray  = 0x10,
    kBracketed    = 0x20,
    kInitializer  = 0x40,
    kThrow        = 0x80,
    kSizeOf       = 0x100,
    kVariadicPack = 0x200,
    kGoto         = 0x400,
  };

  CppExprAtom expr1_ {(CppExpr*) (nullptr)};
  CppExprAtom expr2_ {(CppExpr*) (nullptr)};
  CppExprAtom expr3_ {(CppExpr*) (nullptr)};
  CppOperator oper_;
  short       flags_; // ORed combination of Flag constants.

  CppExpr(CppExprAtom e1, CppOperator op, CppExprAtom e2 = CppExprAtom())
    : CppExpr(e1, op, e2, 0)
  {
  }

  CppExpr(CppExprAtom e1, short flags)
    : CppExpr(e1, kNone, CppExprAtom(), flags)
  {
  }

  // CppExpr(CppExprAtom e1, CppOperator op, short flags)
  //   : CppExpr(e1, op, CppExprAtom(), flags)
  // {
  // }

  CppExpr(CppExprAtom e1, CppOperator op, CppExprAtom e2, short flags)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , expr1_(e1)
    , expr2_(e2)
    , oper_(op)
    , flags_(flags)
  {
  }

  CppExpr(CppExprAtom e1, CppExprAtom e2, CppExprAtom e3)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , expr1_(e1)
    , expr2_(e2)
    , expr3_(e3)
    , oper_(kTertiaryOperator)
    , flags_(0)
  {
  }

  CppExpr(std::string name)
    : CppExpr(CppExprAtom(std::move(name)), CppOperator::kNone)
  {
  }

  CppExpr(CppLambda* l)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , expr1_(l)
    , oper_(kNone)
    , flags_(0)
  {
  }

  ~CppExpr() override
  {
    expr1_.destroy();
    expr2_.destroy();
    expr3_.destroy();
  }

  void accept(CppVisitorBase* v) override
  {
    // std::cout<<"\naccept expr "<< this << "\n";
    VISIT_COND(v->visit(this));
    for (int i = 0; i < 3; i++)
    {
      auto* p = i == 0 ? &expr1_ : i == 1 ? &expr2_ : i == 2 ? &expr3_ : nullptr;
      if (p->expr == nullptr) // it's a union, p->expr == null means the whole thing is null.
        continue;
      switch (p->type)
      {
        case CppExprAtom::kExpr: {
          // std::cout<<"\naccept expr sub expr"<< i <<" "<< this << "\n";
          p->expr->accept(v);
          break;
        }
        case CppExprAtom::kLambda: {
          // std::cout<<"\naccept expr sub lambda"<< i <<" "<< this << "\n";
          p->lambda->accept(v);
          break;
        }
        case CppExprAtom::kVarType: {
          // std::cout<<"\naccept expr sub varType"<< i <<" "<< this << "\n";
          VISIT_COND(v->visit(p->varType));
          break;
        }
        default: { // Atom (is not an AST node) and Invalid
          break;
        }
      }
    }
  }
};

using CppExprEPtr = CppEasyPtr<CppExpr>;

//////////////////////////////////////////////////////////////////////////

struct CppEnumItem
{
  std::string name_;
  CppObjPtr   val_;

  CppEnumItem(std::string name, CppExpr* val = nullptr)
    : name_(std::move(name))
    , val_(val)
  {
  }

  CppEnumItem(CppObj* anyItem)
    : val_(anyItem)
  {
  }
};

using CppEnumItemList    = std::list<CppEnumItem*>;
using CppEnumItemListPtr = std::unique_ptr<CppEnumItemList>;

struct CppEnum : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kEnum;

  std::string        name_;     // Can be empty for anonymous enum.
  CppEnumItemListPtr itemList_; // Can be nullptr for forward declared enum.
  bool               isClass_;
  std::string        underlyingType_;

  CppEnum(CppAccessType    accessType,
          std::string      name,
          CppEnumItemList* itemList,
          bool             isClass        = false,
          std::string      underlyingType = std::string())
    : CppObj(kObjectType, accessType)
    , name_(std::move(name))
    , itemList_(itemList)
    , isClass_(isClass)
    , underlyingType_(std::move(underlyingType))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    // `CppEnumItemListPtr` is a vector of base type `CppEnumItem` which is not an AST node type.
    // but that type contains `CppObjPtr`, idk, maybe `itemList_` should be included here.
    /// TODO: decide on this.
  }
};

using CppEnumEPtr = CppEasyPtr<CppEnum>;

/// Some blocks have common structure like if, while, and do-while.
/// They all contain a body and an expression of condition.
template <CppObjType _ObjType>
struct CppCommonBlock : public CppObj
{
  static constexpr CppObjType kObjectType = _ObjType;

  CppCommonBlock(CppObj* cond, CppObj* body)
    : CppObj(_ObjType, CppAccessType::kUnknown)
    , cond_(cond)
    , body_(body)
  {
  }

  CppObjPtr cond_;
  CppObjPtr body_;

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (cond_ != nullptr)
      cond_->accept(v);
    if (body_ != nullptr)
      body_->accept(v);
  }
};

struct CppIfBlock : public CppCommonBlock<CppObjType::kIfBlock>
{
  CppIfBlock(CppObj* cond, CppObj* body, CppObj* _else = nullptr)
    : CppCommonBlock(cond, body)
    , else_ {_else}
  {
  }
  CppObj* elsePart()
  {
    return else_.get();
  }
  void elsePart(CppObj* _elsePart)
  {
    else_.reset(_elsePart);
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (cond_ != nullptr)
      cond_->accept(v); // assuming that `cond_` pointer cannot be nullptr ever.
    if (body_ != nullptr)
      body_->accept(v);
    if (else_ != nullptr)
      else_->accept(v);
  }

private:
  CppObjPtr else_;
};

using CppIfBlockEPtr = CppEasyPtr<CppIfBlock>;

using CppWhileBlock   = CppCommonBlock<CppObjType::kWhileBlock>;
using CppDoWhileBlock = CppCommonBlock<CppObjType::kDoWhileBlock>;

using CppWhileBlockEPtr   = CppEasyPtr<CppWhileBlock>;
using CppDoWhileBlockEPtr = CppEasyPtr<CppDoWhileBlock>;

struct CppForBlock : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kForBlock;

  CppObjPtr  start_;
  CppExprPtr stop_;
  CppExprPtr step_;
  CppObjPtr  body_;

  CppForBlock(CppObj* start, CppExpr* stop, CppExpr* step, CppObj* body)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , start_(start)
    , stop_(stop)
    , step_(step)
    , body_(body)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (start_ != nullptr)
      start_->accept(v);
    if (stop_ != nullptr)
      VISIT_COND(v->visit(stop_.get()));
    if (step_ != nullptr)
      VISIT_COND(v->visit(step_.get()));
    if (body_ != nullptr)
      body_->accept(v);
  }
};

struct CppRangeForBlock : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kRangeForBlock;

  CppVarPtr  var_;
  CppExprPtr expr_;
  CppObjPtr  body_;

  CppRangeForBlock(CppVar* var, CppExpr* expr, CppObj* body)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , var_(var)
    , expr_(expr)
    , body_(body)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (var_ != nullptr)
      VISIT_COND(v->visit(var_.get()));
    if (expr_ != nullptr)
      VISIT_COND(v->visit(expr_.get()));
    if (body_ != nullptr)
      body_->accept(v);
  }
};

using CppForBlockEPtr = CppEasyPtr<CppForBlock>;

struct CppCase
{
  CppCase(CppExpr* cond, CppCompound* body)
    : case_(cond)
    , body_(body)
  {
  }

  CppExprPtr     case_;
  CppCompoundPtr body_;
};

using CppSwitchBody    = std::vector<CppCase>;
using CppSwitchBodyPtr = std::unique_ptr<CppSwitchBody>;

struct CppSwitchBlock : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kSwitchBlock;

  CppExprPtr       cond_;
  CppSwitchBodyPtr body_;

  CppSwitchBlock(CppExpr* cond, CppSwitchBody* body)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , cond_(cond)
    , body_(body)
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (cond_ != nullptr)
      VISIT_COND(v->visit(cond_.get()));
    for (auto& c : *body_)
    {
      if (c.case_ != nullptr)
        VISIT_COND(v->visit(c.case_.get()));
      if (c.body_ != nullptr)
        c.body_->accept(v);
    }
  }
};

using CppSwitchBlockEPtr = CppEasyPtr<CppSwitchBlock>;

struct CppCatchBlock
{
  CppVarTypePtr  exceptionType_;
  std::string    exceptionName_;
  CppCompoundPtr catchStmt_;
};

using CppCatchBlockPtr = std::unique_ptr<CppCatchBlock>;

using CppCatchBlocks = std::vector<CppCatchBlockPtr>;

struct CppTryBlock : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kTryBlock;
  CppCompoundPtr              tryStmt_;

  CppTryBlock(CppCompound* tryStmt, CppCatchBlock* firstCatchBlock)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , tryStmt_(tryStmt)
  {
    catchBlocks_.emplace_back(firstCatchBlock);
  }

  void addCatchBlock(CppCatchBlock* catchBlock)
  {
    catchBlocks_.emplace_back(catchBlock);
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
    if (tryStmt_ != nullptr)
      tryStmt_->accept(v); // a block of code
    for (auto& c : catchBlocks_)
    {
      if (c->exceptionType_ != nullptr)
        VISIT_COND(v->visit(c->exceptionType_.get()));
      // `c->exceptionName_` is string, not of an AST node type.
      if (c->catchStmt_ != nullptr)
        c->catchStmt_->accept(v); // a block of code
    }
  }

private:
  CppCatchBlocks catchBlocks_;
};

// Templare argument needs more robust support.
// As of now we are treating them just as string.
// But for parsing we need to have a type.
struct CppTemplateArg;

struct CppAsmBlock : public CppObj
{
  std::string asm_; // Entire asm block including keyword asm.

  CppAsmBlock(std::string asmBlock)
    : CppObj(CppObjType::kAsmBlock, CppAccessType::kUnknown)
    , asm_(std::move(asmBlock))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

//////////////////////////////////////////////////////////////////////////

inline void CppExprAtom::destroy()
{
  switch (type)
  {
    case CppExprAtom::kAtom:
      delete atom;
      break;
    case CppExprAtom::kExpr:
      delete expr;
      break;
    case CppExprAtom::kVarType:
      delete varType;
      break;
    case CppExprAtom::kLambda:
      delete lambda;
      break;

    default:
      break;
  }
}

struct CppLabel : public CppObj
{
  static constexpr CppObjType kObjectType = CppObjType::kLabel;

  std::string label_;

  CppLabel(std::string label)
    : CppObj(kObjectType, CppAccessType::kUnknown)
    , label_(std::move(label))
  {
  }

  void accept(CppVisitorBase* v) override
  {
    VISIT_COND(v->visit(this));
  }
};

using CppLabelEPtr = CppEasyPtr<CppLabel>;

inline CppVarDecl::CppVarDecl(std::string name, CppExpr* assign, AssignType assignType)
  : name_(std::move(name))
  , assignValue_(assign)
  , assignType_(assignType)
{
}

inline CppVarType::CppVarType(std::string baseType, CppTypeModifier modifier)
  : CppVarType(CppAccessType::kUnknown, std::move(baseType), modifier)
{
}

inline CppVarType::CppVarType(CppAccessType accessType, std::string baseType, CppTypeModifier modifier)
  : CppVarType(accessType, std::move(baseType), 0, modifier)
{
}

inline CppVarType::CppVarType(CppAccessType accessType, CppCompound* compound, CppTypeModifier modifier)
  : CppObj(kObjectType, accessType)
  , compound_(compound)
  , typeModifier_(modifier)
{
}

inline CppVarType::CppVarType(CppAccessType accessType, CppFunctionPointer* compound, CppTypeModifier modifier)
  : CppObj(kObjectType, accessType)
  , compound_(compound)
  , typeModifier_(modifier)
{
}

inline CppVarType::CppVarType(CppAccessType accessType, CppEnum* enumObj, CppTypeModifier modifier)
  : CppObj(kObjectType, accessType)
  , compound_(enumObj)
  , typeModifier_(modifier)
{
}

inline CppVarType::CppVarType(CppVarType& varType)
  : CppVarType(varType.accessType_, varType.baseType(), varType.typeModifier())
{
  // TODO: clone compound_.
}

inline CppTemplateParam::CppTemplateParam(CppFunctionPointer* paramType, std::string paramName)
  : paramType_(paramType)
  , paramName_(std::move(paramName))
{
}

//////////////////////////////////////////////////////////////////


bool operator==(const CppExpr& expr1, const CppExpr& expr2);

inline bool operator!=(const CppExpr& expr1, const CppExpr& expr2)
{
  return !(expr1 == expr2);
}

inline bool operator==(const CppExprAtom& exprAtom1, const CppExprAtom& exprAtom2)
{
  if (exprAtom1.type != exprAtom2.type)
    return false;
  if (exprAtom1.type == CppExprAtom::kAtom)
  {
    return (*exprAtom1.atom) == (*exprAtom2.atom);
  }
  if (exprAtom1.type == CppExprAtom::kExpr)
  {
    return (*exprAtom1.expr) == (*exprAtom2.expr);
  }
  return false;
}

inline bool operator!=(const CppExprAtom& exprAtom1, const CppExprAtom& exprAtom2)
{
  return !(exprAtom1 == exprAtom2);
}
