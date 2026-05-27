# TODO

Living queue of work toward v1.0. The MVP is **launch a binary, step through it, see source/stack/locals update.**

**Rules:**
- One feature in flight. Finish the current branch before starting another.
- Items below should be concrete enough to start cold. If you're re-deliberating at the keyboard, the item isn't ready — refine it here first.
- "Tech debt" earns a line only if it blocks a future item or is actively crashing.

---

## In flight: `feat/next-continue`

Scope down to these four. Everything else moves to *Next up*.

- [ ] Wire `Continue` and `Step` into `Act`'s visitor (`src/debugplus/debug_session.cpp:72-87`). The branch is misnamed until this is done.
- [ ] Null-guard `const char*` returns in `PrintState` (`debug_session.cpp:102, 107`). `SBFrame::GetFunctionName()` and `SBFileSpec::GetDirectory()/GetFilename()` can return `nullptr`; passing to `std::string` is UB. Top suspect for the segfaults.
- [ ] Guard `Next`/`Step`/`Continue` against an invalid or terminal `_process`. Print a one-line message ("no process running" / "process has exited") instead of calling LLDB on a null handle.
- [ ] Print *something* when `_process.GetState()` is `eStateExited` or crashed. Silent termination feels broken.

---

## Next up (after merging `feat/next-continue`)

Pick the top item. Do not re-deliberate.

### REPL polish (one branch)
- [ ] User feedback on `Unknown` and `monostate` actions in `Act`. Currently silently swallowed.
- [ ] Breakpoint confirmation: `Break` (`debug_session.cpp:44-53`, the `TODO` comment) needs to report where the breakpoint actually bound (file:line) — and warn when no location resolved.
- [ ] Add `default:` case to `debugplus::ToString(StopReason)` (`lldb_strings.h:43`). The function currently has no return path if LLDB ever adds an enum value.

### Toward MVP: stack and locals
Source is partially there. Stack and locals are not started — the MVP demand is all three.

- [ ] Display selected frame's locals when stopped (`SBFrame::GetVariables`). Wire the existing `Print` action at the same time.
- [ ] Display the call stack when stopped (`SBThread::GetFrameAtIndex` over `GetNumFrames`).

### Tests for the `liblldb` wrapper
Per `CLAUDE.md`, this is the most fragile area and the one that most needs coverage.

- [ ] `DebugSession` tests against a small fixture binary in `tests/fixtures/`. Cover: launch, step, breakpoint binds, process exit, invalid-state commands don't crash.

---

## Deferred (don't start until everything above is done)

- **State-aware command dispatch.** A predicate per action declaring which `lldb::StateType`s it's valid in. Only worth doing once the inline guards above prove repetitive — don't pre-build it.
- **Double-`run` handling.** What happens if `run` is typed while a process is alive? Probably kill and relaunch with a message. Low priority.
- **Async event loop with `SBListener`.** Currently `SetAsync(false)`. CLAUDE.md flags this as a deliberate "revisit later." Don't touch until there is a concrete reason.
- **TUI (`ftxui`) replacing the `cout` REPL.** The big v1.0 push. Defer until the REPL has the behaviors above; you will port them.
