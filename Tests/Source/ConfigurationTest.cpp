#include <Qombat/Tests.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace QMBT;

class ConfigTestClass
{
  public:
	static void Init()
	{
		CONFIG_GROUP("TestGroup",
					 CONFIG_INT(s_Member, "Member", "Sets the test member", -20, 30),
					 CONFIG_STRING(s_Member2, "Member2", "Sets the test member2"));
	}

	//inline static std::shared_ptr<spdlog::logger>& Get() { return s_Logger; }
	//inline static std::shared_ptr<spdlog::logger> s_Logger;
	inline static int s_Member = 5;
	inline static std::string s_Member2 = "member";
};

TEST_CASE("Int Configuration Test", "[Core]")
{
	ConfigTestClass::Init();

	ConfigManager::SetConfig("TestGroup", "Member", 23);

	REQUIRE(ConfigTestClass::s_Member == 23);
}

TEST_CASE("String Configuration Test", "[Core]")
{
	ConfigTestClass::Init();

	ConfigManager::SetConfig("TestGroup", "Member2", "foo");

	REQUIRE(ConfigTestClass::s_Member2 == "foo");
}
