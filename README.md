# CRUD-app

JSON 파일을 영속성 계층으로 사용하는 C++20 콘솔 CRUD 애플리케이션.

외부 라이브러리 없이 C++ 표준 라이브러리만으로 구현한 JSON 파서/직렬화기를 내장하며, 연락처 데이터를 `data/records.json` 파일에 저장·관리합니다.

---

## Features

| 기능 | 설명 |
|------|------|
| **Create** | 이름·전화·이메일 입력 → ID 자동 부여 후 JSON 저장 |
| **Read** | 전체 목록 보기 / ID 검색 / 이름 부분 검색 |
| **Update** | ID 선택 → 필드별 수정 (빈 줄 입력 시 기존 값 유지) |
| **Delete** | ID 선택 → 내용 미리보기 → 확인 후 삭제 |

---

## Requirements

- Windows 10 x64
- Visual Studio 2022 (v145 toolset)
- C++20

---

## Build

```powershell
# Visual Studio에서 열기
start CRUD-app.slnx

# 또는 MSBuild로 직접 빌드
$msbuild = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" `
    -latest -requires Microsoft.Component.MSBuild -property installationPath
& "$msbuild\MSBuild\Current\Bin\MSBuild.exe" CRUD-app.vcxproj /p:Configuration=Debug /p:Platform=x64
```

빌드 결과물: `x64\Debug\CRUD-app.exe`

---

## Run

```powershell
.\x64\Debug\CRUD-app.exe
```

실행 시 `data/records.json` 이 없으면 자동 생성됩니다.

```
==============================
  CRUD 메뉴
==============================
  1. Create  (추가)
  2. Read    (조회)
  3. Update  (수정)
  4. Delete  (삭제)
  0. 종료
------------------------------
  선택 :
```

---

## Data Format

레코드는 JSON 배열로 저장됩니다.

```json
[
  {
    "email": "hong@example.com",
    "id": 1,
    "name": "홍길동",
    "phone": "010-1234-5678"
  }
]
```

---

## Project Structure

```
CRUD-app/
├── include/json/          # JSON 엔진 헤더
│   ├── json_export.hpp    # DLL/Static 익스포트 매크로
│   ├── json_value.hpp     # JsonValue (std::variant 기반)
│   ├── parser.hpp         # Parser 선언
│   └── serializer.hpp     # Serializer 선언
├── src/
│   ├── json/              # JSON 엔진 구현
│   │   ├── json_value.cpp
│   │   ├── parser.cpp
│   │   └── serializer.cpp
│   └── main.cpp           # CRUD 애플리케이션 진입점
├── docs/
│   └── COMMIT_CONVENTION.md
├── CLAUDE.md
└── CRUD-app.vcxproj
```

> JSON 엔진은 [jhshin-dev/JSON-poc](https://github.com/jhshin-dev/JSON-poc) 구조를 기반으로 이식했습니다.

---

## License

MIT
