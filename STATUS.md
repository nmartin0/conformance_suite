# Status

Read this first when resuming work in a new session.

## Where things are

- Design doc: https://claude.ai/artifact/BCH6E4hts9mcGb2t44GRW5
- This repository travels as git bundles; see "Bundles" below.

## Done

- Repository initialized (LICENSE: proprietary, Nicholas Martin; README; this file).

## Next (roadmap phase 1, "Skeleton")

1. `core/pc.h`, `core/pc.c`: C test helper (result line, phases, temp files).
2. `core/pc.sh`: shell test helper, builtins only.
3. `core/pcexec.c`: runs one test (timeout, limits, user, private temp dir).
4. `core/probe.c`: prints claimed options and limits.
5. `core/trust/`: trust-base checks for the driver.
6. `posixcheck.sh`: driver writing `results/run.log`.
7. `modules/xsh/open/`: first module, a few assertions with mutants.
8. `analysis/`: log loader into SQLite (runs off-system).

## Open decisions

- Project name (`posixcheck` is a placeholder).

## Bundles

Each checkpoint is sent as `posixcheck-NNNN.bundle`, a complete copy of
`main`. To resume in a new session, attach the latest bundle and clone it:
`git clone posixcheck-NNNN.bundle posixcheck`.
