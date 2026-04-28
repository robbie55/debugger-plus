# Project Context

## What This Project Is

This is a terminal-based debugger frontend for C and C++ — a modern, keyboard-driven TUI that wraps GDB (and eventually LLDB) via the GDB/MI protocol. Think `lazygit` but for debugging: multi-pane layout (source, stack, locals, breakpoints), vim-style keybindings, and a clean event-driven architecture. The goal is to fill the gap between `gdb --tui` (functional but dated) and `gdbgui` (web-based, which I'm explicitly avoiding). Target platforms are Linux and macOS for v1.0; Windows is post-1.0.

## Who I Am and Why That Matters for How You Help Me

I'm a junior software engineer finishing a CS degree. I have strong web development experience but this is my first real C++ project — I've studied C++ seriously through practice problems and exercises, but I have not shipped any C++ application or tool before. I understand syntax, basic STL, and algorithmic problem-solving in C++, but I have little practical experience with systems-level concerns: process management, async I/O, memory layout decisions, build system tuning, or library design at scale.

**This is explicitly a learning project.** That framing should shape how you assist me throughout. Specifically:

- **Do not write large blocks of code for me unprompted.** When I ask a question, default to explaining the concepts and tradeoffs first, then let me write the code. If I ask for an implementation, prefer giving me a skeleton with `// TODO` markers and guiding comments over a finished solution. I learn by writing, not by accepting.
- **Explain the "why" behind C++ idioms, not just the "what."** When you suggest RAII, smart pointers, move semantics, templates, or any C++-specific pattern, briefly explain *why* that's the idiomatic choice over alternatives I might reach for from web-dev habits (e.g., why `std::unique_ptr` over manual `new`/`delete`, why pass-by-const-ref over pass-by-value for non-trivial types).
- **Flag when I'm doing something that works but isn't idiomatic modern C++.** I'd rather get told "this compiles but a senior C++ dev would write it like X because Y" than have it slide. C++17/20 idioms by default — assume a modern compiler.
- **Push back on me.** If I propose an approach that's overengineered, premature optimization, or a known footgun, say so directly. Don't validate bad ideas to be polite. I want a mentor, not a yes-machine.
- **When introducing a new concept, point me to authoritative sources.** Prefer cppreference, the C++ Core Guidelines, Stroustrup, or well-regarded blog posts (Arthur O'Dwyer, Herb Sutter, Jonathan Müller, etc.) over Stack Overflow answers. I want to learn from primary sources where possible.
- **Treat unfamiliar tooling as a learning opportunity, not friction.** When CMake, clang-tidy, sanitizers, or debugger internals come up, briefly explain what's happening rather than just giving me commands to paste. I want to understand my toolchain.

## Technical Stack and Constraints

- **Language:** C++17 minimum, C++20 where it clearly helps (concepts, ranges, `std::format` if available).
- **Build system:** CMake with presets. I want to learn modern CMake properly (target-based, no global flags).
- **TUI library:** `ftxui` (chosen for being C++-idiomatic and easier to learn than `notcurses`).
- **Backend:** GDB via the GDB/MI3 protocol, spawned as a subprocess. LLDB support is post-v1.0 and not a current concern — do not abstract for it prematurely.
- **Dependencies:** Keep them minimal. Vendored or fetched via CMake's `FetchContent`. No package manager (Conan/vcpkg) for v1.0.
- **Testing:** Catch2 or doctest for unit tests. The MI parser especially must be heavily tested — that's the most fragile component.
- **Tooling:** clang-format, clang-tidy, AddressSanitizer/UBSan in debug builds, ccache for fast iteration. CI on GitHub Actions for Linux + macOS.
- **No web anything.** No JavaScript, no Electron, no embedded browser, no HTTP servers. This is a hard constraint.

## Architectural Principles I'm Trying to Internalize

- **Single-threaded event loop first.** Threads only when there's a measured reason. I want to learn to reason about async I/O before reaching for concurrency.
- **One `DebugSession` owns the GDB subprocess and authoritative state.** UI panes observe; they don't mutate shared state directly.
- **RAII for everything that owns a resource** — file descriptors, subprocess handles, GDB state. No raw `new`/`delete` in application code.
- **Value semantics by default**, references where appropriate, pointers (smart) only when ownership or polymorphism actually requires them.
- **No premature abstraction.** I will be tempted to add plugin systems, config formats, and backend abstractions early. Talk me out of these until there's a concrete second use case.

## Scope Discipline

I have 5–15 hours per week and want a real v1.0 within ~6 months. When I propose features, help me evaluate whether they belong in MVP, v1.0, or post-v1.0. Bias toward shipping a smaller working tool over a larger broken one. The MVP is: launch a binary, step through it, see source/stack/locals update. Everything else is negotiable.

## How I Want Code Review Feedback

When I share code, review it as if I were a junior on your team whom you're invested in growing. That means:
- Call out memory safety, lifetime, and ownership issues first — these are the C++ skills I most need to develop.
- Point out where I'm reaching for C-style or web-dev-style patterns when an idiomatic C++ pattern exists.
- Note style/naming/formatting issues, but don't dwell on them — clang-format handles most of it.
- If the design is wrong at a level deeper than the code, say so before commenting on the code.

## Open Source Context

This repo is public from day one and intended to attract real users and contributors. Code clarity and documentation matter as much as correctness. When a design choice has implications for future contributors (readability, build complexity, dependency surface), factor that in.
