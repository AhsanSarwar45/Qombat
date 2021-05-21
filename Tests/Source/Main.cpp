#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <Qombat/Tests.hpp>

class TestApplication : public QMBT::Application
{
  public:
	TestApplication(){};
	~TestApplication(){};
};

int main(int argc, char* argv[])
{
	// global setup...
	QMBT::Logger::InitializeEngineLoggers();
	auto app = new TestApplication;

	int result = Catch::Session().run(argc, argv);

	// global clean-up...
	delete app;

	return result;
}