# RISK-003: independent L-001 harness review

Reviewer: Risk and Validation agent. Date: 2026-09-15.
Reviewed exact head: `df3d0271b6a933fc3cbfc66cd5cd6deacef85ecb`
on `codex/l001-harness`.

## Disposition

Accept the scaffold and exact API for L-001. No blocking findings. This closes
the harness prerequisite for Tommy to write his first failing test; it does not
accept valuation behavior, the apprentice submission, or the full ledger.
Coordinator owns integration and the recorded release handoff.

## Scope and static inspection

Read CMake, helper, public header, stub, both test files, build guide, assignment,
ENG-004, and the accepted P001 decision at this head. Review ran in my own worktree
`C:/Users/18589/.codex/worktrees/44b5/Jacks`, branch `codex/risk-003-harness-review`,
created directly at the reviewed head from a clean prior review branch. Engineering's
worktree was only read during the preliminary inspection, never modified or built.

- Signature and signed 64-bit units agree with L-001: cash and mark in minor
  currency units, quantity in whole units, and variant equity/error result.
  Negative-input precedence, zero versus absent mark, and checked arithmetic are
  documented without supplying their implementation.
- The stub throws an explicit unimplemented exception. The component test skips;
  neither claims valuation correctness or implements Tommy's missing-mark test.
- GoogleTest is pinned to full commit `f8d7d77c06936315286eb55f8de22cd23c188571`.
  FetchContent and test targets are inside `BUILD_TESTING`; only the test executable
  links GoogleTest. Production headers contain no GoogleTest dependency. The
  downloaded checkout's `git rev-parse HEAD` matched the pin. A separate
  `BUILD_TESTING=OFF` build was not run.
- Build paths derive from the helper location; VS2019 and the documented PS7
  executable are machine-specific prerequisites. The guide states that limitation
  and distinguishes Windows PowerShell from the required helper runtime.
- Assignment paths now match include/src/tests and the learning README. Full
  ledger accounting, event ordering, and profit calculation remain outside scope.

## Runtime evidence and reproduction

From this review worktree, executed these documented commands separately:

```powershell
$riskPwsh = 'C:\Users\18589\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe'
& $riskPwsh -NoProfile -File ./tools/l001.ps1 configure
& $riskPwsh -NoProfile -File ./tools/l001.ps1 build
& $riskPwsh -NoProfile -File ./tools/l001.ps1 test
```

Initial configure failed downloading GoogleTest because sandbox network access
was blocked; the helper returned failure. The same configure command succeeded
through normal network escalation. Build compiled and linked with VS2019/MSVC
19.29.30138.0, SDK 10.0.19041.0, x64 Debug. No software was installed.

CTest discovered exactly two tests: `Infrastructure.Cpp17AndGoogleTestRun` passed;
`Valuation.ApprenticeTestsPending` skipped. The command exited zero. Aggregate
success therefore establishes only infrastructure operation.

Additional focused checks:

1. Ran the installed CTest with `--test-dir build/l001 -C Debug
   --output-on-failure --no-tests=error -R '^NoSuchRiskProbe$'`: no tests found,
   exit 8. This guards an empty selection, not disappearance of component tests
   when the infrastructure test still exists.
2. Temporarily appended an unrelated `Infrastructure.RiskRunnerFailureProbe`
   containing `FAIL()` to my infrastructure source. Build succeeded; CTest
   reported one failure, one pass, one skip and exit 8. The PS7 helper propagated
   a nonzero exit (1). Original source bytes were restored in a `finally` block.
   Rebuilt and reran: one pass, one skip, zero failures. Git confirmed no source
   difference remained. No apprentice code or component test was changed.
3. From Windows PowerShell, launched the guide's explicit PS7 executable with
   `-NoProfile -File ./tools/l001.ps1 test`: exit zero, one pass and one skip.
   This verifies the runtime invocation from that shell; configure/build were
   independently executed from PS7, not repeated from Windows PowerShell.

## Limits and handoff

No runtime valuation claims are possible: the placeholder does not call the stub.
Acceptance of Tommy's later work requires actual component cases and independent
expected values; an infrastructure-only or skipped run cannot satisfy L-001.
Only the documented Windows Debug configuration was exercised. No extra framework
or portability work is requested. No required correctness/interface changes or
nonblocking style requests remain for this scaffold.

Only this review is committed. Submission commit accompanies the handoff.
Next owner: coordinator for PR transport/integration; engineering for Tommy's
first-test handoff. Any changed harness head needs review of affected conclusions.
