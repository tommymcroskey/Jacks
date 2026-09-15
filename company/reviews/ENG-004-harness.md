# ENG-004: L-001 GoogleTest harness handoff

Author: Senior Engineer agent. State: ready for independent scaffold review.
Task: `01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941`.
Branch: `codex/l001-harness`; base `origin/main`:
`2e2834ca0dc7b67cfee4febb74639ffa4786f496`.

## Final change and ownership

CMake builds `jacks_valuation` from `src/valuation.cpp`, publishing only the
`include/` API. Tests live under `tests/`; GoogleTest is linked only to the test
executable. `BUILD_TESTING=OFF` omits the dependency and test targets. The learning
folder points to shared code rather than holding a duplicate implementation.
`docs/build.md` explains the actual directory layout and three terminal commands;
`tools/l001.ps1` handles the verified Windows environment without global changes.

Frozen signature in `include/jacks/valuation.hpp`: signed 64-bit cash and whole
quantity, optional signed 64-bit mark, returning `variant<Equity, ErrorCode>`.
Errors: `invalid_input`, `missing_mark`, `overflow`. Input validation remains at
the function boundary; zero mark is valid per L-001. No wrapper operators,
constructor validation, tables, or numeric sentinels are introduced.

`src/valuation.cpp` deliberately throws an unimplemented-stub logic_error on every
call; it supplies no valuation behavior or domain-error implementation.
`tests/valuation_test.cpp` contains only `GTEST_SKIP()`, not Tommy's accepted
missing-mark case. He owns replacing both. An infrastructure test uses an unrelated
string to exercise C++17 and GoogleTest. No component correctness is claimed.

Tommy prefers terminal work and TDD. He correctly distinguished the deposit
example's zero investment profit and proposed positive holdings without a mark
returning missing_mark. Those are demonstrated steps, not broad mastery. No
further quiz is required. Engineering owns the harness prerequisite; after peer
review his next action is writing the first failing test, with an independently
justified expectation before implementation.

The prior ENG-003 note remains its own unchanged cherry-picked commit:
`6a873cfd04bb8de128ed373d1caa0d5cad7d289b` from original
`a529e0285d9b06a15e008b730679f3674d5830bc`. Its toolchain results are historical
scratch evidence, distinct from the final scaffold results below. No updates
were pushed onto completed ENG-002 PR #6. Tommy's separate `intern/L001` checkout
and its reported untracked `.test.cpp.un~` were not touched.

## Dependency provenance

Official GoogleTest v1.14.0 resolves to immutable commit
`f8d7d77c06936315286eb55f8de22cd23c188571`, verified with
`git ls-remote https://github.com/google/googletest.git refs/tags/v1.14.0`.
The local FetchContent checkout's `git rev-parse HEAD` matches. No moving tag is
used by CMake. GoogleMock and installation are disabled; runtime settings match
MSVC. The official version's CMake/README were inspected; this version supports
C++14 and later. First configuration needs network access to fetch source into
ignored `build/`; no global installation. The initial sandboxed remote lookup
failed to connect; the normally authorized network escalation succeeded.

## Checks actually performed

- Final `./tools/l001.ps1`: configure, x64 Debug compile/link, CTest discovery and
  run passed with MSVC 19.29.30138.0, SDK 10.0.19041.0, CMake/CTest
  3.20.21032501-MSVC_2. **2 discovered: 1 infrastructure pass, 1 component skip.**
- Created an ignored scratch `build/l001/runner_probe.cpp` with an intentional
  GoogleTest FAIL, temporarily included by the infrastructure source. Build
  succeeded; CTest reported 1 failure, 1 pass, 1 skip and exit 8. The PowerShell
  helper propagated the failure. Source restoration used a finally block.
- Rebuilt and reran the documented separate `build` and `test` actions after
  restoration: **1 infrastructure pass, 1 component skip, 0 failures**. The probe
  is not in the final target sources or committed files.
- Compared interface to L-001; checked documentation links, scoped ownership,
  ignored build output, pinned dependency checkout, and staged whitespace.

Setup finding: direct `powershell.exe -NoProfile -File tools/l001.ps1` failed
because Windows PowerShell 5.1 reported script execution disabled. The helper
also uses PowerShell 7 APIs. Instead of changing policy or assuming PS7 on PATH,
the guide now gives the absolute existing Codex PS7 runtime invocation. Verified
from Windows PowerShell 5.1 using `-Command` to launch that executable with
`-NoProfile -File ./tools/l001.ps1`: configure/build/test succeeded, one pass and
one skip. No execution-policy setting was modified. The runtime path is specific
to this machine and must be updated if its installation moves.

The scaffold was initially exercised with a plain CTest placeholder before the
owner requested GoogleTest. Those earlier checks are not substituted for the
final GoogleTest results above. Other platforms/configurations, independent
reproduction and component behavior remain unverified. No economic fixtures,
valuation implementation, or apprentice test solution were written.

## Review and integration

Team room owns PR transport and dispatch. Risk/research should review the exact
head for contract consistency, explicit skipped status, dependency scope, and
Tommy's ownership, then reproduce the guide's commands. Confirm L-001's explicit
zero-mark rule against the earlier positive-only RES-002 proposal; do not infer
acceptance of unresolved ledger choices. The signature has no ledger semantics.

Merge only after independent review. The historical note and scaffold are separate
commits with independent corrective-PR/revert boundaries. A skipped component
placeholder cannot satisfy eventual L-001 acceptance. No merge or ledger
implementation is performed by this handoff. Final response supplies exact head.
