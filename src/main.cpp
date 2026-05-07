#include <json/parser.hpp>
#include <json/serializer.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;

static const fs::path DATA_FILE = "data/records.json";

// ── 파일 I/O ────────────────────────────────────────────────────────────────

static json::JsonValue load_records() {
    if (!fs::exists(DATA_FILE))
        return json::JsonValue{json::Array{}};
    return json::Parser::load(DATA_FILE);
}

static void save_records(const json::JsonValue& records) {
    fs::create_directories(DATA_FILE.parent_path());
    json::Serializer::save(records, DATA_FILE, 2);
}

// ── 유틸리티 ────────────────────────────────────────────────────────────────

static int next_id(const json::JsonValue& records) {
    int max_id = 0;
    for (const auto& rec : records.as_array()) {
        int id = static_cast<int>(rec["id"].as_number());
        if (id > max_id) max_id = id;
    }
    return max_id + 1;
}

static std::string prompt(const std::string& msg) {
    std::cout << msg;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

static void print_record(const json::JsonValue& rec) {
    std::cout << "  [ID " << static_cast<int>(rec["id"].as_number()) << "]\n"
              << "  이름   : " << rec["name"].as_string()  << '\n'
              << "  전화   : " << rec["phone"].as_string() << '\n'
              << "  이메일 : " << rec["email"].as_string() << '\n';
}

static void print_separator() {
    std::cout << "  ----------------------------------------\n";
}

// ── CRUD 함수 ────────────────────────────────────────────────────────────────

static void do_create(json::JsonValue& records) {
    std::cout << "\n[새 레코드 추가]\n";
    auto name  = prompt("  이름   : ");
    if (name.empty()) { std::cout << "  이름은 필수입니다.\n"; return; }
    auto phone = prompt("  전화   : ");
    auto email = prompt("  이메일 : ");

    json::Object rec;
    rec["id"]    = json::JsonValue{next_id(records)};
    rec["name"]  = json::JsonValue{std::move(name)};
    rec["phone"] = json::JsonValue{std::move(phone)};
    rec["email"] = json::JsonValue{std::move(email)};

    records.as_array().emplace_back(std::move(rec));
    save_records(records);
    std::cout << "  저장 완료.\n";
}

static void do_read(const json::JsonValue& records) {
    const auto& arr = records.as_array();
    if (arr.empty()) { std::cout << "\n  등록된 데이터가 없습니다.\n"; return; }

    std::cout << "\n[조회]\n"
              << "  1. 전체 목록\n"
              << "  2. ID로 검색\n"
              << "  3. 이름으로 검색\n";
    auto choice = prompt("  선택 : ");

    if (choice == "1") {
        std::cout << '\n';
        for (const auto& rec : arr) { print_separator(); print_record(rec); }
        print_separator();

    } else if (choice == "2") {
        int target = std::stoi(prompt("  검색 ID : "));
        for (const auto& rec : arr) {
            if (static_cast<int>(rec["id"].as_number()) == target) {
                print_separator(); print_record(rec); print_separator();
                return;
            }
        }
        std::cout << "  ID " << target << " 를 찾을 수 없습니다.\n";

    } else if (choice == "3") {
        auto kw = prompt("  이름 검색어 : ");
        bool found = false;
        for (const auto& rec : arr) {
            if (rec["name"].as_string().find(kw) != std::string::npos) {
                print_separator(); print_record(rec);
                found = true;
            }
        }
        if (found) print_separator();
        else std::cout << "  '" << kw << "' 에 해당하는 결과가 없습니다.\n";

    } else {
        std::cout << "  잘못된 선택입니다.\n";
    }
}

static void do_update(json::JsonValue& records) {
    std::cout << "\n[데이터 수정]\n";
    int target = std::stoi(prompt("  수정할 ID : "));

    for (auto& rec : records.as_array()) {
        if (static_cast<int>(rec["id"].as_number()) != target) continue;

        std::cout << "\n[현재 데이터]\n";
        print_separator(); print_record(rec); print_separator();
        std::cout << "  빈 줄 입력 시 기존 값 유지\n";

        auto name  = prompt("  이름   [" + rec["name"].as_string()  + "] : ");
        auto phone = prompt("  전화   [" + rec["phone"].as_string() + "] : ");
        auto email = prompt("  이메일 [" + rec["email"].as_string() + "] : ");

        if (!name.empty())  rec["name"]  = json::JsonValue{std::move(name)};
        if (!phone.empty()) rec["phone"] = json::JsonValue{std::move(phone)};
        if (!email.empty()) rec["email"] = json::JsonValue{std::move(email)};

        save_records(records);
        std::cout << "  수정 완료.\n";
        return;
    }
    std::cout << "  ID " << target << " 를 찾을 수 없습니다.\n";
}

static void do_delete(json::JsonValue& records) {
    std::cout << "\n[데이터 삭제]\n";
    int target = std::stoi(prompt("  삭제할 ID : "));

    auto& arr = records.as_array();
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        if (static_cast<int>((*it)["id"].as_number()) != target) continue;

        std::cout << "\n[삭제 대상]\n";
        print_separator(); print_record(*it); print_separator();
        auto confirm = prompt("  삭제하시겠습니까? (y/N) : ");
        if (confirm == "y" || confirm == "Y") {
            arr.erase(it);
            save_records(records);
            std::cout << "  삭제 완료.\n";
        } else {
            std::cout << "  취소되었습니다.\n";
        }
        return;
    }
    std::cout << "  ID " << target << " 를 찾을 수 없습니다.\n";
}

// ── main ─────────────────────────────────────────────────────────────────────

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    auto records = load_records();

    while (true) {
        std::cout << "\n==============================\n"
                  << "  CRUD 메뉴\n"
                  << "==============================\n"
                  << "  1. Create  (추가)\n"
                  << "  2. Read    (조회)\n"
                  << "  3. Update  (수정)\n"
                  << "  4. Delete  (삭제)\n"
                  << "  0. 종료\n"
                  << "------------------------------\n";
        auto choice = prompt("  선택 : ");

        try {
            if      (choice == "1") do_create(records);
            else if (choice == "2") do_read(records);
            else if (choice == "3") do_update(records);
            else if (choice == "4") do_delete(records);
            else if (choice == "0") break;
            else std::cout << "  잘못된 선택입니다.\n";
        } catch (const std::exception& e) {
            std::cout << "  [오류] " << e.what() << '\n';
        }
    }

    return 0;
}
