#!/usr/bin/env python3
# specmodel.py - build a model of what each header must declare, from the
# user's own copy of the standard (HTML edition).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Runs on a development machine. The model is derived from the standard,
# so it is written under build/ and never committed or bundled (RULES.md,
# rule 3). It records facts only: names, types, prototypes and the option
# that shades each of them.
#
# usage: gen/specmodel.py --standard DIR [--out build/spec/model.json]
#                         [--show HEADER]

import argparse
import html
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Option codes that do not make an item optional for a POSIX system.
BASE_CODES = {"CX", "OB", "OH"}


def header_name(page):
    """basedefs/sys_stat.h.html -> sys/stat.h (the standard's file naming)."""
    stem = page.name[:-len(".html")]
    special = {"sys_": "sys/", "netinet_": "netinet/", "arpa_": "arpa/", "net_": "net/"}
    for k, v in special.items():
        if stem.startswith(k):
            return v + stem[len(k):]
    return stem


def text(frag):
    """Plain text of an HTML fragment, whitespace collapsed."""
    t = re.sub(r"<[^>]+>", " ", frag)
    return re.sub(r"\s+", " ", html.unescape(t)).strip()


class Options:
    """Which option codes shade each position of the page."""

    MARK = re.compile(r"open_code\('[^']*'\)\">([^<]*)</a>|opt-start\.gif|opt-end\.gif")

    def __init__(self, page, resets=(), flat=()):
        """resets: positions (block starts) where shading starts afresh, so
        a span the page forgets to close does not leak into later blocks."""
        self.events = []
        pending = None
        depth = []
        resets = sorted(resets)
        ri = 0
        for m in self.MARK.finditer(page):
            while ri < len(resets) and resets[ri] <= m.start():
                if depth:
                    depth = []
                    self.events.append((resets[ri], []))
                ri += 1
            if m.group(1) is not None:
                pending = m.group(1).strip()
            elif "opt-start" in m.group(0):
                if any(a <= m.start() < b for a, b in flat):
                    # in a table, a new span replaces the one before it: the
                    # tables do not always close a span before the next
                    depth = []
                depth.append(pending or "?")
                pending = None
                self.events.append((m.end(), list(depth)))
            else:
                if depth:
                    depth.pop()
                self.events.append((m.start(), list(depth)))

    def at(self, pos):
        cur = []
        for p, st in self.events:
            if p > pos:
                break
            cur = st
        return cur


# MC1 is shorthand for any of the mutex priority options (XBD 1.8.1).
SHORTHAND = {"MC1": "RPI|RPP|TPI|TPP"}


def option_of(codes):
    """The option an item needs: BASE, or the innermost real option code
    (an alternation such as RPI|TPI is kept whole)."""
    for c in reversed(codes):
        for part in c.split():
            if part not in BASE_CODES and part != "?":
                return SHORTHAND.get(part, part)
    return "BASE"


def conditional(intro_text):
    """True when an intro says its names need not be defined."""
    t = intro_text.lower()
    return any(k in t for k in ("if defined", "only if", "not defined", "are defined in", "shall be omitted",
                                "if any of the following", "if the following", "whether defined",
                                "is defined in", "reserved for compatibility",
                                "if supported", "if and only if"))


def description(page):
    """(start, end) of the DESCRIPTION section in the page."""
    m = re.search(r'id="tag_\d+_\d+_\d+"></a>DESCRIPTION</h4>', page)
    if not m:
        return None
    n = re.search(r'<h4 class="mansect">', page[m.end():])
    return m.end(), (m.end() + n.start()) if n else len(page)


BLOCK = re.compile(r"<p\b[^>]*>.*?</p>|<dl\b.*?</dl>|<pre>.*?</pre>|<table\b.*?</table>", re.S)


def kind_of(intro):
    t = intro.lower()
    if "may be declared as functions, or defined as macros" in t or \
            "declared as functions, defined as macros, or both" in t:
        return "either"
    if "initializer" in t:
        return "macros"
    if "declared as functions" in t or "declared as a function" in t or \
            re.search(r"shall declare the following functions?", t) or \
            ("shall be declared" in t and "function" in t):
        return "functions"
    if "external variable" in t:
        return "variables"
    if "members" in t and ("structure" in t or "union" in t or " type " in t or "shall be defined as" in t):
        return "members"
    if "union shall be defined as" in t or "structure shall be defined as" in t:
        return "members"
    if re.search(r"following (macros|symbolic constants)", t):
        return "macros"
    if re.search(r"following (data )?types\b", t) or "following type" in t:
        return "types"
    if "macro" in t or "symbolic constant" in t or "constant" in t or "values" in t:
        return "macros"
    return None


def aggregate_of(intro_html):
    """('struct'|'union'|'type', name) named in an intro paragraph."""
    t = text(intro_html)
    for pat, kind in ((r"the struct (\w+)", "struct"), (r"the (\w+) structure", "struct"), (r"the structure (\w+)", "struct"),
                      (r"structure (\w+) which", "struct"), (r"the (\w+) union", "union"),
                      (r"the (\w+) type as a structure", "type"), (r"the (\w+) type as a union", "type"),
                      (r"define the (\w+) type .*members", "type"), (r"(\w+) union shall be defined", "union"),
                      (r"(\w+) structure shall be defined", "struct")):
        m = re.search(pat, t)
        if m:
            name = m.group(1)
            if kind == "struct" and name.endswith("_t"):
                kind = "type"	# a typedef name such as glob_t, not a tag
            return kind, name
    return None, None


DECL_NAME = re.compile(r"([A-Za-z_]\w*)\s*\(")


def decl_name(decl):
    """The declared name of a prototype: the identifier before the first
    '(' that is not a '(*'. For 'void (*signal(int, ...))(int)' it is signal."""
    for m in DECL_NAME.finditer(decl):
        name = m.group(1)
        if name not in ("const", "volatile", "restrict", "struct", "union", "enum", "unsigned",
                        "signed", "void", "char", "short", "int", "long", "float", "double"):
            return name
    return None


def split_decls(pre_html, base):
    """Prototypes in a <pre> block, each with its position in the page.
    A declaration ends at ';' or, when the ';' is missing, at a line that
    starts a new declaration after a closing ')'."""
    out = []
    # option tags such as [CX] are not part of the declarations; blank them
    # out (same length, so positions in the page stay right)
    pre_html = re.sub(r"<sup>.*?</sup>", lambda m: " " * len(m.group(0)), pre_html, flags=re.S)
    body = pre_html
    # positions: map each character of the plain text back to the html
    plain, where = [], []
    i = 0
    while i < len(pre_html):
        if pre_html[i] == "<":
            j = pre_html.find(">", i)
            i = j + 1 if j != -1 else len(pre_html)
            continue
        if pre_html[i] == "&":
            j = pre_html.find(";", i)
            ent = pre_html[i:j + 1]
            plain.append(html.unescape(ent))
            where.append(i)
            i = j + 1
            continue
        plain.append(pre_html[i])
        where.append(i)
        i += 1
    s = "".join(plain)
    pos_of = {}
    k = 0
    for idx, ch in enumerate(plain):
        for _ in ch:
            pos_of[k] = where[idx]
            k += 1
    del body
    cur, start, depth = "", None, 0
    lines = s.split("\n")
    off = 0
    for line in lines:
        stripped = line.strip()
        if stripped and start is None:
            start = off + (len(line) - len(line.lstrip()))
        if stripped:
            # a missing ';': a complete declaration followed by a new one
            if cur and depth == 0 and cur.rstrip().endswith(")") and re.match(r"[A-Za-z_]", stripped):
                out.append((cur.strip(), base + pos_of.get(start, 0), True))
                cur, start = "", off + (len(line) - len(line.lstrip()))
            cur += " " + stripped
            depth += stripped.count("(") - stripped.count(")")
            while ";" in cur and depth == 0:
                d, rest = cur.split(";", 1)
                out.append((d.strip(), base + pos_of.get(start, 0), False))
                cur = rest
                start = off + len(line) - len(rest.lstrip()) if rest.strip() else None
        off += len(line) + 1
    if cur.strip():
        out.append((cur.strip(), base + pos_of.get(start or 0, 0), True))
    return [(re.sub(r"\s+", " ", d), p, nosemi) for d, p, nosemi in out if d]


def members_of(pre_html, base, opt_item):
    """(type, name) members listed in a <pre>: each <tt> run that reads
    like a declaration."""
    out = []
    for m in re.finditer(r"<tt>(.*?)</tt>", pre_html, re.S):
        raw = m.group(1)
        clean = re.sub(r"<sup>.*?</sup>", " ", raw, flags=re.S)
        lines = re.sub(r"<[^>]+>", " ", clean).split("\n")
        for part in [x for line in lines for x in html.unescape(line).split(";")]:
            part = part.strip()
            fp = re.match(r"^(.*?)\(\s*\*\s*([A-Za-z_]\w*)\s*\)\s*\((.*)\)$", part)
            if fp:
                where = base + m.start(1) + max(raw.find(fp.group(2)), 0)
                out.append({"type": f"{fp.group(1).strip()} (*)({fp.group(3).strip()})", "name": fp.group(2),
                            "array": False, "option": opt_item(where)})
                continue
            mm = re.match(r"^(.*?[\w*)\]])\s*\b([A-Za-z_]\w*)\s*(\[[^\]]*\])?$", part)
            if not mm or not mm.group(1).strip():
                continue
            ty, name, arr = mm.group(1).strip(), mm.group(2), mm.group(3)
            if not re.match(r"^[A-Za-z_]", ty):
                continue
            where = base + m.start(1) + max(raw.find(name), 0)
            out.append({"type": ty, "name": name, "array": bool(arr), "option": opt_item(where)})
    return out


def members_of_table(tab_html, base, opt_item):
    out = []
    for r in re.finditer(r"<tr\b.*?</tr>", tab_html, re.S):
        cells = [text(c) for c in re.findall(r"<td\b.*?</td>", r.group(0), re.S)]
        if len(cells) >= 2 and re.match(r"^[A-Za-z_][\w\s*()]*$", cells[0]) and re.match(r"^\(?\*?[A-Za-z_]\w*", cells[1]):
            name = re.sub(r"[^\w].*$", "", cells[1].lstrip("(*"))
            out.append({"type": cells[0], "name": name, "array": "[" in cells[1],
                        "option": opt_item(base + r.start())})
    return out


def parse(page_path):
    page = page_path.read_text(errors="replace")
    span = description(page)
    if span is None:
        return None
    blocks = list(BLOCK.finditer(page, span[0], span[1]))
    opts = Options(page, [b.start() for b in blocks],
                   [(b.start(), b.end()) for b in blocks if b.group(0).startswith("<table")])
    hdr = header_name(page_path)
    syn = re.search(r"SYNOPSIS</h4>(.*?)</blockquote>", page, re.S)
    hm = re.search(r"open_code\('[^']*'\)\">([^<]*)</a>\]</sup>\s*<img[^>]*opt-start[^>]*>\s*#include", syn.group(1)) if syn else None
    header_opt = option_of([hm.group(1).strip()]) if hm else "BASE"
    model = {"header": hdr, "page": f"basedefs/{page_path.name}", "option": header_opt, "macros": [], "types": [],
             "aggregates": [], "functions": [], "variables": [], "notes": []}
    intro, intro_pos, intro_opt = "", 0, "BASE"
    lead = ""	# every paragraph since the last list: conditions may sit in any
    seen = set()

    def opt_item(p):
        o = option_of(opts.at(p))
        return intro_opt if o == "BASE" else o
    for b in BLOCK.finditer(page, span[0], span[1]):
        blk, pos = b.group(0), b.start()
        if re.match(r"<p[\s>]", blk):
            t = re.sub(r"\[ ?[A-Z0-9| ]+ ?\] ?", "", text(blk))
            t = re.sub(r"\s+([,.;:])", r"\1", t)	# text() leaves a space where a tag closed
            bare = re.sub(r"\[[A-Z0-9| ]+\]", " ", t).split()
            k0 = kind_of(text(intro)) if intro else None
            if k0 in ("macros", "types") and bare and all(re.match(r"^[A-Za-z_]\w*$", w) for w in bare):
                for w in bare:
                    i = blk.find(w)
                    key = ("m" if k0 == "macros" else "t", w)
                    if key in seen:
                        continue
                    seen.add(key)
                    item = {"name": w, "option": opt_item(pos + max(i, 0))}
                    if k0 == "macros":
                        item["group"] = text(intro)[:80]
                        item["conditional"] = conditional(lead)
                    model["macros" if k0 == "macros" else "types"].append(item)
                continue
            intro, intro_pos = blk, pos
            # a new definition sentence starts a new lead
            lead = t if re.search(r"shall (define|declare|provide)", t) else lead + " " + t
            im = re.match(r"<p[^>]*>\s*<sup>\[<a href=\"javascript:open_code\('[^']*'\)\">([^<]*)</a>\]</sup>", blk)
            intro_opt = option_of([im.group(1).strip()]) if im else "BASE"
            for m in re.finditer(r"shall (?:define|provide) the (\w+)\(\) macro", t):
                if ("m", m.group(1)) not in seen:
                    seen.add(("m", m.group(1)))
                    model["macros"].append({"name": m.group(1), "option": intro_opt,
                                            "group": "function-like macro", "function_like": True})
            m = re.search(r"shall provide a definition for the macro (\w+)", t)
            if m and ("m", m.group(1)) not in seen:
                seen.add(("m", m.group(1)))
                model["macros"].append({"name": m.group(1), "option": intro_opt, "group": t[:80]})
            # single-sentence definitions: "shall define the X, Y and Z types"
            m = re.search(r"shall define the (?:array |data |integer |following )?types? ((?:\w+, )*\w+,? (?:and|or) \w+)\b", t) or \
                re.search(r"shall define the ((?:\w+, )*\w+,? (?:and|or) \w+|\w+) types?\b(?! as a struct)", t)
            if m and "following" not in t and "members" not in t:
                for n in re.split(r",\s*(?:and\s+|or\s+)?|\s+and\s+|\s+or\s+", m.group(1)):
                    n = n.strip()
                    if n and ("t", n) not in seen and re.match(r"^\w+$", n):
                        seen.add(("t", n))
                        i = blk.find(f"<b>{n}</b>")
                        model["types"].append({"name": n, "option": opt_item(pos + i) if i >= 0 else intro_opt})
            m = re.search(r"shall define the (\w+) structure as described", t)
            if m:
                model["aggregates"].append({"kind": "struct", "name": m.group(1), "members": [],
                                            "option": intro_opt})
            for m in re.finditer(r"shall define (?:the )?(?:symbolic constants? |macros? )?([A-Z_][A-Z0-9_]*(?:(?:, | and | or |, and )[A-Z_][A-Z0-9_]*)*) (?:as described|as a macro|as defined)", t):
                for n in re.split(r",\s*(?:and\s+|or\s+)?|\s+and\s+|\s+or\s+", m.group(1)):
                    n = n.strip(" ,")
                    if n and ("m", n) not in seen and re.match(r"^[A-Z_][A-Z0-9_]*$", n):
                        seen.add(("m", n))
                        i = blk.find(n)
                        model["macros"].append({"name": n, "option": opt_item(pos + i) if i >= 0 else intro_opt, "group": t[:80],
                                                "conditional": False})
            continue
        k = kind_of(text(intro)) if intro else None
        if blk.startswith("<dl") and k in ("macros", "types"):
            for d in re.finditer(r"<dt>(.*?)</dt>\s*(<dd>.*?</dd>)?", blk, re.S):
                name = text(d.group(1)).strip("{}[]").strip()
                if not re.match(r"^[A-Za-z_]\w*$", name):
                    continue
                codes = opts.at(pos + d.start())
                if option_of(codes) == "BASE" and intro_opt != "BASE":
                    codes = codes + [intro_opt]
                # an item whose description is wholly shaded takes that option
                dd = re.sub(r"\s+", " ", d.group(2) or "")
                if option_of(codes) == "BASE" and dd:
                    mm = re.match(r"<dd>\s*<sup>\[<a href=\"javascript:open_code\('[^']*'\)\">([^<]*)</a>\]</sup>", dd)
                    # a description that opens with a shading tag gives the
                    # item that option (the span may run into a nested list)
                    if mm:
                        codes = codes + [mm.group(1).strip()]
                key = ("m" if k == "macros" else "t", name)
                if key in seen:
                    continue
                seen.add(key)
                item = {"name": name, "option": option_of(codes)}
                if k == "macros":
                    item["group"] = text(intro)[:80]
                    item["conditional"] = conditional(lead)
                    ddt = text(dd)
                    om = re.search(r"only if the implementation supports the (\w+) option", lead)
                    if item["conditional"] and om:
                        # defined exactly when an option is supported: that option's item
                        item["conditional"], item["option"] = False, SHORTHAND.get(om.group(1), om.group(1))
                    elif item["conditional"] and re.search(r"shall always be (set|defined)", ddt):
                        # an option constant that Issue 8 made mandatory
                        item["conditional"] = False
                        vm = re.search(r"shall always be set to (the value )?([0-9]+L?)\b", ddt)
                        item["always"] = vm.group(2) if vm else ("> 0" if "greater than zero" in ddt else "defined")
                    model["macros"].append(item)
                else:
                    model["types"].append(item)
        elif blk.startswith("<table") and k in ("macros", "types"):
            heads = [text(h).lower() for h in re.findall(r"<th\b.*?</th>", blk, re.S)]
            rows = [[text(c) for c in re.findall(r"<td\b.*?</td>", r.group(0), re.S)]
                    for r in re.finditer(r"<tr\b.*?</tr>", blk, re.S)]
            cols = [i for i, h in enumerate(heads)
                    if h in ("name", "code", "macro", "symbolic constant", "constant", "macro name", "identifier")]
            if not cols:
                # else every column that holds names (mostly identifiers)
                full = [r for r in rows if heads and len(r) == len(heads)]
                for i in range(len(heads)):
                    n = sum(1 for r in full if re.match(r"^\[?[A-Za-z_][A-Za-z0-9_]*\]?$", r[i]) and not re.match(r"^[0-9]", r[i]))
                    if n * 2 >= max(len(full), 1) and heads[i] not in ("value", "description", "type"):
                        cols.append(i)
                if not heads:
                    cols = [0]
            for r, col in [(r, col) for r in re.finditer(r"<tr\b.*?</tr>", blk, re.S) for col in cols]:
                cells = [text(c) for c in re.findall(r"<td\b.*?</td>", r.group(0), re.S)]
                if not cells:
                    continue
                # a row that spans fewer columns continues the one above it
                c = col - (len(heads) - len(cells)) if heads and len(cells) < len(heads) else col
                if c < 0 or c >= len(cells):
                    continue
                name = re.sub(r"\[[A-Z0-9| ]+\]", "", cells[c]).strip().strip("{}[]")
                if not re.match(r"^[A-Za-z_]\w*$", name):
                    continue
                key = ("m" if k == "macros" else "t", name)
                if key in seen:
                    continue
                seen.add(key)
                item = {"name": name, "option": opt_item(pos + r.start() + max(r.group(0).find(name), 0))}
                if k == "macros":
                    item["group"] = text(intro)[:80]
                    item["conditional"] = conditional(lead)
                    if len(cells) >= 3 and re.match(r"^[0-9]+$|^0x[0-9a-fA-F]+$|^'.'$|^\".*\"$", cells[-1]):
                        item["value"] = cells[-1]
                    model["macros"].append(item)
                else:
                    model["types"].append(item)
        elif blk.startswith("<pre") and k == "either":
            for mm in re.finditer(r"([A-Za-z_]\w*)\s*\(\s*\)", text(blk)):
                if ("f", mm.group(1)) not in seen:
                    seen.add(("f", mm.group(1)))
                    model["functions"].append({"name": mm.group(1), "decl": None, "either": True,
                                               "option": opt_item(pos + max(blk.find(mm.group(1)), 0))})
        elif blk.startswith("<pre") and k in ("functions", "variables"):
            for decl, p, nosemi in split_decls(blk, pos):
                name = decl_name(decl) if k == "functions" else None
                if k == "variables" or name is None:
                    d0 = re.sub(r"^extern\s+", "", decl)
                    mm = re.match(r"^((?:const\s+|unsigned\s+|signed\s+|struct\s+)*[A-Za-z_]\w*)\s*(.*)$", d0)
                    if mm:
                        base_t = mm.group(1)
                        for one in mm.group(2).split(","):
                            one = one.strip()
                            nm = re.search(r"([A-Za-z_]\w*)\s*(\[[^\]]*\])?$", one)
                            if nm:
                                stars = one[:nm.start()].strip()
                                model["variables"].append({"decl": f"{base_t} {one}", "name": nm.group(1),
                                                           "type": (base_t + " " + stars).strip() + (" " + nm.group(2) if nm.group(2) else ""),
                                                           "option": opt_item(p)})
                    continue
                if ("f", name) in seen:
                    continue
                seen.add(("f", name))
                f = {"name": name, "decl": decl, "option": opt_item(p)}
                if nosemi:
                    f["no_semicolon"] = True
                model["functions"].append(f)
        elif blk.startswith("<pre") and k == "members":
            kind, name = aggregate_of(intro)
            if name:
                model["aggregates"].append({"kind": kind, "name": name, "option": intro_opt,
                                            "members": members_of(blk, pos, opt_item)})
        elif blk.startswith("<table") and k == "members":
            kind, name = aggregate_of(intro)
            if name:
                model["aggregates"].append({"kind": kind, "name": name, "option": intro_opt,
                                            "members": members_of_table(blk, pos, opt_item)})
        intro = ""
        lead = ""
    # names that a sentence elsewhere makes conditional
    for b in blocks:
        t = text(b.group(0))
        for m in re.finditer(r"(\w+(?:(?:, | and | or |, and )\w+)*) shall be defined if and only if", t):
            for n in re.split(r",\s*(?:and\s+|or\s+)?|\s+and\s+|\s+or\s+", m.group(1)):
                for x in model["macros"]:
                    if x["name"] == n.strip():
                        x["conditional"] = True
    if header_opt != "BASE":
        for k in ("macros", "types", "functions", "variables", "aggregates"):
            for x in model[k]:
                if x["option"] == "BASE":
                    x["option"] = header_opt
                for mem in x.get("members", []):
                    if mem["option"] == "BASE":
                        mem["option"] = header_opt
    return model


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--standard", required=True, help="the user's copy of the standard (HTML)")
    ap.add_argument("--out", default=str(ROOT / "build/spec/model.json"))
    ap.add_argument("--show", help="print the model of one header")
    a = ap.parse_args()
    base = Path(a.standard) / "basedefs"
    models = []
    for page in sorted(base.glob("*.h.html")):
        m = parse(page)
        if m:
            models.append(m)
    if a.show:
        for m in models:
            if m["header"] == a.show:
                json.dump(m, sys.stdout, indent=1)
                print()
        return
    out = Path(a.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(models, indent=1))
    tot = {k: sum(len(m[k]) for m in models) for k in ("macros", "types", "functions", "variables")}
    mem = sum(len(g["members"]) for m in models for g in m["aggregates"])
    print(f"specmodel: {len(models)} headers, {tot['macros']} macros, {tot['types']} types, "
          f"{sum(len(m['aggregates']) for m in models)} structures ({mem} members), "
          f"{tot['functions']} functions, {tot['variables']} variables -> {out}")


if __name__ == "__main__":
    main()
