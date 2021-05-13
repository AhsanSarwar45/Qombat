#include <vector>

#include <Qombat/Tests.hpp>
#include <catch2/catch.hpp>

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

TEST_CASE("PoolAllocator Allocation Test", "[core]")
{
	SECTION("NumAllocations = 1")
	{
		PoolAllocator poolAllocator = PoolAllocator<TestObject>("Allocator", 8);

		TestObject* object = poolAllocator.New(2, 5.2f, 'c', false, 0.3f);

		REQUIRE(poolAllocator.GetUsedSpace() == sizeof(TestObject));

		REQUIRE(object->a == 2);
		REQUIRE(object->b == 5.2f);
		REQUIRE(object->c == 'c');
		REQUIRE(object->d == false);
		REQUIRE(object->e == 0.3f);

		poolAllocator.Delete(object);
	}

	SECTION("1 < NumAllocations <= chunksPerBlock")
	{
		PoolAllocator poolAllocator = PoolAllocator<TestObject>("Allocator", 8);

		std::vector<TestObject*> objectPtrs = std::vector<TestObject*>();

		for (int i = 0; i < 8; i++)
		{
			objectPtrs.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, 8 % 2, 10.6f + (2 * i)));
		}

		REQUIRE(poolAllocator.GetUsedSpace() == 8 * sizeof(TestObject));

		for (int i = 0; i < 8; i++)
		{

			REQUIRE(objectPtrs[i]->a == i);
			REQUIRE(objectPtrs[i]->b == 2.1f + i);
			REQUIRE(objectPtrs[i]->c == 'a' + i);
			REQUIRE(objectPtrs[i]->d == 8 % 2);
			REQUIRE(objectPtrs[i]->e == 10.6f + (2 * i));
		}
	}
}

TEST_CASE("PoolAllocator Deallocation Test", "[core]")
{
	PoolAllocator poolAllocator = PoolAllocator<TestObject>("Allocator", 8);

	TestObject* object = poolAllocator.New(2, 5.2f, 'c', false, 0.3f);

	poolAllocator.Delete(object);

	REQUIRE(poolAllocator.GetUsedSpace() == 0);
}
