# Build and test Jacks

Engineering owns the build setup. These commands cover the whole project,
including the current apprentice assignment; there is no new setup per task.
The historical names `tools/l001.ps1` and `build/l001` remain in use.

## Local Windows quick start

From the repository root in **PowerShell 7**, run:

```powershell
./tools/l001.ps1 all
```

This configures, builds and tests in order, stopping if a stage fails. To run
the stages separately:

```powershell
./tools/l001.ps1 configure
./tools/l001.ps1 build
./tools/l001.ps1 test
```

Check each result before continuing. Reconfigure after pulling changes to
CMake targets or dependencies. Build after editing code or tests; running
`test` alone does not rebuild executables.

The helper requires the installed Visual Studio 2019 C++ x64 toolchain,
`vswhere.exe`, and Visual Studio's bundled CMake/CTest. It selects an x64
Debug build and handles the known inherited `Path`/`PATH` conflict in child
processes without changing machine settings. Initial configuration fetches
pinned GoogleTest and needs network access unless the dependency is cached.
The helper does not install tools. Report missing-tool errors to engineering.

### Starting from Windows PowerShell 5.1

The helper uses PowerShell 7 APIs; do not run it directly under 5.1. On Tommy's
existing machine, the bundled runtime can launch it:

```powershell
$engPwsh = 'C:\Users\18589\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe'
& $engPwsh -NoProfile -File ./tools/l001.ps1 all
```

That runtime path is machine-specific. If it changes, ask engineering to locate
the installed PowerShell 7 executable; do not assume the path exists elsewhere.

## Read the result, not just the success banner

- **Passed:** the executed test's assertions succeeded.
- **Skipped:** a test intentionally did not verify behavior. A new assignment's
  placeholder is an explicit skip, not evidence that its function works.
- **Failed:** investigate the failure before submitting or merging.
- **No tests:** the helper uses `--no-tests=error`; an empty run is a failure.

CTest can print `100% tests passed` while also listing skipped tests. Check the
individual results and final skipped-test list. Do not report discovered tests
as executed passes. Old assignment scaffold counts are historical; use the
output from the exact revision you tested.

New scaffolds may throw until the apprentice implements them. The assignment's
public contract specifies the final behavior; the temporary exception is not
the intended domain-error interface.

## Where things live

| Location | Responsibility |
| --- | --- |
| `CMakeLists.txt` | Explicit library, executable and test targets |
| `cmake/JacksTesting.cmake` | Pinned GoogleTest and shared test-target setup |
| `cmake/JacksWarnings.cmake` | Private warning flags for project targets |
| `include/jacks/` | Public types and function contracts |
| `src/` | Component implementations; apprentice work remains apprentice-owned |
| `tests/` | Unit and integration checks, separate from production code |
| `tests/research/`, `tests/risk/` | Build wiring for independent specialist checks |
| `examples/` | Small runnable demonstrations |
| `build/l001/` | Generated files and dependencies, ignored by Git |
| `learning/assignments/` | Task-specific requirements and ownership |

GoogleTest v1.14.0 is pinned to commit
`f8d7d77c06936315286eb55f8de22cd23c188571`; GoogleMock and dependency installation
are disabled. Do not introduce another test framework or duplicate implementations.

## See the components working together

After a successful build:

```powershell
./build/l001/Debug/jacks_account_demo.exe
```

The [account walkthrough](account-walkthrough.md) explains its complete and
intentionally incomplete deposit histories. It is a demonstration, not a
general trading simulator.

For the synthetic deposit, buy, mark update, and sell example:

```powershell
./build/l001/Debug/jacks_trading_demo.exe
```

The [trading walkthrough](trading-walkthrough.md) explains the supplied prices,
fees, account values, and profit. This uses predetermined synthetic events and
explicit accounting calls; it does not simulate a strategy or market execution.

## Before submitting

Use the current [assignment](../learning/assignments/) for the behavior to test.
Write expected values independently of the implementation. A failing behavioral
test establishes a useful red step; a compilation error alone does not.

Build and run the full suite after changes. Include a brief account of what
changed, the actual commands/results, and the tested commit (or say the run
included uncommitted edits). Distinguish passes, failures and skips. A short PR
description is enough unless the assignment says otherwise; no reconstructed
commit diary is required.

[GitHub CI](ci.md) also configures/builds/tests, using hosted Windows 2022 and
Visual Studio 2022 rather than this machine's VS2019 helper. Record which
environment produced a result; a successful local run is not a hosted-run claim.
