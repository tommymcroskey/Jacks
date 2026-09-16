# ENG-006: L-002 header and harness handoff

Author: Senior Engineer agent. Date: 2026-09-16. State: ready for risk review;
no apprentice coding release until acceptance.
Branch: `codex/l002-harness`.
Base: `959718f9f4366058392380e2ea48bf0412fc6036`, including accepted assignment
`34a72bc8e7cb85c4962591a8cf2557f756f66078` through PR #15.

## Delivered

- `include/jacks/deposit.hpp`: exact assignment aggregates, errors, variant and
  const-reference signature. Comments state units, initialization, structural
  validation limit, error precedence, valuation reuse and immutable inputs.
- `src/deposit.cpp`: explicitly unimplemented throwing stub only. It neither
  answers Tommy's first case nor implements deposit behavior.
- `tests/deposit_test.cpp`: compile-time signature check and GTEST_SKIP placeholder.
  Tommy owns all actual component tests; the skip is not a passing deposit check.
- CMake adds `jacks_deposit`, linked publicly to accepted `jacks_valuation`, and
  includes the placeholder in the existing GoogleTest executable. Warning policy
  remains identical. No new dependency, framework, generator or shell helper.
- Submission README and build-guide addendum identify paths, unchanged terminal
  commands, configure-after-pull step, and expected 13-pass/1-skip state.

Risk accepted the exact assignment by direct message before this work. The
header/harness itself still needs independent review. Latest coordinator request
froze the assignment-only branch during PR #15 integration; scaffold edits were
kept uncommitted and moved to this fresh branch after that merge. No changes were
pushed onto the frozen assignment branch and no user worktree was touched.

## Verification

Executed `./tools/l001.ps1`: CMake configure, MSVC 19.29.30138.0 x64 Debug
compile/link, and CTest 3.20.21032501-MSVC_2 run. **14 discovered: 13 pass,
1 explicit deposit skip, 0 failures.** The existing 12 valuation tests and one
infrastructure test pass. The new library and test source compile with warnings
as errors. No deposit result is tested or claimed.

The run occurred before moving unchanged scaffold files to this branch; the
intervening main commits changed assignment/status documents only. No runtime
change justified repeating the same run. Staged whitespace and path checks were
performed before commit. The established runner failure-propagation mechanism is
unchanged; earlier evidence remains ENG-004. No new failure-path claim is made.

Only the installed Windows Debug configuration was run. Independent reproduction
and actual deposit behavior remain unverified. Use `docs/build.md`'s existing
runtime invocation if the terminal lacks CMake or PowerShell 7 on PATH. The helper
retains the historical l001 name but builds/tests all current project targets.

## Review and next owner

Risk: review exact head against accepted L-002, reproduce configure/build/test,
verify skipped/throwing placeholders cannot be mistaken for implemented behavior,
and confirm no apprentice solution or new financial scope was introduced.
Engineering supplies fixes only for scaffold findings. Tommy's first case and
implementation/tests remain his work.

Coordinator owns PR transport and integration after review, and coding-release
communication. This handoff does not itself release implementation or merge.
The bounded scaffold can be reverted separately from the accepted assignment.
