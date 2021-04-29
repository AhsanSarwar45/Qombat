#pragma once

#include <QMBTPCH.hpp>

#include "Core/Aliases.hpp"
#include "Core/Core.hpp"
#include "Utility/Hashing.hpp"

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

		static void SetConfigInt(const char* groupName, const char* configName, const int value);
		static void SetConfigString(const char* groupName, const char* configName, const std::string& value);

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

} // namespace QMBT