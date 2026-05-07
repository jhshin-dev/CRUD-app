# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

JSON 파일을 영속성 계층으로 사용하는 C++20 콘솔 CRUD 애플리케이션. MSBuild / Visual Studio 2022, Windows 10 x64 타겟. 외부 라이브러리 없이 표준 라이브러리만 사용하는 JSON 파서/직렬화기를 내장한다.

참조 POC: [jhshin-dev/JSON-poc](https://github.com/jhshin-dev/JSON-poc) — 동일한 JSON 엔진 구조(파서, 직렬화기, JsonValue)를 기반으로 한다.

## Build Commands

```powershell
# 전체 솔루션 빌드 (Debug x64)
msbuild CRUD-app.slnx /p:Configuration=Debug /p:Platform=x64

# Release 빌드
msbuild CRUD-app.slnx /p:Configuration=Release /p:Platform=x64

# 프로젝트만 빌드
msbuild CRUD-app.vcxproj /p:Configuration=Debug /p:Platform=x64

# 실행 (빌드 후)
.\x64\Debug\CRUD-app.exe
```

## Architecture

### 디렉토리 구조 (목표)

```
CRUD-app/
├── include/json/          # JSON 엔진 헤더 (POC에서 이식)
│   ├── json_value.hpp     # JsonValue 타입 정의
│   ├── parser.hpp         # Parser 클래스 선언
│   └── serializer.hpp     # Serializer 클래스 선언
├── src/
│   ├── json/              # JSON 엔진 구현 (POC에서 이식)
│   │   ├── json_value.cpp
│   │   ├── parser.cpp
│   │   └── serializer.cpp
│   └── main.cpp           # CRUD 애플리케이션 진입점
└── data/
    └── records.json       # 런타임 데이터 파일
```

### JSON 엔진 핵심 타입

POC 코드에서 이식하며, `JSON_STATIC` 매크로를 정의해 정적 링크로 사용한다(DLL 불필요).

| 타입 | 역할 |
|------|------|
| `json::JsonValue` | `std::variant<Null, Bool, Number, String, Array, Object>` — 6개 JSON 타입 통합 표현 |
| `json::Parser` | `parse(string_view)` / `load(path)` — 단일 패스 재귀 하강 파서 |
| `json::Serializer` | `stringify(value, indent)` / `save(value, path, indent)` — 콤팩트·들여쓰기 출력 |

`json::Array` = `std::vector<JsonValue>`, `json::Object` = `std::map<std::string, JsonValue>`

### CRUD 데이터 흐름

```
애플리케이션 시작
  └─► Parser::load("data/records.json") → JsonValue (메모리 내 객체 그래프)

사용자 명령 처리 (루프)
  ├─ Create: Object 구성 → Array에 push_back → Serializer::save()
  ├─ Read:   JsonValue 순회 → 콘솔 출력
  ├─ Update: 인덱스/키로 요소 찾기 → 값 교체 → Serializer::save()
  └─ Delete: Array에서 요소 제거 → Serializer::save()
```

파일은 매 쓰기 연산마다 즉시 저장한다(전체 덮어쓰기 방식).

### 숫자 처리 주의사항

POC와 동일하게 `std::from_chars` / `std::to_chars`를 사용해 최단 왕복 표현을 보장한다. ID 같은 정수 필드는 `double`로 저장되므로 읽을 때 `static_cast<int>(value.as_number())`로 변환한다.

## Git Commit

커밋 메시지 작성 시 [`docs/COMMIT_CONVENTION.md`](docs/COMMIT_CONVENTION.md) 를 반드시 따른다.

핵심 규칙:
- 메시지는 **영어**로 작성한다
- 헤더 타입은 `[ ]` 대괄호로 감싼다 — 예: `[feat] add search by name`, `[fix] emergency fix`
- subject는 소문자 명령형, 마침표 없음, 72자 이내

## Preprocessor Definitions

프로젝트에 `JSON_STATIC` 전처리기를 추가해야 JSON_API 매크로가 빈 문자열로 확장된다(POC의 `json_export.hpp` 참조).

```xml
<PreprocessorDefinitions>JSON_STATIC;_DEBUG;_CONSOLE;...</PreprocessorDefinitions>
```
