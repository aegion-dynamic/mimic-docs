# Contributing to MIMIC

Thank you for your interest in contributing to Mimic — a hardware simulation ecosystem by [Aegion Dynamic](https://github.com/aegion-dynamic).

This document covers the rules, conventions, and workflow for contributing to any of the three core repositories:

- **[Mimic-Firmware](https://github.com/aegion-dynamic/Mimic-Firmware)** — STM32 firmware (C)
- **[Mimic-Bridge](https://github.com/aegion-dynamic/Mimic-Bridge)** — Python host interface & CLI
- **[Mimic-Sensors](https://github.com/aegion-dynamic/Mimic-Sensors)** — Sensor emulation profiles (Python)

---

## Ground Rules

1. **Be respectful.** We're building hardware tools — precision matters more than speed. Communicate kindly, review carefully.
2. **One concern per PR.** Keep pull requests focused. A PR that fixes a bug should not also refactor unrelated code.
3. **Test your changes.** If you're touching firmware, flash and verify on hardware. If you're touching Python code, run the existing test suite. If there isn't one yet, consider writing a test as your contribution.
4. **Document what you do.** If your change affects behavior, update the relevant documentation in `mimic-docs`.
5. **Ask before starting large work.** For anything tagged `L` (multi-week), open a discussion or issue first so maintainers can align on scope and approach.

---

## How to Contribute

### 1. Find a Task

Check the **[Contributor Task List](./TASKS.md)** for curated, pre-scoped tasks. Each task is tagged with effort and skill requirements.

You can also browse open issues labeled:
- `good first issue` — Ideal for new contributors
- `help wanted` — Contributions actively sought

### 2. Claim It

Comment on the corresponding GitHub issue with **"I'd like to take this."** A maintainer will respond within 48 hours. No prior contribution required.

### 3. Fork & Branch

```bash
# Fork the relevant repo, then:
git clone https://github.com/YOUR_USERNAME/Mimic-Firmware.git
cd Mimic-Firmware
git checkout -b feature/your-descriptive-branch-name
```

### 4. Make Your Changes

Follow the coding conventions for the repo you're working in:

| Repo | Language | Style |
|------|----------|-------|
| Mimic-Firmware | C | STM32 HAL conventions, snake_case, 4-space indentation |
| Mimic-Bridge | Python | PEP 8, type hints encouraged, docstrings for public functions |
| Mimic-Sensors | Python | PEP 8, inherit from `SensorBase`, include register maps |

### 5. Commit Messages

Use clear, descriptive commit messages:

```
feat(sensors): add BMP280 pressure calibration routine

Implements the compensation formula from the BMP280 datasheet (section 4.2.3).
Adds register-level read for calibration coefficients stored in NVM.
```

Prefix conventions:
- `feat:` — New feature or capability
- `fix:` — Bug fix
- `docs:` — Documentation only
- `test:` — Adding or modifying tests
- `refactor:` — Code restructuring without behavior change
- `ci:` — Build/CI pipeline changes

### 6. Submit a Pull Request

- Target the `main` branch.
- Fill out the PR template completely.
- Link the issue you're resolving (e.g., `Closes #42`).
- Ensure CI passes before requesting review.

---

## Contribution Expectations

- **Update cadence:** If you claim a task, provide an update within **two weeks** or the task returns to the pool. Communicated kindly — life happens.
- **Review turnaround:** Maintainers aim to review PRs within **72 hours** of submission.
- **Recognition:** Every merged contribution gets a shoutout in the next "State of MIMIC" post, by name.

---

## Roles

| Role | Description |
|------|-------------|
| **Maintainer** | Full write access. Reviews and merges PRs. Currently: [@Karthik-Sarvan](https://github.com/Karthik-Sarvan) |
| **Contributor** | Anyone who has had a PR merged. Listed on the project landing page automatically via GitHub API. |
| **Reviewer** | Trusted contributors who can approve PRs. Path: 3+ merged PRs → invite to review. |

---

## Code of Conduct

We follow the [Contributor Covenant v2.1](https://www.contributor-covenant.org/version/2/1/code_of_conduct/). In short:

- Be welcoming and inclusive.
- Be respectful of differing viewpoints.
- Accept constructive criticism gracefully.
- Focus on what is best for the community.

Violations can be reported to the maintainer directly.

---

## Security

If you discover a security vulnerability, **do not** open a public issue. Email the maintainer directly or use GitHub's private vulnerability reporting feature.

---

## Questions?

Open a [Discussion](https://github.com/aegion-dynamic/mimic-docs/discussions) or comment on any issue. We're happy to help you find the right task and get started.
