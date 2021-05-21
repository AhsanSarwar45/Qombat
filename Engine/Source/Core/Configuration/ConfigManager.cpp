#include "ConfigManager.hpp"

namespace QMBT
{
	ConfigInt::ConfigInt(int& dataReference, const char* name, const char* description, int min, int max)
		: m_Data(dataReference), m_Name(name), m_Description(description), m_Max(max), m_Min(min), m_NameHash(Utility::GenerateHash(name))
	{
		if (max == 0 && min == 0)
		{
			m_Ranged = true;
		}
	}

	void ConfigInt::SetData(int data)
	{
		if (m_Ranged)
		{
			if (data > m_Max)
			{
				LOG_CORE_WARN("Tried to set Config value of {0} to {1} more than max value of {2}", m_Name, data, m_Max);
				return;
			}

			if (data < m_Min)
			{
				LOG_CORE_WARN("Tried to set Config value of {0} to {1} less than min value of {2}", m_Name, data, m_Min);
				return;
			}
		}

		m_Data = data;
	}

	ConfigString::ConfigString(std::string& dataReference, const char* name, const char* description)
		: m_Data(dataReference), m_Name(name), m_Description(description), m_NameHash(Utility::GenerateHash(name))
	{
	}

	void ConfigManager::Register(const UInt32 groupHash, const ConfigVariant& configVar)
	{
		m_ConfigGroupMap[groupHash][std::visit([](auto&& arg) -> UInt32 { return arg.GetNameHash(); }, configVar)] = std::make_shared<ConfigVariant>(configVar);
		// LOG_CORE_INFO("Added Config with Hash {0} in Group {1}",
		// 			  std::visit([](auto&& arg) -> const std::string& { return arg.GetName(); }, configVar),
		// 			  groupHash);
	}

	void ConfigManager::SetConfigInt(const char* groupName, const char* configName, int value)
	{
		auto groupHash = Utility::GenerateHash(groupName);
		QMBT_CORE_ASSERT(m_ConfigGroupMap.find(groupHash) != m_ConfigGroupMap.end(), "Group does not exist!");

		auto nameHash = Utility::GenerateHash(configName);
		QMBT_CORE_ASSERT(m_ConfigGroupMap[groupHash].find(nameHash) != m_ConfigGroupMap[groupHash].end(), "Config variable doesnot exist!");

		std::get<ConfigInt>(*m_ConfigGroupMap[groupHash][nameHash]).SetData(value);
	}

	void ConfigManager::SetConfigString(const char* groupName, const char* configName, const std::string& value)
	{
		auto groupHash = Utility::GenerateHash(groupName);
		QMBT_CORE_ASSERT(m_ConfigGroupMap.find(groupHash) != m_ConfigGroupMap.end(), "Group does not exist!");

		auto nameHash = Utility::GenerateHash(configName);
		QMBT_CORE_ASSERT(m_ConfigGroupMap[groupHash].find(nameHash) != m_ConfigGroupMap[groupHash].end(), "Config variable doesnot exist!");

		std::get<ConfigString>(*m_ConfigGroupMap[groupHash][nameHash]).SetData(value);
	}
} // namespace QMBT
