#include <catch2/catch.hpp>

#include <Qombat/Tests.hpp>

using namespace QMBT;

struct TestObject
{
	int a;
	float b;
	char c;
	bool d;
	float e;

	TestObject(int _a, float _b, char _c, bool _d, float _e)
		: a(_a), b(_b), c(_c), d(_d), e(_e) {}
};

TEST_CASE("FreeList Allocation Test", "[core]")
{
	SECTION("NumAllocations == 1")
	{
		FreeListAllocator allocator = FreeListAllocator();

		TestObject* object = allocator.New<TestObject>(2, 5.2f, 'c', false, 0.3f);

		REQUIRE(object->a == 2);
		REQUIRE(object->b == 5.2f);
		REQUIRE(object->c == 'c');
		REQUIRE(object->d == false);
		REQUIRE(object->e == 0.3f);

		allocator.Delete(object);
	}
}

// TEST_CASE("PoolAllocator Deallocation Test", "[core]")
// {
// 	PoolAllocator poolAllocator = PoolAllocator<TestObject>("Allocator", 8);

// 	TestObject* object = poolAllocator.New(2, 5.2f, 'c', false, 0.3f);

// 	poolAllocator.Deallocate(object);

// 	REQUIRE(poolAllocator.GetUsedSpace() == 0);
// }