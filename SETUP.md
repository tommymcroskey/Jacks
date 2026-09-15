# Start the Jacks team

## 1. Save the foundation in Git

The three worktrees must include these instructions. From a terminal in the Jacks repository, inspect git status. If the setup has already been committed, skip this step. Otherwise stage only the setup files and commit:

```powershell
git add README.md AGENTS.md SETUP.md .gitignore company learning
git commit -m "Set up Jacks research apprenticeship"
```

The foundation was committed and pushed by the owner. Subsequent changes use feature branches and GitHub pull requests, including documentation. The team may merge independently reviewed routine PRs without owner permission. See company/workflow.md. GitHub access must be available to agents that push branches or open PRs; otherwise they report the exact missing step.

## 2. Start three Codex tasks under the existing Jacks project

Keep the existing conversation as your team room. Open a new task under the Jacks project. Under the message box, select Worktree, then select main as the starting branch. Codex creates the worktree folder when you submit the prompt; there is no folder to find beforehand. Start from main after the workflow PR is merged. Use your configured default model. Submit the matching prompt below. If the starting workspace does not contain AGENTS.md and company/projects/P001.md, fix its base before proceeding.

You can instead ask the existing team room: "Create the three specialist tasks for Jacks in separate worktrees using the startup prompts in SETUP.md, and run the kickoff." This is an explicit request to create the additional tasks; no new repository or replacement team room is needed.

### Senior Engineer

```text
You are the Senior Engineer and primary apprentice mentor for Jacks. Read AGENTS.md, company/workflow.md, company/status.md, company/roles/senior-engineer.md, learning/README.md, and company/projects/P001.md. Verify that you are in your own worktree. Create codex/eng-001-kickoff if no suitable branch is active. Complete only ENG-001, writing company/briefs/ENG-001.md. Do not implement the component or issue the apprentice assignment yet. Commit your brief, push your feature branch to origin, and open a draft PR against main using the repository PR template. Return its PR URL, full commit hash, artifact path, findings, and handoff. Do not merge. If GitHub access is unavailable, preserve the local commit and report the missing operation. Stop after this bounded deliverable. Use Senior Engineer as this task's title.
```

### Quant Researcher

```text
You are the Quant Researcher for Jacks. Read AGENTS.md, company/workflow.md, company/status.md, company/roles/quant-researcher.md, learning/README.md, and company/projects/P001.md. Verify that you are in your own worktree. Create codex/res-001-kickoff if no suitable branch is active. Complete only RES-001, writing company/briefs/RES-001.md. Do not implement the apprentice contribution. Commit your brief, push your feature branch to origin, and open a draft PR against main using the repository PR template. Return its PR URL, full commit hash, artifact path, findings, and handoff. Do not merge. If GitHub access is unavailable, preserve the local commit and report the missing operation. Stop after this bounded deliverable. Use Quant Researcher as this task's title.
```

### Risk and Validation

```text
You are the Risk and Validation specialist for Jacks. Read AGENTS.md, company/workflow.md, company/status.md, company/roles/risk-validation.md, learning/README.md, and company/projects/P001.md. Verify that you are in your own worktree. Create codex/risk-001-kickoff if no suitable branch is active. Complete only RISK-001, writing company/briefs/RISK-001.md. Commit your brief, push your feature branch to origin, and open a draft PR against main using the repository PR template. Return its PR URL, full commit hash, artifact path, findings, and handoff. Do not merge. If GitHub access is unavailable, preserve the local commit and report the missing operation. Stop after this bounded deliverable. Use Risk and Validation as this task's title.
```

## 3. Run the kickoff from the existing conversation

If you opened the specialists manually, send this in the existing team room:

```text
Act as the Jacks team room using AGENTS.md and company/workflow.md. Find and register the three specialist tasks for this repository. Collect their kickoff briefs and arrange actual cross-review: risk reviews the engineering proposal, engineering reviews research's measurement requirements, and research reviews whether risk's checks address the question. Preserve disagreements. Review the briefs as proposals through their PRs, prepare integration handoffs, and merge after independent acceptance and relevant checks under the team's authority. Record the kickoff decision with exact reviewed commits in a separate PR after the briefs are merged. Ask engineering to give me a beginner-friendly project briefing and a short introductory discussion, then prepare one useful apprentice assignment. Explain what the team is thinking and why. Do not solve my assignment or add schedules.
```

The team room must verify actual task identities, responses, and commits. It must not invent a meeting transcript. Paused specialists need explicit messages to resume.

## 4. Your ordinary working routine

- "Team update: what changed, why, what is uncertain, and what needs me?"
- "Explain this term using our current project."
- "Here is my understanding and proposed approach. Have my mentor review it."
- "I submitted L-001. Arrange review and resume the work it unblocks when accepted."
- "I am stuck. Help me find the missing concept without writing my solution."

You can also speak directly to a specialist. Bring material scope changes back to the team room so the shared record stays coherent.

## What this setup does not automatically do

Files do not create running agents. Worktrees do not share uncommitted edits. Separate tasks do not automatically read each other's conversations. No background schedule is configured. Use explicit team-room dispatch first; add scheduling only after a complete working cycle succeeds.

## Official product references

- [Codex worktrees](https://learn.chatgpt.com/docs/environments/git-worktrees)
- [AGENTS.md instructions](https://learn.chatgpt.com/docs/agent-configuration/agents-md)
