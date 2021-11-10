#include <catch/catch.hpp>

#include "cppparser.h"

#include "embedded-snippet-test-base.h"

#include <string>

class ExpressionTest : public EmbeddedSnippetTestBase
{
protected:
  ExpressionTest()
    : EmbeddedSnippetTestBase(__FILE__)
  {
  }
};

TEST_CASE_METHOD(ExpressionTest, "new char* []")
{
#if TEST_CASE_SNIPPET_STARTS_FROM_NEXT_LINE
  auto p = new char*[5];
#endif
  auto testSnippet = getTestSnippetParseStream(__LINE__ - 2);

  CppParser  parser;
  const auto ast = parser.parseStream(testSnippet.data(), testSnippet.size());
  REQUIRE(ast != nullptr);

  const auto& members = ast->members();
  REQUIRE(members.size() == 1);

  CppVarEPtr var = members[0];
  REQUIRE(var);

  CHECK(var->assignValue() != nullptr);
}
