# Work and communication

## Authority

Research proposes investigations; risk and validation defines evidence standards and may block promotion with specific reasons; engineering owns system correctness and feasibility. The team resolves routine technical choices from evidence. Present material unresolved disagreements to the owner with options and consequences, not an unexplained demand for technical arbitration.

The owner controls scope, spending, external commitments, and eventual deployment. They are not required to invent the research agenda. All three specialists help train them; engineering coordinates assignments.

## Initial communication mechanism

The existing Jacks conversation is the team room. It can use available Codex task tools to dispatch requests and collect actual specialist responses once the user has created the three tasks or explicitly requested their creation.

Tasks do not automatically share chat history or wake on file changes. No scheduler is installed. A team-room request starts a bounded coordination cycle; report blockers and return rather than claiming perpetual operation.

At first registration, discover the three tasks, verify their project and role, and record their actual IDs in company/status.md. Do not invent IDs. If task tools are unavailable, tell the user exactly which handoff needs to be pasted into which task.

## Work states

Ready -> active -> review -> done. Blocked names the exact missing artifact and owner. Rejected and inconclusive are valid research outcomes. Acceptance belongs to the named reviewer, never solely to the author.

Keep one project and one apprentice assignment active. Additional specialist work must be a bounded dependency of that project.

## Branch, pull-request, and integration protocol

1. Start from the committed foundation on main. Check for local changes before Git operations; preserve unrelated work.
2. Each specialist creates its own codex/ branch if starting detached. Never check out main in a specialist worktree.
3. Produce a focused commit, push the feature branch to origin, and open a PR against main. Use the PR template. For unfinished proposals, open a draft PR. Handoff: task ID, PR URL, source branch, full commit hash, artifact paths, claim, checks, requested reviewer, and blockers. Do not publish credentials or raw datasets.
4. The team room delivers this handoff to a different specialist. Local worktrees share Git objects, not uncommitted files. Review the exact PR head; use the reviewer's own workspace for executable checks without overwriting unfinished work.
5. The reviewer records performed checks, findings, and disposition against the exact commit. Record the review in a committed review artifact linked from the PR, or an explicitly attributed PR review/comment. Resolve correctness findings and rerun affected checks before acceptance. New changes require reconsidering affected review conclusions.
6. Engineering explains readiness, integration order, and conflicts. Initially the owner performs the final GitHub merge after review and applicable checks. A merge command succeeding is not evidence the result is correct. The team room proposes status/decision updates through a separate PR; no direct-main shortcut.
7. Fetch origin and update main safely after merge, preserving unfinished work. Specialists explicitly bring the resulting main commit into their own branches before dependent work. Use fresh branches for new deliverables after merging; do not reuse a completed PR branch indefinitely.

The apprentice follows the same cycle with mentoring: understand ticket -> create branch -> implement -> commit -> push -> open PR -> respond to review -> merge when accepted. The mentor explains each operation before the apprentice's first use and does not perform their contribution or PR interactions silently.

## GitHub identity and enforcement

Agents currently use the owner's available credentials. They are functional reviewers, not separate GitHub accounts. GitHub does not count an author's self-approval as another person's approval. Label reviews with the actual agent role and reviewed commit, and disclose agent-authored contributions. Do not fabricate human authorship or activity.

These rules are initially a documented process, not configured branch protection. When repository settings and independent review identities permit, configure a main-branch ruleset requiring PRs and applicable status checks. Do not require unavailable external approvals and then bypass the requirement. Add real build/test CI once the project has runnable code; documentation-only kickoff work needs proportionate checks.

Initial kickoff briefs can be integrated as proposals, not as accepted findings. Record their disposition explicitly.

## Human-facing communication

A team update answers: What are we investigating? What changed? Why? What is uncertain? What needs the apprentice? What happens next?

A review meeting collects actual specialist reviews. Preserve disagreements. Do not roleplay multiple reviewers in one answer and call it independent review.

Explain terms when introduced, maintain project vocabulary as it becomes useful, and distinguish measured results from conjecture. Avoid ritual meetings and status updates with no new information.

## Completion

A contribution is done when it meets acceptance criteria, has been reviewed and integrated where applicable, and someone else can use or reproduce it. Learning evidence includes explanation, revisions, and handoff quality as well as code.
