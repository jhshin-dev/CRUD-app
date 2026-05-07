#include <json/parser.hpp>
#include <charconv>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace json {

namespace {

class ParseContext {
public:
    explicit ParseContext(std::string_view text) : text_(text), pos_(0) {}

    JsonValue parse() {
        skip_ws();
        auto result = parse_value();
        skip_ws();
        if (pos_ != text_.size())
            throw std::runtime_error("unexpected trailing content");
        return result;
    }

private:
    std::string_view text_;
    std::size_t pos_;

    char peek() const {
        if (pos_ >= text_.size()) throw std::runtime_error("unexpected end of input");
        return text_[pos_];
    }

    char consume() {
        if (pos_ >= text_.size()) throw std::runtime_error("unexpected end of input");
        return text_[pos_++];
    }

    void expect(char c) {
        char got = consume();
        if (got != c)
            throw std::runtime_error(std::string("expected '") + c + "' but got '" + got + "'");
    }

    void skip_ws() {
        while (pos_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[pos_])))
            ++pos_;
    }

    JsonValue parse_value() {
        skip_ws();
        char c = peek();
        if (c == 'n')  return parse_null();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == '"') return JsonValue{parse_raw_string()};
        if (c == '[') return parse_array();
        if (c == '{') return parse_object();
        return parse_number();
    }

    JsonValue parse_null() {
        for (char c : std::string_view("null")) expect(c);
        return JsonValue{};
    }

    JsonValue parse_bool() {
        if (peek() == 't') {
            for (char c : std::string_view("true")) expect(c);
            return JsonValue{true};
        }
        for (char c : std::string_view("false")) expect(c);
        return JsonValue{false};
    }

    JsonValue parse_number() {
        const char* start = text_.data() + pos_;
        double val{};
        auto [ptr, ec] = std::from_chars(start, text_.data() + text_.size(), val);
        if (ec != std::errc{})
            throw std::runtime_error("invalid number near position " + std::to_string(pos_));
        pos_ = static_cast<std::size_t>(ptr - text_.data());
        return JsonValue{val};
    }

    std::string parse_raw_string() {
        expect('"');
        std::string result;
        while (true) {
            char c = consume();
            if (c == '"') break;
            if (c != '\\') { result += c; continue; }
            char esc = consume();
            switch (esc) {
            case '"':  result += '"';  break;
            case '\\': result += '\\'; break;
            case '/':  result += '/';  break;
            case 'b':  result += '\b'; break;
            case 'f':  result += '\f'; break;
            case 'n':  result += '\n'; break;
            case 'r':  result += '\r'; break;
            case 't':  result += '\t'; break;
            default:
                throw std::runtime_error(std::string("unknown escape: \\") + esc);
            }
        }
        return result;
    }

    JsonValue parse_array() {
        expect('[');
        Array arr;
        skip_ws();
        if (peek() == ']') { consume(); return JsonValue{std::move(arr)}; }
        while (true) {
            arr.push_back(parse_value());
            skip_ws();
            if (peek() == ']') { consume(); break; }
            expect(',');
        }
        return JsonValue{std::move(arr)};
    }

    JsonValue parse_object() {
        expect('{');
        Object obj;
        skip_ws();
        if (peek() == '}') { consume(); return JsonValue{std::move(obj)}; }
        while (true) {
            skip_ws();
            auto key = parse_raw_string();
            skip_ws();
            expect(':');
            obj[key] = parse_value();
            skip_ws();
            if (peek() == '}') { consume(); break; }
            expect(',');
        }
        return JsonValue{std::move(obj)};
    }
};

} // namespace

JsonValue Parser::parse(std::string_view text) {
    ParseContext ctx(text);
    return ctx.parse();
}

JsonValue Parser::load(const std::filesystem::path& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("cannot open: " + path.string());
    std::ostringstream ss;
    ss << f.rdbuf();
    return parse(ss.str());
}

} // namespace json
