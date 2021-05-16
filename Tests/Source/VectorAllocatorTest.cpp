#include <Qombat/Tests.hpp>
#include <catch2/catch.hpp>

using namespace QMBT;

TEST_CASE("VectorAllocator Allocation Test", "[Memory]")
{
	Vector<int> vec(VectorAllocator("Test Allocator"));

	for (int i = 0; i < 50; i++)
	{
		vec.push_back(i);
	}

	vec.get_allocator().
}
