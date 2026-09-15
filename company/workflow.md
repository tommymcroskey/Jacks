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

## Worktree and integration protocol

1. Start from the committed foundation on main. Check for local changes before Git operations; preserve unrelated work.
2. Each specialist creates its own codex/ branch if starting detached. Never check out main in a specialist worktree.
3. Produce a report and commit. Handoff: task ID, source branch, full commit hash, artifact paths, claim, checks, requested review, acceptance criteria, blockers.
4. The team room delivers this handoff to the reviewer. Local worktrees share Git objects, but not uncommitted files. The reviewer can inspect the exact commit with git show. For executable checks, arrange the reviewed version on the reviewer's own branch without overwriting unfinished work.
5. The reviewer creates a separate review artifact referencing the exact reviewed commit, including performed checks, findings, and disposition. Changed code needs reconsideration of affected approvals.
6. Engineering proposes integration order and any conflict resolution. The team room integrates accepted commits sequentially in the clean main checkout, checks the integrated result, then updates canonical status. A merge command succeeding is not evidence the result is correct.
7. Specialists explicitly bring the resulting main commit into their own branches before dependent work; never assume automatic synchronization. Cross-machine collaboration would additionally require pushing/fetching, which is outside initial setup.

Initial kickoff briefs can be integrated as proposals, not as accepted findings. Record their disposition explicitly.

## Human-facing communication

A team update answers: What are we investigating? What changed? Why? What is uncertain? What needs the apprentice? What happens next?

A review meeting collects actual specialist reviews. Preserve disagreements. Do not roleplay multiple reviewers in one answer and call it independent review.

Explain terms when introduced, maintain project vocabulary as it becomes useful, and distinguish measured results from conjecture. Avoid ritual meetings and status updates with no new information.

## Completion

A contribution is done when it meets acceptance criteria, has been reviewed and integrated where applicable, and someone else can use or reproduce it. Learning evidence includes explanation, revisions, and handoff quality as well as code.
