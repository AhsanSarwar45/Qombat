
#include <Qombat/Tests.hpp>
#include <catch2/catch_session.hpp>

int main(int argc, char* argv[])
{
	// global setup...
	QMBT::Logger::InitializeEngineLoggers();

	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	return result;
}