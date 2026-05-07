#include <json/json_value.hpp>
#include <stdexcept>

namespace json {

JsonValue::JsonValue(bool v)        : v_(v) {}
JsonValue::JsonValue(double v)      : v_(v) {}
JsonValue::JsonValue(int v)         : v_(static_cast<double>(v)) {}
JsonValue::JsonValue(const char* v) : v_(std::string(v)) {}
JsonValue::JsonValue(std::string v) : v_(std::move(v)) {}
JsonValue::JsonValue(Array v)       : v_(std::move(v)) {}
JsonValue::JsonValue(Object v)      : v_(std::move(v)) {}

bool JsonValue::is_null()   const noexcept { return std::holds_alternative<Null>(v_); }
bool JsonValue::is_bool()   const noexcept { return std::holds_alternative<Bool>(v_); }
bool JsonValue::is_number() const noexcept { return std::holds_alternative<Number>(v_); }
bool JsonValue::is_string() const noexcept { return std::holds_alternative<String>(v_); }
bool JsonValue::is_array()  const noexcept { return std::holds_alternative<Array>(v_); }
bool JsonValue::is_object() const noexcept { return std::holds_alternative<Object>(v_); }

bool JsonValue::as_bool() const {
    if (!is_bool()) throw std::runtime_error("JsonValue is not a bool");
    return std::get<Bool>(v_);
}
double JsonValue::as_number() const {
    if (!is_number()) throw std::runtime_error("JsonValue is not a number");
    return std::get<Number>(v_);
}
const std::string& JsonValue::as_string() const {
    if (!is_string()) throw std::runtime_error("JsonValue is not a string");
    return std::get<String>(v_);
}
const Array& JsonValue::as_array() const {
    if (!is_array()) throw std::runtime_error("JsonValue is not an array");
    return std::get<Array>(v_);
}
const Object& JsonValue::as_object() const {
    if (!is_object()) throw std::runtime_error("JsonValue is not an object");
    return std::get<Object>(v_);
}
Array& JsonValue::as_array() {
    if (!is_array()) throw std::runtime_error("JsonValue is not an array");
    return std::get<Array>(v_);
}
Object& JsonValue::as_object() {
    if (!is_object()) throw std::runtime_error("JsonValue is not an object");
    return std::get<Object>(v_);
}

JsonValue& JsonValue::operator[](std::size_t idx) {
    return as_array().at(idx);
}
const JsonValue& JsonValue::operator[](std::size_t idx) const {
    return as_array().at(idx);
}
JsonValue& JsonValue::operator[](const std::string& key) {
    return as_object()[key];
}
const JsonValue& JsonValue::operator[](const std::string& key) const {
    return as_object().at(key);
}
JsonValue& JsonValue::operator[](const char* key) {
    return as_object()[key];
}
const JsonValue& JsonValue::operator[](const char* key) const {
    return as_object().at(key);
}

JsonValue::Variant& JsonValue::variant() noexcept { return v_; }
const JsonValue::Variant& JsonValue::variant() const noexcept { return v_; }

} // namespace json
