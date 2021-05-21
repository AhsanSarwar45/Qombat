
#include <Qombat/Tests.hpp>
#include <catch2/catch_session.hpp>

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