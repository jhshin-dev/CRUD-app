# Commit Convention

## Language

All commit messages must be written in **English**.

## Format

```
[<type>] <subject>

[body]

[footer]
```

- **Header** (`[<type>] <subject>`): required
- **Body**: optional — explain *why*, not *what*
- **Footer**: optional — reference issues or breaking changes

## Types

| Type | When to use |
|------|-------------|
| `[feat]` | New feature |
| `[fix]` | Bug fix |
| `[refactor]` | Code change that neither fixes a bug nor adds a feature |
| `[docs]` | Documentation only changes |
| `[style]` | Formatting, missing semicolons, etc. (no logic change) |
| `[test]` | Adding or correcting tests |
| `[chore]` | Build process, dependency updates, project config |
| `[perf]` | Performance improvement |
| `[revert]` | Revert a previous commit |

## Rules

1. Use `[ ]` brackets for the header type — e.g., `[fix]`, `[feat]`
2. Write the subject in **lowercase**, imperative mood (e.g., `add`, `fix`, `remove`)
3. Do **not** end the subject line with a period
4. Keep the subject line under **72 characters**
5. Separate subject from body with a **blank line**
6. Use the body to explain *what* changed and *why*, not *how*

## Examples

```
[feat] add JSON-based record persistence

[fix] prevent crash when data file is missing

[fix] emergency fix for null pointer in parser

[refactor] extract parse_number into ParseContext

[docs] add commit convention guide

[chore] add JSON_STATIC preprocessor to all configurations
```
