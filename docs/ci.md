# Automated C++ checks

`.github/workflows/cpp.yml` configures, builds and runs CTest for pull requests and
pushes to main on a hosted Windows 2022 runner, x64 Debug. It uses C++17 and the
existing pinned GoogleTest source from CMake. No developer-machine paths or
additional credentials are needed.

Checkout is pinned to the full commit resolved from the official action's v7 tag
on 2026-09-18. Repository permission is read-only and credentials are not retained
in Git configuration. The workflow neither deploys nor changes repository settings.
See the official [checkout action](https://github.com/actions/checkout) and
[workflow syntax](https://docs.github.com/en/actions/reference/workflows-and-actions/workflow-syntax).

Each build stage is a separate step; a failed configure/build prevents tests from
running against stale output. CTest uses `--no-tests=error`, so an empty discovery
or selection is a failure, not a successful check. Domain rejection tests still
pass when the returned error matches their assertions. An explicit apprentice
scaffold skip remains a skip and is not counted as verified behavior.

The log records the checked-out commit and tool versions. For a pull request,
the default checkout tests GitHub's merge ref; record that SHA alongside the PR
head when reporting evidence. This checks the proposed integration, not just the
head in isolation. The main push run verifies the actual merge separately.

## Verification status

A committed workflow is configuration, not evidence of a hosted successful run.
Exact PR review/run links and observed pass/fail/skip counts must accompany
acceptance. Risk also checks that a controlled incorrect assertion and an empty
test selection fail. Temporary probes belong in reviewer-owned work, are removed
before acceptance, and never replace or edit the apprentice's contribution.

No branch-protection enforcement, billing change, secret, deployment, Release,
sanitizer or additional platform coverage is introduced by this workflow.
