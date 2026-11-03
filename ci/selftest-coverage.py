#!/usr/bin/env python3
# selftest-coverage.py - ci/coverage.py must fail for a page nobody covers.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Builds a tiny standard (three function pages, two with the same text) and
# an empty module tree, and checks coverage.py's exit status and output for
# a missing page, an alias, a stale alias and a stale known-missing line.

import subprocess
import sys
import tempfile
from pathlib import Path

here = Path(__file__).resolve().parent
PAGE = '<html><body><h4 class="mansect">DESCRIPTION</h4><p>%s</p></body></html>'


def run(std, mods, aliases, known):
    r = subprocess.run([sys.executable, str(here / "coverage.py"),
                        "--standard", str(std), "--modules", str(mods),
                        "--aliases", str(aliases),
                        "--known-missing", str(known)],
                       capture_output=True, text=True)
    return r.returncode, r.stdout


def main():
    bad = 0

    def expect(label, got, rc, want_rc, needle):
        nonlocal bad
        ok = rc == want_rc and (needle in got if needle else True)
        if not ok:
            bad += 1
            print(f"selftest-coverage: FAIL {label} (exit {rc})\n{got}")

    with tempfile.TemporaryDirectory() as t:
        t = Path(t)
        (t / "std/functions").mkdir(parents=True)
        (t / "mods").mkdir()
        (t / "std/functions/a.html").write_text(PAGE % "one")
        (t / "std/functions/b.html").write_text(PAGE % "one")
        (t / "std/functions/c.html").write_text(PAGE % "two")
        al, km = t / "aliases", t / "known"
        # No modules at all: a is not covered, so the run fails and names it.
        al.write_text(""), km.write_text("")
        rc, out = run(t / "std", t / "mods", al, km)
        expect("missing page", out, rc, 1, "functions/a.html has no module")
        # Known missing pages pass; a stale alias target still fails.
        km.write_text("functions/a.html\nfunctions/b.html\nfunctions/c.html\n")
        rc, out = run(t / "std", t / "mods", al, km)
        expect("known missing", out, rc, 0, "")
        # One module (a): b aliases a, c is known missing: green.
        (t / "mods/xsh/a").mkdir(parents=True)
        (t / "mods/xsh/a/MODULE").write_text("page functions/a.html\n")
        (t / "mods/xsh/a/a.asr").write_text("")
        al.write_text("functions/b.html functions/a.html\n")
        km.write_text("functions/c.html\n")
        rc, out = run(t / "std", t / "mods", al, km)
        expect("alias", out, rc, 0, "")
        # An alias whose text differs from its target needs its own module.
        al.write_text("functions/c.html functions/a.html\n")
        km.write_text("functions/b.html\n")
        rc, out = run(t / "std", t / "mods", al, km)
        expect("alias differs", out, rc, 1, "text differs")
        # A known-missing line for a page that has a module is stale.
        al.write_text("functions/b.html functions/a.html\n")
        km.write_text("functions/c.html\nfunctions/a.html\n")
        rc, out = run(t / "std", t / "mods", al, km)
        expect("stale known-missing", out, rc, 1, "has a module but")
    print("selftest-coverage:", "FAILED" if bad else "ok")
    sys.exit(1 if bad else 0)


main()
