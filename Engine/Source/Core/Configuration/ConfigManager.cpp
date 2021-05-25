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
	}

} // namespace QMBT
