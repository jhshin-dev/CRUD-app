#include <json/serializer.hpp>
#include <array>
#include <charconv>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace json {

namespace {

void write_string(std::ostringstream& out, const std::string& s) {
    out << '"';
    for (unsigned char c : s) {
        switch (c) {
        case '"':  out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\b': out << "\\b";  break;
        case '\f': out << "\\f";  break;
        case '\n': out << "\\n";  break;
        case '\r': out << "\\r";  break;
        case '\t': out << "\\t";  break;
        default:   out << static_cast<char>(c); break;
        }
    }
    out << '"';
}

void write_indent(std::ostringstream& out, int indent, int depth) {
    if (indent < 0) return;
    out << '\n';
    for (int i = 0; i < indent * depth; ++i) out << ' ';
}

void write_value(std::ostringstream& out, const JsonValue& val, int indent, int depth) {
    std::visit([&](auto&& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, Null>) {
            out << "null";
        } else if constexpr (std::is_same_v<T, Bool>) {
            out << (v ? "true" : "false");
        } else if constexpr (std::is_same_v<T, Number>) {
            std::array<char, 32> buf{};
            auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), v);
            if (ec == std::errc{})
                out.write(buf.data(), ptr - buf.data());
            else
                out << v;
        } else if constexpr (std::is_same_v<T, String>) {
            write_string(out, v);
        } else if constexpr (std::is_same_v<T, Array>) {
            out << '[';
            for (std::size_t i = 0; i < v.size(); ++i) {
                if (i > 0) out << ',';
                write_indent(out, indent, depth + 1);
                write_value(out, v[i], indent, depth + 1);
            }
            if (!v.empty()) write_indent(out, indent, depth);
            out << ']';
        } else if constexpr (std::is_same_v<T, Object>) {
            out << '{';
            bool first = true;
            for (auto& [k, v2] : v) {
                if (!first) out << ',';
                first = false;
                write_indent(out, indent, depth + 1);
                write_string(out, k);
                out << ':';
                if (indent >= 0) out << ' ';
                write_value(out, v2, indent, depth + 1);
            }
            if (!first) write_indent(out, indent, depth);
            out << '}';
        }
    }, val.variant());
}

} // namespace

std::string Serializer::stringify(const JsonValue& value, int indent) {
    std::ostringstream ss;
    write_value(ss, value, indent, 0);
    return ss.str();
}

void Serializer::save(const JsonValue& value,
                      const std::filesystem::path& path,
                      int indent) {
    std::ofstream f(path);
    if (!f) throw std::runtime_error("cannot write: " + path.string());
    f << stringify(value, indent);
}

} // namespace json
