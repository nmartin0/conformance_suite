#!/usr/bin/env python3
# coverage.py - is every requirement on a page accounted for? (off-system)
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage:
#   ci/coverage.py --standard DIR                 report every module
#   ci/coverage.py --standard DIR --total         also size the whole standard
#   ci/coverage.py --standard DIR --show MODULE   print a page's units, with
#                                                 their text, to the terminal
#   ci/coverage.py --standard DIR --stub MODULE   print .cov lines for units
#                                                 not yet accounted for
#
# A "unit" is one statement on a page of the standard, in a normative
# section (the informative sections, from EXAMPLES on, are skipped):
#   - each sentence of a paragraph or list item (a requirement need not say
#     "shall"); its kind, for triage, is "shall", "unspec" (leaves something
#     unspecified, undefined or to the implementation), "item" or "stmt";
#   - each table row ("row") and each member of a preformatted member list
#     ("member"); any other preformatted block is one unit ("pre");
#   - for a utility, a standard section with no other unit (such as
#     "STDOUT: Not used."), whose meaning comes from XCU 1.4;
#   - each error under "shall fail" or "may fail".
# Each unit is identified by its section and a hash of its text, so the
# repository never holds the standard's words (RULES.md).
#
# Each module has a <page>.cov file mapping units to a disposition:
# (A section name's spaces become underscores: RETURN_VALUE:hash.)
#   SECTION:hash  XSH.open.001 [XSH.open.002 ...]   covered by assertions
#   SECTION:hash  app                               requirement on applications
#   SECTION:hash  impl  reason                      implementation-defined or
#                                                   documentation requirement
#   SECTION:hash  optional                          a "may fail" error
#   SECTION:hash  none  reason                      nothing testable (cross-
#                                                   reference, definition, or
#                                                   restates another unit)
#   SECTION:hash  untestable  reason
#   SECTION:hash  todo  reason                      known gap
#   SECTION:hash  partial XSH.open.015  # what is missing
#                                                   assertions cover only part
# Text after " #" on a line is a comment.
# Units with no line are "unaccounted". Exit status 1 when a .cov line names
# a unit that is not on the page (the text changed) or an unknown assertion.

import argparse
import hashlib
import html
import re
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import catalog  # noqa: E402

INFORMATIVE = {"EXAMPLES", "APPLICATION USAGE", "RATIONALE",
               "FUTURE DIRECTIONS", "SEE ALSO", "CHANGE HISTORY", "NAME",
               "SYNOPSIS"}
XCU_DEFAULT_SECTIONS = {"OPTIONS", "OPERANDS", "STDIN", "INPUT FILES",
                        "ENVIRONMENT VARIABLES", "ASYNCHRONOUS EVENTS",
                        "STDOUT", "STDERR", "OUTPUT FILES",
                        "EXTENDED DESCRIPTION", "EXIT STATUS",
                        "CONSEQUENCES OF ERRORS"}
KINDS = {"app", "impl", "optional", "none", "untestable", "todo", "partial"}
NEEDS_REASON = {"impl", "none", "untestable", "todo"}


def clean(fragment):
    t = re.sub(r"<[^>]+>", " ", fragment)
    t = html.unescape(t)
    t = re.sub(r"\s+", " ", t).strip()
    return re.sub(r"\s*The following sections are informative\.?$", "", t)


def option_spans(fragment):
    """[(option, start, end)]: the shaded spans of a fragment, as offsets
    into clean(fragment). A span starts at an opt-start image, named by
    the last open_code() before it, and ends at the next opt-end image
    (or the end of the fragment)."""
    spans = []
    for m in re.finditer(r'<img[^>]*opt-start[^>]*>', fragment):
        names = re.findall(r"open_code\('([A-Z0-9|]+)'\)", fragment[:m.start()])
        if not names:
            continue
        e = re.search(r'<img[^>]*opt-end[^>]*>', fragment[m.end():])
        end = m.end() + e.start() if e else len(fragment)
        a = len(clean(fragment[:m.start()]))
        b = len(clean(fragment[:end]))
        for name in names[-1].split("|"):
            spans.append((name, a, b))
    return spans


def sentences(text):
    parts = re.split(r"(?<=[.;])\s+(?=[A-Z\[(\"'])", text)
    return [p.strip() for p in parts if p.strip()]


def unit_hash(text):
    norm = re.sub(r"\s+", " ", text).strip().lower()
    return hashlib.sha1(norm.encode()).hexdigest()[:8]


def page_sections(src, section=None):
    """[(section name, html)] for a reference page, or for one numbered
    section of a chapter page (section="2.6")."""
    if section:
        heads = [(m.start(), m.end(), m.group(2))
                 for m in re.finditer(
                     r"<h([2-5])[^>]*>(?:<a[^>]*></a>)?\s*(\d+(?:\.\d+)*)\s",
                     src)]
        start = end = None
        for i, (s0, e0, num) in enumerate(heads):
            if start is None and num == section:
                start = e0
                continue
            if start is not None and not num.startswith(section + "."):
                end = s0
                break
        if start is None:
            raise SystemExit(f"section {section} not found")
        return [(section, src[start:end])]
    parts = re.split(
        r'<h4 class="mansect">(?:<a[^>]*></a>)?([^<]+)</h4>', src)
    return [(parts[i].strip(), parts[i + 1])
            for i in range(1, len(parts) - 1, 2)]


def units_of(path, section=None, xcu=False):
    """[(unit id, kind, label, options, text)] in page order."""
    src = path.read_text(encoding="latin-1")
    out = []
    seen = Counter()

    def add(sec, kind, label, opts, text):
        sec = sec.replace(" ", "_")     # "RETURN VALUE" -> RETURN_VALUE
        h = unit_hash(text)
        seen[(sec, h)] += 1
        uid = f"{sec}:{h}" + (f".{seen[(sec, h)]}" if seen[(sec, h)] > 1 else "")
        out.append((uid, kind, label, opts, text))

    for sec, body in page_sections(src, section):
        if sec in ("NAME", "SYNOPSIS"):
            continue
        if sec in INFORMATIVE:
            break       # informative sections (and their subsections) close the page
        before = len(out)
        mode = None     # in ERRORS: "shall" or "may"
        label = ""
        for m in re.finditer(
                r"<dl\b[^>]*>|<(p|dt|dd|li|tr|pre)\b[^>]*>(.*?)</\1>",
                body, re.S):
            if m.group(1) is None:      # a new list: forget the last label
                label = ""
                continue
            tag, inner = m.group(1), m.group(2)
            if tag == "pre":
                # A member list ("<tt>type name </tt>description", with
                # continuation lines under an empty <tt>): one unit per
                # member. Any other block (a grammar, a format): one unit.
                segs = re.split(r"<tt>(.*?)</tt>", inner, flags=re.S)
                members = []
                for i in range(1, len(segs), 2):
                    decl, desc = clean(segs[i]), clean(segs[i + 1])
                    if decl:
                        members.append([decl, desc])
                    elif members and desc:
                        members[-1][1] += " " + desc
                if any(desc for _, desc in members):
                    for decl, desc in members:
                        if desc:
                            add(sec, "member", "", [], f"{decl} {desc}")
                elif clean(inner):
                    add(sec, "pre", "", [], clean(inner))
                continue
            # Table rows (struct members, constants) are one unit each,
            # cells joined; header rows (<th> only) are skipped. A table
            # may sit inside a list item; its rows are units of their own.
            rows = [inner] if tag == "tr" else \
                re.findall(r"<tr\b[^>]*>(.*?)</tr>", inner, re.S)
            for row in rows:
                cells = [clean(c) for c in
                         re.findall(r"<td\b[^>]*>(.*?)</td>", row, re.S)]
                text = " | ".join(c for c in cells if c)
                if text:
                    add(sec, "row", "", [], text)
            if tag == "tr":
                continue
            inner = re.sub(r"<(?:table|pre)\b.*?</(?:table|pre)>", " ",
                           inner, flags=re.S)
            text = clean(inner)
            if not text:
                continue
            opts = re.findall(r"open_code\('([A-Z0-9|]+)'\)", inner)
            if tag == "dt":
                label = text
                continue
            low = text.lower()
            if sec == "ERRORS" and tag == "p":
                if "shall fail" in low:
                    mode = "shall"
                elif "may fail" in low:
                    mode = "may"
                if "shall not return" in low or "shall not set" in low:
                    add(sec, "shall", "", opts, text)
                continue
            if sec == "ERRORS" and tag == "dd" and mode:
                add(sec, "error-" + mode, label, opts, f"{label} {text}")
                continue
            # Every sentence of a normative section counts: a requirement
            # need not say "shall" ("the file is determined relative to
            # fd"). The kind only helps triage: "shall"; "unspec" (the
            # sentence leaves something unspecified, undefined or to the
            # implementation); "item" (a list item); "stmt" (any other
            # sentence of a paragraph).
            spans = option_spans(inner)
            pos = 0
            for s in sentences(text):
                if len(s) < 4:
                    continue
                # Options are per shaded span, not per paragraph: a
                # sentence carries the options whose span it overlaps.
                at = text.find(s, pos)
                if at < 0:
                    at = pos
                pos = at + len(s)
                sopts = [o for o, a, b in spans if a < pos - 1 and b > at + 1]
                if not spans:
                    sopts = opts
                if re.search(r"\bshall\b", s):
                    kind = "shall"
                elif re.search(r"\b(unspecified|undefined|implementation-"
                               r"defined|need not)\b", s):
                    kind = "unspec"
                else:
                    kind = "stmt" if tag == "p" else "item"
                add(sec, kind, label if tag != "p" else "", sopts, s)
            if tag == "p":
                label = ""
        if xcu and sec in XCU_DEFAULT_SECTIONS and len(out) == before:
            text = clean(body)
            if text:
                add(sec, "default", "", [], text)
    return out


def read_cov(path):
    entries = {}
    if not path.is_file():
        return entries
    for n, line in enumerate(path.read_text().splitlines(), 1):
        comment = ""
        if " #" in line:
            line, comment = line.split(" #", 1)
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(None, 2)
        uid = parts[0]
        disp = parts[1] if len(parts) > 1 else ""
        rest = parts[2] if len(parts) > 2 else ""
        entries[uid] = (n, disp, rest, comment.strip())
    return entries


def classify(disp, rest):
    if disp in KINDS:
        return disp
    return "asserted"


def main():
    root = Path(__file__).resolve().parent.parent
    ap = argparse.ArgumentParser()
    ap.add_argument("--standard", type=Path, required=True)
    ap.add_argument("--modules", type=Path, default=root / "modules")
    ap.add_argument("--show")
    ap.add_argument("--stub")
    ap.add_argument("--total", action="store_true")
    a = ap.parse_args()

    mods = {m.path: m for m in catalog.all_modules(a.modules)}
    all_ids = {x.id for m in mods.values() for x in m.assertions}

    def module_units(m):
        page = a.standard / m.fields["page"]
        return units_of(page, m.fields.get("section"),
                        xcu=m.path.startswith("xcu/"))

    if a.show or a.stub:
        m = mods.get(a.show or a.stub)
        if m is None:
            raise SystemExit(f"no module {a.show or a.stub}")
        cov = read_cov(m.dir / (m.path.split("/", 1)[1] + ".cov"))
        for uid, kind, label, opts, text in module_units(m):
            if a.stub:
                if uid not in cov:
                    print(f"{uid:<24} todo  classify")
                continue
            d = cov.get(uid)
            mark = f"{d[1]} {d[2]}".strip() if d else "UNACCOUNTED"
            if d and d[3]:
                mark += f"  # {d[3]}"
            o = f" [{','.join(opts)}]" if opts else ""
            print(f"{uid} ({kind}{o}{' ' + label if label else ''}) -> {mark}")
            print(f"    {text}\n")
        return

    problems = 0
    totals = Counter()
    print(f"{'module':<38} {'units':>5} {'assert':>6} {'part.':>5} "
          f"{'class.':>6} {'todo':>4} {'unacc.':>6}")
    for path, m in sorted(mods.items()):
        units = module_units(m)
        cov = read_cov(m.dir / (path.split("/", 1)[1] + ".cov"))
        ids = {u[0] for u in units}
        c = Counter()
        for uid, (n, disp, rest, comment) in cov.items():
            where = f"{path}.cov:{n}"
            if uid not in ids:
                print(f"coverage: {where}: unit {uid} is not on the page "
                      f"(text changed?)")
                problems += 1
                continue
            k = classify(disp, rest)
            if k in ("asserted", "partial"):
                ids_ = (disp + " " + rest).split()[1 if k == "partial" else 0:]
                if k == "partial" and not comment:
                    print(f"coverage: {where}: 'partial' needs a comment "
                          f"saying what is missing")
                    problems += 1
                for aid in ids_:
                    if aid not in all_ids:
                        print(f"coverage: {where}: unknown assertion {aid}")
                        problems += 1
            elif k in NEEDS_REASON and not rest:
                print(f"coverage: {where}: '{k}' needs a reason")
                problems += 1
            c[k] += 1
        unacc = len(ids - set(cov))
        classified = c["app"] + c["impl"] + c["optional"] + c["none"] + \
            c["untestable"]
        print(f"{path:<38} {len(units):>5} {c['asserted']:>6} "
              f"{c['partial']:>5} {classified:>6} {c['todo']:>4} {unacc:>6}")
        totals.update({"units": len(units), "asserted": c["asserted"],
                       "partial": c["partial"], "classified": classified,
                       "todo": c["todo"], "unacc": unacc})
    print(f"{'all modules':<38} {totals['units']:>5} {totals['asserted']:>6} "
          f"{totals['partial']:>5} {totals['classified']:>6} "
          f"{totals['todo']:>4} {totals['unacc']:>6}")

    if a.total:
        n = Counter()
        for vol, d in (("XSH", "functions"), ("XCU", "utilities"),
                       ("XBD", "basedefs")):
            for p in sorted((a.standard / d).glob("*.html")):
                if p.name.startswith("V") or p.name == "contents.html":
                    continue
                try:
                    n[vol] += len(units_of(p, xcu=(vol == "XCU")))
                except SystemExit:
                    pass
        print("\nwhole standard (reference pages; chapters counted per "
              "module):", ", ".join(f"{k} {v}" for k, v in n.items()),
              f"= {sum(n.values())} units")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
