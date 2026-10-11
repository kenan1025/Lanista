# Lanista project instructions

These rules apply only to this repository.

## Default workflow: Ponytail full

- Use the `ponytail` skill in **full** mode for every request in this project, even when the user does not explicitly mention it. Read `.hermes/skills/ponytail/SKILL.md` before acting; if the skill tool cannot resolve it, load that file directly.
- A later explicit user instruction can change the level or disable Ponytail for the current session. Otherwise, new project sessions default to full.
- Make the smallest complete change. Preserve validation, security, error handling and verification; do not add unrequested abstractions or features.
- End completed-work replies with a brief statement of what was not checked or changed and any relevant remaining risk.

## Project-local skills

Skills are installed in `.hermes/skills/` for Hermes. Do not install or update these globally without explicit permission. `skills-lock.json` records their sources.

Load the relevant installed skills for Unreal work:

- `ue-project-context`
- `ue-cpp-foundations`
- `ue-actor-component-architecture`
- `ue-blueprint-cpp-interop`
- `ue-gameplay-framework`
- `ue-testing-debugging`
- `ue-ui-umg-slate`

Use `find-skills` when discovering additional skills. Before using Unreal guidance, read `.agents/ue-project-context.md` if present; if absent, inspect the project's actual files and do not invent project details. Installing the context skill does not itself generate that document.

## Preserve gameplay and user work

- Keep `Lanista_Main_Level` unless the user explicitly requests otherwise.
- Do not delete character, first-person, movement, input, animation dependencies or C++ gameplay source as incidental cleanup.
- Preserve unrelated changes and unsaved editor work. Do not commit, push or rewrite Git history unless requested.
