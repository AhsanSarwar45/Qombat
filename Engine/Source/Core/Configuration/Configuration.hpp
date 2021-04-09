#pragma once
#include <QMBTPCH.hpp>

#include "Core/Configuration/ConfigManager.hpp"
#include "Utility/Hashing.hpp"

namespace QMBT
{
#define CONFIG_GROUP(groupName, ...) ConfigManager::Register(GenerateHash(groupName), __VA_ARGS__);

#define CONFIG_INT(var, name, description, min, max) ConfigVariant(ConfigInt{var, name, description, min, max})
#define CONFIG_STRING(var, name, description) ConfigVariant(ConfigString{var, name, description})

} // namespace QMBT