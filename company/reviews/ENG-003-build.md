# ENG-003: local C++17 build check

Author: Senior Engineer agent. Date: 2026-09-15.
Branch: `codex/eng-002-review`; starting commit:
`c59d896f615cda3e6da38f6d152c5db0c5142047` (clean tracked worktree).
Assignment: verify installed tools using a non-quant scratch program and record
later ledger-contract considerations. L-001 and its implementation are unchanged.

## Result and limitation

Successful x64 Debug compile/link and execution with MSVC 19.29.30138.0,
MSBuild 16.11.2, and Windows SDK 10.0.19041.0. CMake/CTest version:
3.20.21032501-MSVC_2, bundled in Visual Studio Community 2019. CTest passed
1/1; direct execution printed `C++17 toolchain OK` and exited 0. C++17 is
required by CMake and a compile-time assertion; warnings are treated as errors.

Initial CMake configuration succeeded, but the first build failed with MSB6001:
MSBuild received both `Path` and `PATH` and reported a duplicate dictionary key.
Re-running with one combined PATH entry in the child process succeeded. No
machine/user environment settings were changed and no software was installed.
The reproduction helper below applies that child-only normalization. Before
the successful retry, the CTest output regex was changed to bracketed plus signs
to avoid CMake backslash-escaping ambiguity; no test had run before that change.

This establishes a usable local compiler/linker/test runner for this smoke check.
It does not validate the future project harness, quant logic, Release builds,
another machine, or an independent reproduction. The team still owns preparing
the minimal L-001 harness before Tommy relies on it.

## Exact scratch contents and reproduction

Scratch directory: `build/eng-toolchain-smoke/` in this specialist worktree.
It is ignored by the existing `build/` rule. Only this note is committed.
To reproduce elsewhere, create its `src/` directory and save these two files.

`src/main.cpp`:

```cpp
#include <optional>
#include <string_view>
#include <iostream>
static_assert(__cplusplus >= 201703L, "C++17 required");
int main() {
    const std::optional<std::string_view> greeting{"C++17 toolchain OK"};
    if (!greeting || *greeting != "C++17 toolchain OK") return 1;
    std::cout << *greeting << '\n';
    return 0;
}
```

`src/CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(eng_toolchain_smoke LANGUAGES CXX)
add_executable(eng_toolchain_smoke main.cpp)
set_target_properties(eng_toolchain_smoke PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
if(MSVC)
  target_compile_options(eng_toolchain_smoke PRIVATE /W4 /WX /permissive- /Zc:__cplusplus)
endif()
enable_testing()
add_test(NAME smoke COMMAND eng_toolchain_smoke)
set_tests_properties(smoke PROPERTIES PASS_REGULAR_EXPRESSION "C[+][+]17 toolchain OK")
```

PowerShell 7 commands run from the repository root (helper formatting condensed):

```powershell
$engCmakeBin = 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin'
function Invoke-EngCleanPath([string]$exe, [string[]]$arguments) {
    $engStart = [System.Diagnostics.ProcessStartInfo]::new()
    $engStart.FileName = $exe
    $engStart.UseShellExecute = $false
    foreach ($engArg in $arguments) { $engStart.ArgumentList.Add($engArg) }
    $engPathKeys = @($engStart.Environment.Keys | Where-Object { $_ -ieq 'PATH' })
    $engPathValues = @($engPathKeys | ForEach-Object { $engStart.Environment[$_] })
    foreach ($engKey in $engPathKeys) { $engStart.Environment.Remove($engKey) | Out-Null }
    $engStart.Environment['PATH'] = ($engPathValues | Select-Object -Unique) -join ';'
    $engProcess = [System.Diagnostics.Process]::Start($engStart)
    $engProcess.WaitForExit()
    if ($engProcess.ExitCode -ne 0) { throw "Tool failed with exit $($engProcess.ExitCode)" }
}
Invoke-EngCleanPath "$engCmakeBin/cmake.exe" @('-S','build/eng-toolchain-smoke/src','-B','build/eng-toolchain-smoke/out','-G','Visual Studio 16 2019','-A','x64')
Invoke-EngCleanPath "$engCmakeBin/cmake.exe" @('--build','build/eng-toolchain-smoke/out','--config','Debug')
Invoke-EngCleanPath "$engCmakeBin/ctest.exe" @('--test-dir','build/eng-toolchain-smoke/out','-C','Debug','--output-on-failure')
& 'build/eng-toolchain-smoke/out/Debug/eng_toolchain_smoke.exe'
# Observed LASTEXITCODE: 0
```

## Reserved for the later ledger contract, not L-001

These are engineering proposals for peer agreement, not implemented behavior:

- A sale must have sufficient holdings and leave nonnegative cash after its fee.
  The fee may exceed sale proceeds only if existing cash covers the difference.
  Check representability of trade arithmetic and resulting balances before
  committing any event state; overselling and fee-funded borrowing are rejected.
- Separate event rejection from valuation unavailability. A valid event updates
  balances and accepted sequence even if a missing mark or equity overflow makes
  its valuation unavailable. Return that valuation status explicitly; never
  present stale equity as the current result. Invalid event fields or overflow
  in balance/flow/fee updates reject the event atomically, including its sequence.
- A batch stops at the first rejected event and returns its position/reason plus
  the accepted prefix and its resulting state. No whole-batch rollback and no
  silent skipping. A valuation-only error is attached to the accepted snapshot
  and does not stop subsequent valid events. This must be made explicit in the
  future result types and checked with peer-authored cases before implementation.

Next owner: team room for peer review/PR transport; engineering for the later
ledger contract and minimal harness when dispatched. No fixture or valuation
solution was created. The successful smoke check has no remaining local blocker.
