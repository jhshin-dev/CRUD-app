#pragma once
#include "json_export.hpp"
#include "json_value.hpp"
#include <filesystem>
#include <string_view>

namespace json {

class JSON_API Parser {
public:
    static JsonValue parse(std::string_view text);
    static JsonValue load(const std::filesystem::path& path);
};

} // namespace json
