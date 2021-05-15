#include <vector>

#include <Qombat/Tests.hpp>
#include <catch2/catch.hpp>

#include "MemoryTestObjects.hpp"

using namespace QMBT;

TEST_CASE("PoolAllocatorBlocked Initialisation Test", "[core]")
{
	PoolAllocatorBlocked<TestObject> poolAllocator = PoolAllocatorBlocked<TestObject>("Allocator", 8);

	REQUIRE(poolAllocator.GetUsedSize() == 0);
}

TEST_CASE("PoolAllocatorBlocked Allocation Test", "[core]")
{
	PoolAllocatorBlocked<TestObject> poolAllocator = PoolAllocatorBlocked<TestObject>("Allocator", 8);
	std::vector<TestObject*> objectPtrs = std::vector<TestObject*>();

	int numAllocations = 0;

	SECTION("NumAllocations = 1")
	{
		numAllocations = 1;
	}

	SECTION("1 < NumAllocations <= chunksPerBlock")
	{
		numAllocations = 8;
	}

	SECTION("NumAllocations >= chunksPerBlock")
	{
		numAllocations = 12;
	}

	for (int i = 0; i < numAllocations; i++)
	{
		objectPtrs.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, numAllocations % 2, 10.6f + (2 * i)));
	}

	REQUIRE(poolAllocator.GetUsedSize() == numAllocations * sizeof(TestObject));

	for (int i = 0; i < numAllocations; i++)
	{

		REQUIRE(objectPtrs[i]->a == i);
		REQUIRE(objectPtrs[i]->b == 2.1f + i);
		REQUIRE(objectPtrs[i]->c == 'a' + i);
		REQUIRE(objectPtrs[i]->d == numAllocations % 2);
		REQUIRE(objectPtrs[i]->e == 10.6f + (2 * i));
	}
}

TEST_CASE("PoolAllocatorBlocked Deallocation Test", "[core]")
{
	PoolAllocatorBlocked<TestObject> poolAllocator = PoolAllocatorBlocked<TestObject>("Allocator", 8);
	std::vector<TestObject*> objectPtrs = std::vector<TestObject*>();

	int numAllocations = 0;

	SECTION("Normal Order")
	{

		SECTION("NumAllocations = 1")
		{
			numAllocations = 1;
		}

		SECTION("1 < NumAllocations <= chunksPerBlock")
		{
			numAllocations = 8;
		}

		SECTION("NumAllocations >= chunksPerBlock")
		{
			numAllocations = 12;
		}

		for (int i = 0; i < numAllocations; i++)
		{
			objectPtrs.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, numAllocations % 2, 10.6f + (2 * i)));
		}

		for (int i = 0; i < numAllocations; i++)
		{
			poolAllocator.Delete(objectPtrs[i]);
		}
	}

	SECTION("Reverse Order")
	{

		SECTION("NumAllocations = 1")
		{
			numAllocations = 1;
		}

		SECTION("1 < NumAllocations <= chunksPerBlock")
		{
			numAllocations = 8;
		}

		SECTION("NumAllocations >= chunksPerBlock")
		{
			numAllocations = 12;
		}

		for (int i = 0; i < numAllocations; i++)
		{
			objectPtrs.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, numAllocations % 2, 10.6f + (2 * i)));
		}

		for (int i = numAllocations - 1; i >= 0; i--)
		{
			poolAllocator.Delete(objectPtrs[i]);
		}
	}

	REQUIRE(poolAllocator.GetUsedSize() == 0);
}

TEST_CASE("PoolAllocatorBlocked Reallocation Test", "[core]")
{
	PoolAllocatorBlocked<TestObject> poolAllocator = PoolAllocatorBlocked<TestObject>("Allocator", 8);
	std::vector<TestObject*> objectPtrs = std::vector<TestObject*>();
	int numAllocations = 0;

	SECTION("Normal Order")
	{

		SECTION("NumAllocations = 1")
		{
			numAllocations = 1;
		}

		SECTION("1 < NumAllocations <= chunksPerBlock")
		{
			numAllocations = 8;
		}

		SECTION("NumAllocations >= chunksPerBlock")
		{
			numAllocations = 12;
		}

		for (int i = 0; i < numAllocations; i++)
		{
			objectPtrs.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, numAllocations % 2, 10.6f + (2 * i)));
		}

		for (int i = 0; i < numAllocations; i++)
		{
			poolAllocator.Delete(objectPtrs[i]);
		}
	}

	SECTION("Reverse Order")
	{

		SECTION("NumAllocations = 1")
		{
			numAllocations = 1;
		}

		SECTION("1 < NumAllocations <= chunksPerBlock")
		{
			numAllocations = 8;
		}

		SECTION("NumAllocations >= chunksPerBlock")
		{
			numAllocations = 12;
		}

		for (int i = 0; i < numAllocations; i++)
		{
			objectPtrs.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, numAllocations % 2, 10.6f + (2 * i)));
		}

		for (int i = numAllocations - 1; i >= 0; i--)
		{
			poolAllocator.Delete(objectPtrs[i]);
		}
	}

	std::vector<TestObject*> objectPtrs2 = std::vector<TestObject*>();

	for (int i = 0; i < numAllocations; i++)
	{
		objectPtrs2.push_back(poolAllocator.New(i, 2.1f + i, 'a' + i, numAllocations % 2, 10.6f + (2 * i)));
	}

	REQUIRE(poolAllocator.GetUsedSize() == numAllocations * sizeof(TestObject));

	for (int i = 0; i < numAllocations; i++)
	{

		REQUIRE(objectPtrs2[i]->a == i);
		REQUIRE(objectPtrs2[i]->b == 2.1f + i);
		REQUIRE(objectPtrs2[i]->c == 'a' + i);
		REQUIRE(objectPtrs2[i]->d == numAllocations % 2);
		REQUIRE(objectPtrs2[i]->e == 10.6f + (2 * i));
	}
}
