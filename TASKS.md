# MIMIC — Contributor Task List

*A curated set of tasks open to community contributors, organized by area and effort.*

-----

## How to use this list

Each task below is sized and tagged so a prospective contributor can pick one without negotiating scope first. The intent is that every task here is **directly file-able as a GitHub issue** — title, description, acceptance criteria, and rough effort already specified. Maintainers should keep ~10–15 of these open and labeled `good first issue` or `help wanted` at any given time; replenish as they get claimed.

**Effort tags:**

`XS` — under 2 hours, suitable for a first contribution
`S` — half a day to a day
`M` — a weekend or a few evenings
`L` — multi-week project, usually needs design discussion first

**Skill tags:** `hardware`, `firmware`, `software`, `docs`, `testing`, `mechanical`, `community`, `research`.

Tasks marked **[MIMIC-specific]** need to be customized once the project's actual scope is filled in — they're scaffolding for the categories of work that will exist, not literal issues yet.

-----

## Documentation

Documentation tasks are the highest-leverage contributions an outsider can make, because they reflect what a new user actually struggles with — something maintainers cannot easily see.

**Improve the "first 30 minutes" path in the README** `XS` `docs`
   Walk through the README on a clean machine and file a PR fixing every place you got stuck, confused, or had to guess. No prior project knowledge required — confusion is the qualification.

**Write a "from zero to blinking" tutorial** `S` `docs`
   A step-by-step tutorial assuming the reader has never built a PCB before. Include photos at each soldering or assembly step. Define the minimal "hello world" demo for MIMIC.

**Add troubleshooting section to the build guide** `S` `docs` `hardware`
   Document the failure modes you (or others) hit during the build, with the symptom, the cause, and the fix. Even five entries is useful.

**Translate the README to another language** `S` `docs`
   Priority languages depend on community interest; Spanish, German, Japanese, and Hindi tend to be high-value for OSHW projects.

**Create an interactive BOM page** `S` `docs` `hardware`
   Set up [openscopeproject/InteractiveHtmlBom](https://github.com/openscopeproject/InteractiveHtmlBom) and wire it into CI so the iBOM regenerates on every merge to `main`. Publish to GitHub Pages.

**Document the design decisions in an ADR log** `M` `docs`
   Create `docs/adr/` and write Architecture Decision Records for the major design choices already made — component selection, topology, license choice, etc. Even retrospective ADRs are valuable.

**Write a "porting MIMIC to a different microcontroller" guide** `M` `docs` `firmware` `[MIMIC-specific]`
   Document the abstraction layer (or the lack of one) so contributors can fork for alternate MCUs.

-----

## Hardware Design

**Review the schematic and file issues for anything unclear** `XS` `hardware`
   A schematic review from someone who hasn't designed the board is a useful sanity check. File one issue per unclear annotation, missing value, or under-documented net.

**Add silkscreen labels and indicators** `S` `hardware`
   Where useful, add silkscreen labels for connector pinouts, jumper functions, LED meanings, and revision markers. Improves the unboxing experience materially.

**Add a footprint for an alternate component** `S` `hardware`
   For any component in the BOM with supply-chain risk, design a footprint that accepts a second source. Document the swap in the BOM.

**Generate fabrication-ready outputs in CI** `M` `hardware`
   Set up [KiBot](https://github.com/INTI-CMNB/KiBot) to auto-generate Gerbers, drill files, pick-and-place, and assembly drawings on every tagged release.

**Add SPICE models or simulation for the analog stages** `M` `hardware` `research` `[MIMIC-specific]`
   For any analog subsystem, contribute LTSpice or ngspice models and check them into `simulation/` alongside expected results.

**Design an alternate form factor** `L` `hardware`
   A more compact / castellated / panel-mount variant. Should be proposed via RFC first.

**Run DFM analysis and propose improvements** `M` `hardware`
   Walk through the board for manufacturability issues — track widths near pad edges, tombstone risk, panelization, etc. File one issue per finding.

-----

## Firmware

**Add unit tests for an untested module** `S` `firmware` `testing`
   Pick any source file under `firmware/` without corresponding tests in `firmware/tests/` and add coverage. Even three test cases is a useful contribution.

**Improve error handling and logging** `S` `firmware`
   Audit the firmware for unchecked return values, silent failures, or unclear error states. File issues and submit incremental PRs.

**Add a `--version` and `--help` to the host tooling** `XS` `software`
   If the project ships any CLI utilities, make sure they answer these two flags coherently.

**Profile firmware memory and CPU usage, document baseline** `M` `firmware` `research`
   Run measurements, write them up in `docs/performance.md`, and add the measurement script to CI so future regressions get caught.

**Add a serial protocol fuzzer** `M` `firmware` `testing` `[MIMIC-specific]`
   If MIMIC has a serial / USB / network protocol, write a fuzzer that exercises it and runs in CI.

**Implement a missing feature from the roadmap** `L` `firmware` `[MIMIC-specific]`
   Pick from the `Next` column on the public Projects board. Coordinate with maintainers first.

-----

## Testing & Verification

**Build a board and report back** `M` `hardware` `testing`
   Order the PCB, source the BOM, build it, follow the build guide, and file issues for anything that didn't work as documented. **This is the single most valuable contribution a new community member can make.** Maintainers will reimburse PCB costs on request for the first N independent builds.

**Test the design under environmental stress** `M` `testing` `research` `[MIMIC-specific]`
   Temperature, humidity, vibration, EMI — whichever is relevant for MIMIC's use case. Document the test setup and results.

**Verify the BOM against current distributor stock** `S` `hardware`
   Walk the BOM against current Mouser / Digikey / LCSC stock. Flag anything that's NRND, on long lead time, or only available in large quantities.

**Test the build guide on a Windows / macOS / Linux machine** `S` `docs` `testing`
   For each OS, follow the build guide on a clean install and report what doesn't work. Toolchain bugs are usually the long tail of contributor drop-off.

**Run the firmware on a hardware variant** `M` `firmware` `testing`
   Test on rev-A vs rev-B silicon, different MCU bin, alternate component substitutions, etc. Document compatibility.

-----

## Mechanical / Enclosure

**Design a 3D-printable enclosure** `M` `mechanical`
   STL + STEP files in `mechanical/enclosure/`. Document the printer settings used.

**Add mounting hole templates** `S` `mechanical` `docs`
   For desk mounting, DIN rail, panel mount, etc. PDF templates plus drill specs.

**Design a tray for desk use** `S` `mechanical`
   Simple printable tray that holds the board plus any connectors at usable angles.

**Photograph the assembled hardware professionally** `S` `community`
   High-resolution photos on a neutral background, multiple angles, with and without enclosure. Release under CC-BY 4.0 in `media/`.

-----

## Tooling & Automation

**Set up first-contributor welcome bot** `XS` `software` `community`
   GitHub Action that comments on a contributor's first PR with a thank-you and a link to `CONTRIBUTING.md`.

**Add issue templates and PR templates** `XS` `docs`
   Bug report, feature request, hardware issue, documentation issue — each with a clear template that captures the information maintainers need.

**Add release-please or git-cliff for automated changelogs** `S` `software`
   Whichever fits the project's commit style. Document the convention.

**Build a Matrix/Discord bridge bot** `M` `software` `community`
   Posts new issues, merged PRs, and releases to a `#firehose` channel.

**Add docs site auto-deploy** `S` `software` `docs`
   MkDocs (Material) or Docusaurus from `docs/`, deployed to GitHub Pages or `docs.mimic.aegiondynamic.com` on every merge to `main`.

**Create a reproducible build environment** `M` `software`
   Devcontainer or Nix flake so a contributor can get a working toolchain in one command. Drops onboarding friction substantially.

-----

## Community & Outreach

**Triage stale issues** `S` `community`
   Walk through the open issues, comment on anything older than 60 days, close anything resolved, label anything missing labels.

**Answer questions in the Matrix / Discord room** `ongoing` `community`
   Even partial answers ("I hit this too, here's what worked for me") are useful. Becomes a path to maintainership over time.

**Write a build-log post for Hackaday.io** `S` `community` `docs`
   Document your build experience, what surprised you, what you'd change. Cross-link from the project's Hackaday.io page.

**Make a video walkthrough** `M` `community` `docs`
   A 5–15 minute build or demo video. Doesn't need to be polished — clarity beats production value.

**Submit MIMIC to relevant directories and showcases** `S` `community`
   awesome-lists, Hackster project showcase, OSHWA directory (after certification), domain-specific catalogs.

**Run a workshop or meetup using MIMIC** `L` `community`
   Local makerspace, university lab, conference workshop. Aegion can provide materials support for serious proposals; coordinate before committing.

-----

## Research & Characterization

**Benchmark MIMIC against a comparable proprietary product** `M` `research` `[MIMIC-specific]`
   Pick a comparable commercial system, design a head-to-head benchmark, document methodology and results. Reproducibility script in `benchmarks/`.

**Characterize the noise / accuracy / drift profile** `M` `research` `[MIMIC-specific]`
   Whatever the relevant performance metrics are for MIMIC's domain. Publish raw data alongside the writeup.

**Write a comparison study against alternative open designs** `M` `research` `docs`
   Honest comparison — what MIMIC does better, what it does worse, where the gaps are. The OSHW community values this kind of writing highly.

**Investigate a specific failure mode** `M` `research` `hardware`
   Pick a known issue, run it down, document the root cause and the fix. Worth a blog post in its own right.

-----

## Stewardship of this list

A few rules to keep the list useful rather than aspirational:

- A task stays open only as long as it is genuinely *available*. If a maintainer is already working on it silently, close it or assign it.
- When a task is claimed, assign the contributor and set a soft expectation: an update in two weeks or the task returns to the pool. Communicated kindly.
- Every closed task gets a `thanks/` shoutout in the next "State of MIMIC" post, by name.
- This list is reviewed quarterly: completed items archived, stale items closed, new tasks added based on what the project actually needs.
- Tags evolve. If a new category of work emerges (e.g., `localization`, `safety-cert`), add it; don't force everything into the existing scheme.

-----

If you're a contributor reading this — pick anything that catches your interest, comment on the corresponding issue with "I'd like to take this," and a maintainer will respond within 48 hours. No prior contribution required.
