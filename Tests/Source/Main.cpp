#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <Qombat/Tests.hpp>

int main(int argc, char* argv[])
{
	// global setup...
	QMBT::Logger::InitializeEngineLoggers();

	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	return result;
}