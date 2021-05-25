#pragma once

#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"
#include "Core/Core.hpp"
#include "Utility/Hashing.hpp"
#include "Utility/Types.hpp"

namespace QMBT
{

	class ConfigInt
	{
	  public:
		ConfigInt(int& dataReference, const char* name, const char* description, int min = 0, int max = 0);
		//ConfigInt(const ConfigInt& other) = delete;
		//ConfigInt(ConfigInt&& other) noexcept;

		inline int GetData() const { return m_Data; }
		inline const std::string& GetName() const { return m_Name; }
		inline UInt32 GetNameHash() const { return m_NameHash; }

		void SetData(int data);

	  private:
		int& m_Data;
		std::string m_Name;
		std::string m_Description;
		UInt32 m_NameHash;

		bool m_Ranged;

		int m_Max;
		int m_Min;
	};

	class ConfigString
	{
	  public:
		ConfigString(std::string& dataReference, const char* name, const char* description);
		//ConfigString(const ConfigString& other) = delete;

		inline const std::string& GetData() const
		{
			return m_Data;
		}
		inline const std::string& GetName() const { return m_Name; }
		inline UInt32 GetNameHash() const { return m_NameHash; }

		inline void SetData(const std::string& data) { m_Data = data; }

	  private:
		std::string& m_Data;
		std::string m_Name;
		std::string m_Description;
		UInt32 m_NameHash;
	};

	using ConfigVariant = std::variant<ConfigInt, ConfigString>;
	using ConfigMap = std::unordered_map<UInt32, std::shared_ptr<ConfigVariant>>;
	using ConfigGroupMap = std::unordered_map<UInt32, ConfigMap>;

	class ConfigManager
	{
	  public:
		ConfigManager();  // Read Files
		~ConfigManager(); // Write Files

		template <typename... Args>
		static void Register(const UInt32 groupHash, const ConfigVariant& configVar, Args&&... configVars);
		static void Register(const UInt32 groupHash, const ConfigVariant& configVar);

		/**
		 * @brief Sets a Config Setting to a value
		 * 
		 * @tparam T The type of the value
		 * @param groupName The group in which the config setting is stored
		 * @param configName The name of the config setting
		 * @param value The value to which the config setting needs to be set
		 */
		template <typename T>
		static void SetConfig(const char* groupName, const char* configName, T value);

	  private:
		static inline ConfigGroupMap m_ConfigGroupMap;
	};

	template <typename... Args>
	void ConfigManager::Register(const UInt32 groupHash, const ConfigVariant& configVar, Args&&... configVars)
	{
		m_ConfigGroupMap[groupHash][std::visit([](auto&& arg) -> UInt32 { return arg.GetNameHash(); }, configVar)] = std::make_shared<ConfigVariant>(configVar);
		;
		LOG_CORE_INFO("Added Config with Hash {0} in Group {1}",
					  std::visit([](auto&& arg) -> const std::string& { return arg.GetName(); }, configVar),
					  groupHash);
		Register(groupHash, configVars...);
	}

	template <typename T>
	void ConfigManager::SetConfig(const char* groupName, const char* configName, T value)
	{
		auto groupHash = Utility::GenerateHash(groupName);
		QMBT_CORE_ASSERT(m_ConfigGroupMap.find(groupHash) != m_ConfigGroupMap.end(), "Group does not exist!");

		auto nameHash = Utility::GenerateHash(configName);
		QMBT_CORE_ASSERT(m_ConfigGroupMap[groupHash].find(nameHash) != m_ConfigGroupMap[groupHash].end(), "Config variable doesnot exist!");

		try
		{
			if constexpr (std::is_same<T, int>::value)
			{
				std::get<ConfigInt>(*m_ConfigGroupMap[groupHash][nameHash]).SetData(value);
			}
			else if constexpr (std::is_same<T, std::string>::value or std::is_same<T, const char*>::value)
			{
				std::get<ConfigString>(*m_ConfigGroupMap[groupHash][nameHash]).SetData(value);
			}
			else
			{
				LOG_CORE_ERROR("The config type is not supported yet!");
			}
		}
		// If the passed value type cannot be used to set the value of the config
		catch (std::bad_variant_access const& exception)
		{
			LOG_CORE_ERROR("{0}: Wrong config type!", exception.what());
		}
	}

} // namespace QMBT