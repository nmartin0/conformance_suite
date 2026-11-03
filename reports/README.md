# Run reports

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Each directory here keeps the results of a run of posixcheck on one system, so
they can be analysed later without repeating the run.

    reports/<date>-<system>/
        system.txt      uname, compiler, shell, how the run was made
        root.txt        full standard output of the run with elevated privilege
        user.txt        the same as an ordinary user (when it was made)
        not-pass.txt    only the FAIL, INSPECT and UNRESOLVED lines of both runs
        summary.txt     the SUMMARY lines
        root.run.log    the run.log of each run, as `analysis/analyze.py load`
        user.run.log    reads it (give ci/save-run.sh the results directories)

`ci/save-run.sh` makes such a directory from the run outputs. The
cross-system comparison and the triage of each finding (system fault, test
fault, or a question about the standard) are in `reports/FINDINGS.md`.
