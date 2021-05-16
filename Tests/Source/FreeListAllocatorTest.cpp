#include <catch2/catch.hpp>

#include <Qombat/Tests.hpp>

#include "MemoryTestObjects.hpp"

using namespace QMBT;

TEST_CASE("FreeListAllocator Initialisation Test", "[Memory]")
{
	FreeListAllocator freeListAllocator = FreeListAllocator("FreeList Allocator", 10_MB);

	REQUIRE(freeListAllocator.GetUsedSize() == 0);
}

TEST_CASE("FreeListAllocator Allocation Test", "[Memory]")
{
	FreeListAllocator freeListAllocator = FreeListAllocator("FreeList Allocator", 10_MB);

	SECTION("Single Object")
	{
		TestObject* object = freeListAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);

		REQUIRE(object->a == 1);
		REQUIRE(object->b == 2.1f);
		REQUIRE(object->c == 'a');
		REQUIRE(object->d == false);
		REQUIRE(object->e == 10.6f);
	}

	SECTION("Multiple Objects")
	{
		TestObject* object = freeListAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);
		TestObject2* object2 = freeListAllocator.New<TestObject2>(2, 5.4, 8.2, false, std::vector<int>(6));

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

TEST_CASE("FreeListAllocator Deallocation Test", "[Memory]")
{
	FreeListAllocator freeListAllocator = FreeListAllocator("Stack Allocator", 10_MB);

	SECTION("Single Object")
	{
		TestObject* object = freeListAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);

		freeListAllocator.Delete(object);
	}

	SECTION("Multiple Objects")
	{
		TestObject* object = freeListAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);
		TestObject2* object2 = freeListAllocator.New<TestObject2>(2, 5.4, 8.2, false, std::vector<int>(6));

		SECTION("Normal Order")
		{
			freeListAllocator.Delete(object2);
			freeListAllocator.Delete(object);
		}

		SECTION("Reverse Order")
		{
			freeListAllocator.Delete(object);
			freeListAllocator.Delete(object2);
		}
	}
}

TEST_CASE("FreeListAllocator Reallocation Test", "[Memory]")
{
	FreeListAllocator freeListAllocator = FreeListAllocator("Stack Allocator", 10_MB);

	SECTION("Single Object")
	{
		TestObject* object = freeListAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);

		freeListAllocator.Delete(object);

		TestObject* objectNew = freeListAllocator.New<TestObject>(2, 2.1f, 'a', false, 10.6f);

		REQUIRE(objectNew->a == 2);
		REQUIRE(objectNew->b == 2.1f);
		REQUIRE(objectNew->c == 'a');
		REQUIRE(objectNew->d == false);
		REQUIRE(objectNew->e == 10.6f);
	}

	SECTION("Multiple Objects")
	{
		TestObject* object = freeListAllocator.New<TestObject>(1, 2.1f, 'a', false, 10.6f);
		TestObject2* object2 = freeListAllocator.New<TestObject2>(2, 5.4, 8.2, false, std::vector<int>(6));

		freeListAllocator.Delete(object2);
		freeListAllocator.Delete(object);

		TestObject* objectNew = freeListAllocator.New<TestObject>(2, 2.1f, 'a', false, 10.6f);
		TestObject2* object2New = freeListAllocator.New<TestObject2>(3, 5.4, 8.2, false, std::vector<int>(6));

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
