#pragma once
#include "json_export.hpp"
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

class JsonValue;

using Null   = std::monostate;
using Bool   = bool;
using Number = double;
using String = std::string;
using Array  = std::vector<JsonValue>;
using Object = std::map<std::string, JsonValue>;

class JSON_API JsonValue {
public:
    JsonValue() = default;
    explicit JsonValue(bool v);
    explicit JsonValue(double v);
    explicit JsonValue(int v);
    explicit JsonValue(const char* v);
    explicit JsonValue(std::string v);
    explicit JsonValue(Array v);
    explicit JsonValue(Object v);

    bool is_null()   const noexcept;
    bool is_bool()   const noexcept;
    bool is_number() const noexcept;
    bool is_string() const noexcept;
    bool is_array()  const noexcept;
    bool is_object() const noexcept;

    bool               as_bool()   const;
    double             as_number() const;
    const std::string& as_string() const;
    const Array&       as_array()  const;
    const Object&      as_object() const;
    Array&             as_array();
    Object&            as_object();

    JsonValue&       operator[](std::size_t idx);
    const JsonValue& operator[](std::size_t idx) const;
    JsonValue&       operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;
    JsonValue&       operator[](const char* key);
    const JsonValue& operator[](const char* key) const;

    using Variant = std::variant<Null, Bool, Number, String, Array, Object>;
    Variant&       variant() noexcept;
    const Variant& variant() const noexcept;

private:
    Variant v_;
};

} // namespace json
