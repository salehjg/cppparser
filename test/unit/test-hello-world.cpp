#include <catch/catch.hpp>

#include "cppparser.h"
#include "cppvisitormatcher.h"
#include "cppvisitorprinter.h"
#include "cppwriter.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

namespace fs = boost::filesystem;

TEST_CASE("Parsing hello world program")
{
  CppParser  parser;
  const auto testFilePath = fs::path(__FILE__).parent_path() / "test-files/hello-world.cpp";
  const auto ast          = parser.parseFile(testFilePath.string());

  REQUIRE(ast != nullptr);

  {
    // CppVisitorPrinter visitor;
    // ast->accept(&visitor);
  } {
    CppVisitorMatcher matcher({CppObjType::kExpression, CppObjType::kHashInclude});
    ast->accept(&matcher);
  }

  const auto& members = ast->members();
  REQUIRE(members.size() == 2);

  CppIncludeEPtr hashInclude = members[0];
  REQUIRE(hashInclude);
  CHECK(hashInclude->name_ == "<iostream>");

  CppFunctionEPtr func = members[1];
  REQUIRE(func);
  CHECK(func->name_ == "main");

  REQUIRE(func->defn());
  const auto& mainBodyMembers = func->defn()->members();
  REQUIRE(mainBodyMembers.size() == 2);

  CppExprEPtr coutHelloWorld = mainBodyMembers[0];
  REQUIRE(coutHelloWorld);
  CHECK(coutHelloWorld->oper_ == CppOperator::kInsertion);
}

class MyAstModifierVisitor : public CppVisitorMatcher
{
public:
  explicit MyAstModifierVisitor(const std::vector<CppObjType>& typesToMatch)
    : CppVisitorMatcher(typesToMatch)
  {
  }
  bool matched(CppObj* p) override
  {
    auto* pCast = (CppFunction*) p;
    pCast->name_ += "FuncNameModified";
    return true;
  }
};

TEST_CASE("Modifying AST with a visitor")
{
  CppParser  parser;
  const auto testFilePath = fs::path(__FILE__).parent_path() / "test-files/hello-world.cpp";
  const auto ast = parser.parseFile(testFilePath.string());
  REQUIRE(ast != nullptr);
  MyAstModifierVisitor matcher({CppObjType::kFunction});
  ast->accept(&matcher);

  std::ofstream ofStream("/tmp/T3.txt");
  CppWriter cppWriter;
  cppWriter.emit(ast.get(), ofStream);
  std::cout << "Done." << std::endl;
}
