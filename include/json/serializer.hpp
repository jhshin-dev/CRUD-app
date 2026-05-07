#pragma once
#include "json_export.hpp"
#include "json_value.hpp"
#include <filesystem>
#include <string>

namespace json {

class JSON_API Serializer {
public:
    static std::string stringify(const JsonValue& value, int indent = -1);
    static void save(const JsonValue& value,
                     const std::filesystem::path& path,
                     int indent = 2);
};

} // namespace json
