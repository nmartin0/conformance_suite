# catalog.py - read modules, assertions and test headers (off-system tools).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Shared by ci/lint.py, ci/mutate.sh (via lint.py --list-mutants) and
# analysis/analyze.py. The parsing rules match posixcheck.sh: "key value"
# lines, blank or "#" lines ignored, indented lines continue the previous
# value.

import re
from dataclasses import dataclass, field
from pathlib import Path

VOLUMES = {"xbd": "XBD", "xsh": "XSH", "xcu": "XCU", "st": "ST",
           # the ABI suite (abi/modules): module <architecture>/<area>, IDs ABI.<architecture>.<area>.NNN
           "i386": "ABI.i386", "x86_64": "ABI.x86_64"}
# Volume directories that name an architecture rather than a volume of the standard.
ARCHITECTURES = {"i386", "x86_64"}
ASR_KEYS = {"assertion", "anchor", "class", "when", "since", "says", "test",
            "needs", "timeout", "expect", "reason", "origin", "note", "uses",
            "nomutant"}
# Keys that may repeat; their values are kept as a list.
LIST_KEYS = {"anchor"}
CLASSES = {"required", "conditional", "profile", "inspect", "untestable",
           "disputed", "retired"}


@dataclass
class Assertion:
    id: str
    path: Path
    line: int
    fields: dict = field(default_factory=dict)

    def get(self, key, default=""):
        v = self.fields.get(key, default)
        return " ".join(v) if isinstance(v, list) else v

    def anchors(self):
        return list(self.fields.get("anchor", []))


@dataclass
class Module:
    path: str            # e.g. "xsh/open"
    dir: Path
    fields: dict
    assertions: list


def parse_kv(path):
    """Yield (line_number, key, value) with continuation lines joined."""
    key = None
    for n, raw in enumerate(path.read_text().splitlines(), 1):
        if not raw.strip():
            key = None
            continue
        if raw[:1] in " \t":
            if key is not None:
                yield n, key, raw.strip(), True
            continue
        if raw.startswith("#"):
            continue
        parts = raw.split(None, 1)
        key = parts[0]
        yield n, key, parts[1].strip() if len(parts) > 1 else "", False


def read_module(modroot, relpath):
    d = modroot / relpath
    fields = {}
    for _, k, v, cont in parse_kv(d / "MODULE"):
        if cont:
            continue
        fields[k] = (fields[k] + " " + v) if k in fields and k == "note" else v
    assertions = []
    for asr in sorted(d.glob("*.asr")):
        cur = None
        for n, k, v, cont in parse_kv(asr):
            if k == "assertion" and not cont:
                cur = Assertion(v, asr, n)
                assertions.append(cur)
                continue
            if cur is None:
                continue
            if k in LIST_KEYS:
                if cont:
                    cur.fields[k][-1] += " " + v
                else:
                    cur.fields.setdefault(k, []).append(v)
            elif cont:
                cur.fields[k] = cur.fields.get(k, "") + " " + v
            else:
                cur.fields[k] = v
    return Module(relpath, d, fields, assertions)


def all_modules(modroot):
    mods = []
    for mf in sorted(modroot.glob("*/*/MODULE")):
        rel = mf.parent.relative_to(modroot).as_posix()
        mods.append(read_module(modroot, rel))
    return mods


def id_module(aid):
    """XSH.close.001 -> xsh/close; ABI.i386.entry.001 -> i386/entry"""
    if aid.startswith("ABI."):
        arch, rest = aid[4:].split(".", 1)
        return f"{arch}/{rest.rsplit('.', 1)[0]}"
    vol, rest = aid.split(".", 1)
    page = rest.rsplit(".", 1)[0]
    for low, up in VOLUMES.items():
        if up == vol:
            return f"{low}/{page}"
    return None


HEADER_RE = re.compile(r"^\s*(?:/?\*|#)\s*(ASSERTION|MUTANT|VARIANT|READING)\s+(\S+)\s*$")


def test_header(path):
    """Return (assertion ids, mutant names) declared in a test's header."""
    ids, mutants, _ = test_header_full(path)
    return ids, mutants


def test_header_full(path):
    """Return (assertion ids, mutant names, variant names) declared in a test's header.

    A MUTANT is a planted fault the test must FAIL; a VARIANT is behaviour the
    standard allows (but the reference systems may not show) that the test
    must PASS.
    """
    ids, mutants, variants = [], [], []
    for line in path.read_text().splitlines()[:60]:
        m = HEADER_RE.match(line)
        if m:
            if m.group(1) != "READING":
                {"ASSERTION": ids, "MUTANT": mutants, "VARIANT": variants}[m.group(1)].append(m.group(2))
    return ids, mutants, variants


def test_readings(path):
    """Return the READING names a test declares.

    A READING is a variant that plants the other reading of a disputed
    assertion: the test may PASS or log INSPECT under it, never a plain FAIL.
    """
    out = []
    for line in path.read_text().splitlines()[:60]:
        m = HEADER_RE.match(line)
        if m and m.group(1) == "READING":
            out.append(m.group(2))
    return out
