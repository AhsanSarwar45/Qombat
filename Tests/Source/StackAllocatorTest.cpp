#include <Qombat/Tests.hpp>
#include <catch2/catch_test_macros.hpp>

#include "MemoryTestObjects.hpp"

using namespace QMBT;

TEST_CASE("StackAllocator Initialisation Test", "[Memory]") {
  StackAllocator stackAllocator = StackAllocator("Allocator", 10_MB);

  REQUIRE(stackAllocator.GetUsedSize() == 0);
}

TEST_CASE("StackAllocator Allocation Test", "[Memory]") {
  StackAllocator stackAllocator = StackAllocator("Allocator", 10_MB);

  SECTION("Single Object") {
    TestObject *object =
        stackAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);

    REQUIRE(object->a == 1);
    REQUIRE(object->b == 2.1f);
    REQUIRE(object->c == 'a');
    REQUIRE(object->d == false);
    REQUIRE(object->e == 10.6f);
  }

  SECTION("Multiple Objects") {
    TestObject *object =
        stackAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);
    TestObject2 *object2 = stackAllocator.New<TestObject2>(2, 5.4, 8.2, false,
                                                           std::vector<int>(6));

    REQUIRE(object->a == 1);
    REQUIRE(object->b == 2.1f);
    REQUIRE(object->c == 'a');
    REQUIRE(object->d == false);
    REQUIRE(object->e == 10.6f);

    REQUIRE(object2->a == 2);
    REQUIRE(object2->b == 5.4);
    REQUIRE(object2->c == 8.2);
    REQUIRE(object2->d == false);
    REQUIRE(object2->e.size() == 6);
  }
}

TEST_CASE("StackAllocator Deallocation Test", "[Memory]") {
  StackAllocator stackAllocator = StackAllocator("Stack Allocator", 10_MB);

  SECTION("Single Object") {
    TestObject *object =
        stackAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);

    stackAllocator.Delete(object);
  }

  SECTION("Multiple Objects") {
    TestObject *object =
        stackAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);
    TestObject2 *object2 = stackAllocator.New<TestObject2>(2, 5.4, 8.2, false,
                                                           std::vector<int>(6));

    stackAllocator.Delete(object2);
    stackAllocator.Delete(object);
  }
}

TEST_CASE("StackAllocator Reallocation Test", "[Memory]") {
  StackAllocator stackAllocator = StackAllocator("Stack Allocator", 10_MB);

  SECTION("Single Object") {
    TestObject *object =
        stackAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);

    stackAllocator.Delete(object);

    TestObject *objectNew =
        stackAllocator.New<TestObject>(2, 2.1f, 'a', false, 10.6f);

    REQUIRE(objectNew->a == 2);
    REQUIRE(objectNew->b == 2.1f);
    REQUIRE(objectNew->c == 'a');
    REQUIRE(objectNew->d == false);
    REQUIRE(objectNew->e == 10.6f);
  }

  SECTION("Multiple Objects") {
    TestObject *object =
        stackAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);
    TestObject2 *object2 = stackAllocator.New<TestObject2>(2, 5.4, 8.2, false,
                                                           std::vector<int>(6));

    stackAllocator.Delete(object2);
    stackAllocator.Delete(object);

    TestObject *objectNew =
        stackAllocator.New<TestObject>(2, 2.1f, 'a', false, 10.6f);
    TestObject2 *object2New = stackAllocator.New<TestObject2>(
        3, 5.4, 8.2, false, std::vector<int>(6));

    REQUIRE(objectNew->a == 2);
    REQUIRE(objectNew->b == 2.1f);
    REQUIRE(objectNew->c == 'a');
    REQUIRE(objectNew->d == false);
    REQUIRE(objectNew->e == 10.6f);

    REQUIRE(object2New->a == 3);
    REQUIRE(object2New->b == 5.4);
    REQUIRE(object2New->c == 8.2);
    REQUIRE(object2New->d == false);
    REQUIRE(object2New->e.size() == 6);
  }
}