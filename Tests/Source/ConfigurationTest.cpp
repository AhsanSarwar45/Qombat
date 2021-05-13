#include <catch2/catch.hpp>

#include <Qombat/Tests.hpp>

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

TEST_CASE("Config Test", "[core]")
{
	ConfigTestClass::Init();

	ConfigManager::SetConfigInt("TestGroup", "Member", 23);

	REQUIRE(ConfigTestClass::s_Member == 23);
}
