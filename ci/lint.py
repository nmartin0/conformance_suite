#!/usr/bin/env python3
# lint.py - traceability checks for posixcheck (off-system, CI).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ci/lint.py [--standard DIR] [--modules DIR] [--list-mutants]
#
# Fails (exit 1) when the catalog and the tests disagree:
#   - a module's MODULE file is missing fields or names the wrong path
#   - an assertion ID is malformed, duplicated, or outside its module
#   - an assertion has an unknown key or class, or a 'since' that is not
#     an edition year (1988, 1992, 2001, 2008, 2024)
#   - a testable assertion has no test, or its test file is missing
#   - a test's header does not name exactly the assertion that uses it
#   - a test is used by more than one assertion, or by none
#   - a testable assertion's test declares no mutant
#   - a mutant has no implementation
#   - a "uses" prerequisite does not exist, or prerequisites form a cycle
#   - an anchor's page or section is missing from the standard (when
#     --standard points at a local copy of the HTML edition)
#
# --list-mutants prints "ID MUTANT" pairs for ci/mutate.sh.

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog  # noqa: E402

root = Path(__file__).resolve().parent.parent
ap = argparse.ArgumentParser()
ap.add_argument("--modules", type=Path, default=root / "modules")
ap.add_argument("--standard", type=Path,
                help="local copy of the POSIX.1-2024 HTML (susv5-html)")
ap.add_argument("--list-mutants", action="store_true")
args = ap.parse_args()

errors = []


def err(where, msg):
    errors.append(f"{where}: {msg}")


mods = catalog.all_modules(args.modules)
by_id = {}
mutant_pairs = []

for m in mods:
    where = m.dir / "MODULE"
    vol_dir, page = m.path.split("/", 1)
    vol = catalog.VOLUMES.get(vol_dir)
    if vol is None:
        err(where, f"unknown volume directory '{vol_dir}'")
        continue
    if m.fields.get("module") != m.path:
        err(where, f"'module' is '{m.fields.get('module')}', expected '{m.path}'")
    if m.fields.get("volume") != vol:
        err(where, f"'volume' is '{m.fields.get('volume')}', expected '{vol}'")
    for key in ("page", "option"):
        if key not in m.fields:
            err(where, f"missing '{key}'")
    if not m.assertions:
        err(where, "module has no assertions")
    if not (m.dir / (page + ".cov")).is_file():
        err(where, f"no {page}.cov: classify every requirement unit "
                   f"(ci/coverage.py --stub {m.path})")

    used_tests = {}
    prefix = f"{vol}.{page}."
    for a in m.assertions:
        here = f"{a.path.relative_to(args.modules)}:{a.line}"
        if not re.fullmatch(r"[A-Za-z0-9._-]+", a.id):
            err(here, f"malformed id '{a.id}'")
        if not a.id.startswith(prefix) or not re.fullmatch(r"\d{3}", a.id[len(prefix):]):
            err(here, f"id '{a.id}' should be {prefix}NNN")
        if a.id in by_id:
            err(here, f"duplicate id {a.id}")
        by_id[a.id] = (m, a)
        for k in a.fields:
            if k not in catalog.ASR_KEYS:
                err(here, f"unknown key '{k}'")
        cls = a.get("class", "required")
        if cls not in catalog.CLASSES:
            err(here, f"unknown class '{cls}'")
        since = a.get("since")
        if since and since not in ("1988", "1992", "2001", "2008", "2024"):
            err(here, f"'since {since}' is not an edition year "
                "(1988, 1992, 2001, 2008, 2024)")
        if not a.anchors():
            err(here, "no anchor")
        if not a.get("says"):
            err(here, "no 'says'")
        if cls == "untestable":
            if not a.get("reason"):
                err(here, "untestable assertion needs a 'reason'")
            continue
        test = a.get("test")
        if not test:
            continue        # reported as UNTESTED by the driver: a gap, not an error
        tpath = m.dir / test
        if not tpath.is_file():
            err(here, f"test file {test} does not exist")
            continue
        used_tests.setdefault(tpath.resolve(), []).append(a.id)
        ids, mutants = catalog.test_header(tpath)
        if ids != [a.id]:
            err(tpath.relative_to(args.modules),
                f"header names {ids or 'no assertion'}, expected [{a.id}]")
        if not mutants and not a.get("nomutant"):
            err(tpath.relative_to(args.modules),
                "declares no MUTANT (or give the assertion a 'nomutant' reason)")
        for mu in mutants:
            fn = mu.split(".", 1)[0]
            if tpath.suffix == ".c":
                srcs = list((args.modules / "xsh" / fn / "mutants").glob("*.c"))
                text = "".join(s.read_text() for s in srcs)
                fault = mu.split(".", 1)[1] if "." in mu else ""
                if not srcs or f'"{fault}"' not in text:
                    err(tpath.relative_to(args.modules),
                        f"mutant {mu} not implemented in xsh/{fn}/mutants/")
            else:
                mf = args.modules / "xcu" / fn / "mutants" / f"{mu}.sh"
                if not mf.is_file():
                    err(tpath.relative_to(args.modules),
                        f"mutant {mu} has no file {mf.relative_to(args.modules)}")
            mutant_pairs.append((a.id, mu))

    for t in sorted((m.dir / "t").glob("*")) if (m.dir / "t").is_dir() else []:
        if t.suffix not in (".c", ".sh"):
            continue
        users = used_tests.get(t.resolve(), [])
        if not users:
            err(t.relative_to(args.modules), "test is not used by any assertion")
        elif len(users) > 1:
            err(t.relative_to(args.modules), f"test is used by {users}")

# prerequisites: the module's "uses" and each assertion's
graph = {}
for m in mods:
    deps = set()
    for u in m.fields.get("uses", "").split():
        if u not in by_id:
            err(m.dir / "MODULE", f"uses unknown assertion {u}")
            continue
        if catalog.id_module(u) == m.path:
            err(m.dir / "MODULE", f"module uses its own assertion {u}")
        deps.add(catalog.id_module(u))
    order = [a.id for a in m.assertions]
    for a in m.assertions:
        here = f"{a.path.relative_to(args.modules)}:{a.line}"
        for u in a.get("uses").split():
            if u not in by_id:
                err(here, f"uses unknown assertion {u}")
            elif catalog.id_module(u) == m.path:
                if order.index(u) >= order.index(a.id):
                    err(here, f"uses {u}, which does not come earlier "
                              f"in the same module")
            else:
                deps.add(catalog.id_module(u))
    graph[m.path] = deps


def cycle_from(node, stack, seen):
    if node in stack:
        return stack[stack.index(node):] + [node]
    if node in seen:
        return None
    seen.add(node)
    for d in graph.get(node, ()):
        c = cycle_from(d, stack + [node], seen)
        if c:
            return c
    return None


seen = set()
for node in graph:
    c = cycle_from(node, [], seen)
    if c:
        err("modules", "prerequisite cycle: " + " -> ".join(c))
        break

# anchors against the standard
if args.standard:
    headings = {}
    for _, (m, a) in sorted(by_id.items()):
      for anchor in a.anchors():
        page, _, rest = anchor.partition("#")
        f = args.standard / page
        here = f"{a.path.relative_to(args.modules)}:{a.line}"
        if not f.is_file():
            err(here, f"anchor page {page} not in the standard")
            continue
        if page not in headings:
            text = f.read_text(encoding="latin-1")
            headings[page] = set(re.findall(r"<h\d[^>]*>(?:<a[^>]*></a>)?([^<]+)</h\d>", text))
            headings[page] |= {h.strip() for h in headings[page]}
            headings[page] |= set(re.findall(r"<h\d[^>]*>(?:<a[^>]*></a>)?(\d+(?:\.\d+)*)\s", text))
        section = rest.split("/", 1)[0]
        if section and section not in headings[page]:
            err(here, f"anchor section '{section}' not found on {page}")
        item = rest.split("/", 1)[1] if "/" in rest else ""
        if item and item not in f.read_text(encoding="latin-1"):
            err(here, f"anchor item '{item}' not found on {page}")

# RULES.md 2: the standard's text never enters the repository.
MARKERS = ("The IEEE and The Open Group, All Rights Reserved",
           "Copyright \u00a9 2001-2024 The IEEE and The Open Group")
for f in sorted(root.rglob("*")):
    if ".git" in f.parts or not f.is_file() or f.name == "RULES.md" \
            or f.resolve() == Path(__file__).resolve():
        continue
    try:
        text = f.read_text(encoding="utf-8", errors="ignore")
    except OSError:
        continue
    if any(m in text for m in MARKERS):
        err(f.relative_to(root), "contains text copied from the standard")

if args.list_mutants:
    for aid, mu in mutant_pairs:
        print(aid, mu)
    sys.exit(0)

for e in errors:
    print("lint:", e)
n_asr = len(by_id)
print(f"lint: {len(mods)} modules, {n_asr} assertions, {len(mutant_pairs)} "
      f"mutants, {len(errors)} problems")
sys.exit(1 if errors else 0)
