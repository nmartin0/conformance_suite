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
# --list-mutants prints "ID MUTANT" pairs for ci/mutate.sh; --list-variants
# prints "ID VARIANT [reading|shim]" (behaviour the text allows, which must
# PASS; a reading plants the other reading of a disputed assertion; a shim
# only supplies declarations a reference system's headers leave out).

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
ap.add_argument("--list-variants", action="store_true")
args = ap.parse_args()

errors = []


def err(where, msg):
    errors.append(f"{where}: {msg}")


mods = catalog.all_modules(args.modules)
# option codes an assertion's "when" may combine with "|" (either) and "+" (both)
OPTION_CODES = {"BASE"} | {l.split()[0] for l in (root / "core/options.list").read_text().splitlines()
                           if l.strip() and not l.startswith("#")}
by_id = {}
mutant_pairs = []
variant_pairs = []

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
        when = a.get("when")
        if when:
            for code in re.split(r"[|+]", when):
                if code not in OPTION_CODES:
                    err(here, f"'when {when}': unknown option code '{code}' (core/options.list)")
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
        # Names the driver creates must fit the results directory's {NAME_MAX}
        # budget (PC_NAME_NEED in posixcheck.sh): <id>.build.txt, <test>.variant.
        if len(a.id) + len(".build.txt") > 64:
            err(here, f"assertion ID {a.id} is too long for the 64-byte name budget")
        if len(tpath.stem) + len(".variant") > 64:
            err(here, f"test file name {tpath.name} is too long for the 64-byte name budget")
        ids, mutants, variants = catalog.test_header_full(tpath)
        readings = catalog.test_readings(tpath)
        if readings and cls != "disputed":
            err(tpath.relative_to(args.modules), "READING lines need a disputed assertion")
        if ids != [a.id]:
            err(tpath.relative_to(args.modules),
                f"header names {ids or 'no assertion'}, expected [{a.id}]")
        if not mutants and not a.get("nomutant"):
            err(tpath.relative_to(args.modules),
                "declares no MUTANT (or give the assertion a 'nomutant' reason)")
        for mu in mutants:
            if mu.count(".") != 1 and not (args.modules / "xsh" / mu.rsplit(".", 1)[0]).is_dir() \
                    and not (args.modules / "xcu" / mu.rsplit(".", 1)[0]).is_dir():
                err(tpath.relative_to(args.modules), f"mutant {mu}: the part after the last dot names the fault, the rest a module")
            fn = mu.rsplit(".", 1)[0]
            if tpath.suffix == ".c":
                srcs = list((args.modules / "xsh" / fn / "mutants").glob("*.c"))
                text = "".join(s.read_text() for s in srcs)
                fault = mu.rsplit(".", 1)[1] if "." in mu else ""
                if not srcs or f'"{fault}"' not in text:
                    err(tpath.relative_to(args.modules),
                        f"mutant {mu} not implemented in xsh/{fn}/mutants/")
            else:
                mf = args.modules / "xcu" / fn / "mutants" / f"{mu}.sh"
                if not mf.is_file():
                    err(tpath.relative_to(args.modules),
                        f"mutant {mu} has no file {mf.relative_to(args.modules)}")
            mutant_pairs.append((a.id, mu))
        for va in variants + readings:
            if va.count(".") != 1 and not (args.modules / "xsh" / va.rsplit(".", 1)[0]).is_dir() \
                    and not (args.modules / "xcu" / va.rsplit(".", 1)[0]).is_dir():
                err(tpath.relative_to(args.modules), f"variant {va}: the part after the last dot names the variant, the rest a module")
            fn = va.rsplit(".", 1)[0]
            name = va.rsplit(".", 1)[1] if "." in va else ""
            if va in mutants:
                err(tpath.relative_to(args.modules), f"{va} is declared both MUTANT and VARIANT")
            if tpath.suffix == ".c":
                vdir = args.modules / "xsh" / fn / "variants"
                srcs = list(vdir.glob("*.c"))
                text = "".join(s.read_text() for s in srcs)
                if not (vdir / f"{va}.h").is_file() and (not srcs or f'"{name}"' not in text):
                    err(tpath.relative_to(args.modules),
                        f"variant {va} not implemented in xsh/{fn}/variants/")
            else:
                vf = args.modules / "xcu" / fn / "variants" / f"{va}.sh"
                if not vf.is_file():
                    err(tpath.relative_to(args.modules),
                        f"variant {va} has no file {vf.relative_to(args.modules)}")
            # A build shim (a header alone marked "posixcheck: shim") only
            # supplies what a reference system's headers leave out; what
            # runs is that system's own behaviour (ci/mutate.sh: SYSTEM).
            hdr = args.modules / "xsh" / fn / "variants" / f"{va}.h"
            shim = tpath.suffix == ".c" and hdr.is_file() and "posixcheck: shim" in hdr.read_text()
            if shim and va in readings:
                err(tpath.relative_to(args.modules), f"{va} is a build shim and cannot be a READING")
            variant_pairs.append((a.id, va + (" reading" if va in readings else " shim" if shim else "")))

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
            # A heading may be wrapped in <i> or <b> (some section headings are).
            headings[page] = set(re.findall(r"<h\d[^>]*>(?:<[ib]>)?(?:<a[^>]*></a>)?([^<]+)(?:</[ib]>)?</h\d>", text))
            headings[page] |= {h.strip() for h in headings[page]}
            headings[page] |= set(re.findall(r"<h\d[^>]*>(?:<[ib]>)?(?:<a[^>]*></a>)?(\d+(?:\.\d+)*)\s", text))
        section = rest.split("/", 1)[0]
        if section and section not in headings[page]:
            err(here, f"anchor section '{section}' not found on {page}")
        item = rest.split("/", 1)[1] if "/" in rest else ""
        if item and item not in f.read_text(encoding="latin-1"):
            err(here, f"anchor item '{item}' not found on {page}")

# XBD.2-conformance.001 hard-codes the mandatory utilities: keep the list in
# step with the XCU modules whose option is BASE (localedef is left out).
_t = args.modules / "xbd/2-conformance/t/001-mandatory-utilities.sh"
if _t.is_file():
    _txt = _t.read_text()
    _body = _txt[_txt.index("for u in") + 8:_txt.index("; do")].replace("\\\n", " ")
    _have = set(_body.replace("'['", "[").split())
    _want = {"[", "test"}
    for _m in mods:
        if _m.path.startswith("xcu/") and _m.fields.get("option") == "BASE" \
                and not _m.path[4:5].isdigit() and not _m.path.startswith("xcu/sh-") \
                and _m.path != "xcu/localedef":
            _want.add({"colon": ":", "dot": "."}.get(_m.path[4:], _m.path[4:]))
    if _have != _want:
        err(_t.relative_to(args.modules), "utility list differs from the BASE xcu modules: "
            f"missing {sorted(_want - _have)}, extra {sorted(_have - _want)}")

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

if args.list_variants:
    for aid, va in variant_pairs:
        print(aid, va)
    sys.exit(0)

if args.list_mutants:
    for aid, mu in mutant_pairs:
        print(aid, mu)
    sys.exit(0)

for e in errors:
    print("lint:", e)
n_asr = len(by_id)
print(f"lint: {len(mods)} modules, {n_asr} assertions, {len(mutant_pairs)} "
      f"mutants, {len(variant_pairs)} variants, {len(errors)} problems")
sys.exit(1 if errors else 0)
