#!/usr/bin/env python3
# analyze.py - analyze posixcheck run logs on another computer.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Nothing here runs on the system under test. Results directories are
# copied off each system and loaded into one SQLite database; every report
# is a query over it. The raw logs are never modified.
#
#   analyze.py load DB RESULTS... [--name NAME]   add runs (checks integrity)
#   analyze.py runs DB                            list loaded runs
#   analyze.py verdict DB [RUN]                   per-tier verdicts
#   analyze.py failures DB [RUN]                  FAILs with assertion text
#   analyze.py matrix DB                          assertions x runs, by pattern
#   analyze.py diff DB RUN_A RUN_B                what changed between runs
#   analyze.py coverage DB                        expected/recorded/run counts
#   analyze.py html DB OUT.html                   static report, no server
#   analyze.py calibrate DB RUN KNOWN...          check a reference run
#                                                 against its expected
#                                                 failures (known/*.known)
#
# Options for load: --modules DIR (default: the suite's modules/),
# --waivers DIR (default: the suite's waivers/).

import argparse
import fnmatch
import hashlib
import html
import signal
import sqlite3
import sys
from datetime import date
from pathlib import Path

root = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(root / "ci"))
import catalog  # noqa: E402

CODES = ["PASS", "FAIL", "UNRESOLVED", "NOTINUSE", "UNSUPPORTED", "UNTESTED",
         "UNINITIATED", "INSPECT"]
BLOCKING = {"UNRESOLVED", "UNINITIATED", "INSPECT"}
NEUTRAL = {"NOTINUSE", "UNSUPPORTED", "UNTESTED"}

SCHEMA = """
CREATE TABLE IF NOT EXISTS runs (
  run_id TEXT PRIMARY KEY, name TEXT, sysname TEXT, release TEXT,
  machine TEXT, sh TEXT, cc TEXT, suite TEXT, tree TEXT, started TEXT,
  finished TEXT, aborted TEXT, log_sha256 TEXT, source TEXT);
CREATE TABLE IF NOT EXISTS claims (run_id TEXT, code TEXT, claimed TEXT,
  PRIMARY KEY (run_id, code));
CREATE TABLE IF NOT EXISTS results (run_id TEXT, assertion TEXT, code TEXT,
  ms INTEGER, note TEXT, PRIMARY KEY (run_id, assertion));
CREATE TABLE IF NOT EXISTS assertions (assertion TEXT PRIMARY KEY,
  module TEXT, class TEXT, option TEXT, since TEXT, anchor TEXT, says TEXT);
CREATE TABLE IF NOT EXISTS waivers (assertion TEXT, system TEXT,
  version TEXT, expires TEXT, reason TEXT);
"""


def suite_version():
    for line in (root / "posixcheck.sh").read_text().splitlines():
        if line.startswith("pc_version="):
            return line.split("=", 1)[1].strip()
    return "?"


def connect(db):
    con = sqlite3.connect(db)
    con.executescript(SCHEMA)
    return con


def load_catalog(con, modroot):
    con.execute("DELETE FROM assertions")
    for m in catalog.all_modules(modroot):
        for a in m.assertions:
            con.execute(
                "INSERT OR REPLACE INTO assertions VALUES (?,?,?,?,?,?,?)",
                (a.id, m.path, a.get("class", "required"),
                 a.get("when", m.fields.get("option", "BASE")),
                 a.get("since"), "; ".join(a.anchors()), a.get("says")))


def load_waivers(con, wdir):
    con.execute("DELETE FROM waivers")
    if not wdir.is_dir():
        return
    for f in sorted(wdir.glob("*.waiver")):
        for line in f.read_text().splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            parts = line.split(None, 4)
            if len(parts) < 5:
                raise SystemExit(f"{f}: waiver needs: assertion system "
                                 f"version expires reason")
            con.execute("INSERT INTO waivers VALUES (?,?,?,?,?)", parts)


def cmd_load(a):
    con = connect(a.db)
    load_catalog(con, a.modules)
    load_waivers(con, a.waivers)
    want = suite_version()
    for rdir in a.results:
        log = Path(rdir) / "run.log"
        if not log.is_file():
            raise SystemExit(f"{rdir}: no run.log")
        raw = log.read_bytes()
        hdr, rows, claims = {}, [], {}
        aborted = None
        for line in raw.decode("utf-8", "replace").splitlines():
            if line.startswith("# "):
                body = line[2:]
                key, _, val = body.partition(" ")
                if key == "claim":
                    p = val.split()
                    if len(p) >= 3:
                        claims[p[0]] = p[2]
                elif key == "ABORT":
                    aborted = val
                else:
                    hdr.setdefault(key, val)
                continue
            p = line.split("\t")
            if len(p) != 4 or p[0] not in CODES:
                raise SystemExit(f"{log}: malformed line: {line!r}")
            rows.append(p)
        if hdr.get("posixcheck") != want:
            raise SystemExit(f"{log}: written by posixcheck "
                             f"{hdr.get('posixcheck')}, this suite is {want}")
        name = a.name or f"{hdr.get('sysname','?')}-{hdr.get('release','?')}"
        run_id = f"{name}@{hdr.get('started','?')}"
        con.execute("DELETE FROM runs WHERE run_id=?", (run_id,))
        con.execute("DELETE FROM results WHERE run_id=?", (run_id,))
        con.execute("DELETE FROM claims WHERE run_id=?", (run_id,))
        con.execute(
            "INSERT INTO runs VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
            (run_id, name, hdr.get("sysname"), hdr.get("release"),
             hdr.get("machine"), hdr.get("sh"), hdr.get("cc"),
             hdr.get("posixcheck"), hdr.get("tree"), hdr.get("started"),
             hdr.get("finished"), aborted, hashlib.sha256(raw).hexdigest(),
             str(Path(rdir).resolve())))
        for code, claimed in claims.items():
            con.execute("INSERT INTO claims VALUES (?,?,?)",
                        (run_id, code, claimed))
        for code, aid, ms, note in rows:
            con.execute("INSERT OR REPLACE INTO results VALUES (?,?,?,?,?)",
                        (run_id, aid, code, int(ms or 0), note))
        print(f"loaded {run_id}: {len(rows)} results"
              + (f" (ABORTED: {aborted})" if aborted else ""))
    con.commit()


def runs(con, which=None):
    q = "SELECT run_id, name, release FROM runs"
    rows = con.execute(q + " ORDER BY name, run_id").fetchall()
    if which:
        rows = [r for r in rows if r[0] == which or r[1] == which]
        if not rows:
            raise SystemExit(f"no run {which}")
    return rows


def waived(con, run_id, aid):
    sysname, release = con.execute(
        "SELECT name, release FROM runs WHERE run_id=?", (run_id,)).fetchone()
    today = date.today().isoformat()
    for _, system, version, expires, reason in con.execute(
            "SELECT * FROM waivers WHERE assertion=?", (aid,)):
        if expires < today:
            continue
        if fnmatch.fnmatch(sysname or "", system) and \
                fnmatch.fnmatch(release or "", version):
            return reason
    return None


def tiers_for(option):
    return option.split("|")


def cmd_verdict(a):
    con = connect(a.db)
    for run_id, name, _ in runs(con, a.run):
        claims = dict(con.execute(
            "SELECT code, claimed FROM claims WHERE run_id=?", (run_id,)))
        res = dict((r[0], r[1:]) for r in con.execute(
            "SELECT assertion, code, note FROM results WHERE run_id=?",
            (run_id,)))
        tiers = {}
        for aid, option, cls in con.execute(
                "SELECT assertion, option, class FROM assertions"):
            if aid.startswith("ST."):
                continue
            for t in tiers_for(option):
                tiers.setdefault(t, []).append(aid)
        aborted = con.execute("SELECT aborted FROM runs WHERE run_id=?",
                              (run_id,)).fetchone()[0]
        print(f"== {run_id}" + (f"  ABORTED: {aborted}" if aborted else ""))
        for t in sorted(tiers, key=lambda x: (x != "BASE", x)):
            if t != "BASE" and claims.get(t) != "yes":
                print(f"   {t:<6} not claimed")
                continue
            fails, blocking, notrun, gaps, waivedn = [], [], [], 0, 0
            for aid in tiers[t]:
                if aid not in res:
                    notrun.append(aid)
                    continue
                code = res[aid][0]
                if code == "FAIL":
                    if waived(con, run_id, aid):
                        waivedn += 1
                    else:
                        fails.append(aid)
                elif code in BLOCKING:
                    blocking.append(aid)
                elif code == "UNTESTED":
                    gaps += 1
            if fails:
                v = f"does not conform ({len(fails)} FAIL)"
            elif blocking or notrun or aborted:
                v = (f"no verdict ({len(blocking)} open, "
                     f"{len(notrun)} not run)")
            else:
                v = "conforms"
            extra = []
            if waivedn:
                extra.append(f"{waivedn} waived")
            if gaps:
                extra.append(f"{gaps} untested")
            print(f"   {t:<6} {v}" + (f"  [{', '.join(extra)}]" if extra else ""))


def cmd_failures(a):
    con = connect(a.db)
    for run_id, name, _ in runs(con, a.run):
        rows = con.execute(
            "SELECT r.assertion, r.note, s.says, s.anchor FROM results r "
            "LEFT JOIN assertions s USING (assertion) WHERE r.run_id=? AND "
            "r.code='FAIL' ORDER BY r.assertion", (run_id,)).fetchall()
        print(f"== {run_id}: {len(rows)} FAIL")
        for aid, note, says, anchor in rows:
            w = waived(con, run_id, aid)
            print(f"   {aid}" + (f"  (waived: {w})" if w else ""))
            print(f"      got:   {note}")
            print(f"      rule:  {says}")
            print(f"      where: {anchor}")


def matrix_rows(con):
    rids = [r[0] for r in runs(con)]
    data = {}
    for rid, aid, code in con.execute(
            "SELECT run_id, assertion, code FROM results"):
        data.setdefault(aid, {})[rid] = code
    rows = []
    for aid in sorted(data):
        if aid.startswith("ST."):
            continue
        codes = [data[aid].get(r, "-") for r in rids]
        judged = [c for c in codes if c in ("PASS", "FAIL")]
        nfail = judged.count("FAIL")
        if not judged or nfail == 0:
            pattern = "all pass" if judged else "no verdicts"
        elif nfail == 1 and len(judged) > 1:
            pattern = "one system fails"
        elif nfail == len(judged):
            pattern = "all fail"
        elif nfail > len(judged) / 2:
            pattern = "most fail"
        else:
            pattern = "split"
        rows.append((pattern, aid, codes))
    order = {"all fail": 0, "most fail": 1, "split": 2,
             "one system fails": 3, "no verdicts": 4, "all pass": 5}
    rows.sort(key=lambda r: (order[r[0]], r[1]))
    return rids, rows


HINTS = {
    "all fail": "suspect the test or our reading of the text",
    "most fail": "suspect the test or our reading of the text",
    "split": "ambiguous text or shared history: check interpretations",
    "one system fails": "likely a bug in that system",
}


def cmd_matrix(a):
    con = connect(a.db)
    rids, rows = matrix_rows(con)
    names = [r.split("@")[0] for r in rids]
    w = max([len(r[1]) for r in rows] + [10])
    print(f"{'assertion':<{w}}  " + "  ".join(f"{n[:12]:<12}" for n in names))
    last = None
    for pattern, aid, codes in rows:
        if pattern != last:
            print(f"-- {pattern}" + (f": {HINTS[pattern]}" if pattern in HINTS else ""))
            last = pattern
        print(f"{aid:<{w}}  " + "  ".join(f"{c[:12]:<12}" for c in codes))


def cmd_diff(a):
    con = connect(a.db)
    ra, rb = runs(con, a.run_a)[0][0], runs(con, a.run_b)[0][0]
    A = dict(con.execute("SELECT assertion, code FROM results WHERE run_id=?", (ra,)))
    B = dict(con.execute("SELECT assertion, code FROM results WHERE run_id=?", (rb,)))
    groups = {"regressions (now FAIL)": [], "fixes (now PASS)": [],
              "other changes": [], "only in A": [], "only in B": []}
    for aid in sorted(set(A) | set(B)):
        x, y = A.get(aid), B.get(aid)
        if x == y:
            continue
        if x is None:
            groups["only in B"].append((aid, "-", y))
        elif y is None:
            groups["only in A"].append((aid, x, "-"))
        elif y == "FAIL":
            groups["regressions (now FAIL)"].append((aid, x, y))
        elif y == "PASS":
            groups["fixes (now PASS)"].append((aid, x, y))
        else:
            groups["other changes"].append((aid, x, y))
    print(f"A = {ra}\nB = {rb}")
    for g, items in groups.items():
        if items:
            print(f"-- {g}: {len(items)}")
            for aid, x, y in items:
                print(f"   {aid:<44} {x:<12} -> {y}")
    if not any(groups.values()):
        print("no differences")


def cmd_coverage(a):
    con = connect(a.db)
    rows = con.execute(
        "SELECT module, COUNT(*), SUM(class='untestable') FROM assertions "
        "WHERE module NOT LIKE 'st/%' GROUP BY module ORDER BY module").fetchall()
    rids = [r[0] for r in runs(con)]
    print(f"{'module':<40} {'recorded':>8} {'untestable':>10} {'tested':>7}  run per system")
    for mod, n, unt in rows:
        tested = con.execute(
            "SELECT COUNT(*) FROM assertions WHERE module=? AND class!='untestable'",
            (mod,)).fetchone()[0]
        per = []
        for rid in rids:
            ran = con.execute(
                "SELECT COUNT(*) FROM results r JOIN assertions s USING (assertion) "
                "WHERE r.run_id=? AND s.module=? AND r.code IN ('PASS','FAIL')",
                (rid, mod)).fetchone()[0]
            per.append(f"{ran}/{n}")
        print(f"{mod:<40} {n:>8} {unt or 0:>10} {tested:>7}  " + " ".join(per))
    print("\n'expected' (requirement sentences per page) needs the "
          "completeness checker, not built yet.")


def cmd_html(a):
    con = connect(a.db)
    rids, rows = matrix_rows(con)
    says = dict(con.execute("SELECT assertion, says FROM assertions"))
    notes = {(r, i): n for r, i, n in con.execute(
        "SELECT run_id, assertion, note FROM results")}
    color = {"PASS": "#2e7d32", "FAIL": "#c62828", "UNRESOLVED": "#ef6c00",
             "UNINITIATED": "#ef6c00", "INSPECT": "#ef6c00"}
    out = ["<!doctype html><meta charset=utf-8><title>posixcheck report</title>",
           "<style>body{font:14px system-ui,sans-serif;margin:24px;color:#222}"
           "table{border-collapse:collapse}td,th{border:1px solid #ddd;"
           "padding:4px 8px;text-align:left;vertical-align:top}"
           "th{background:#f5f5f5}.p{font-weight:600;background:#fafafa}"
           "small{color:#666}</style>",
           "<h1>posixcheck report</h1>",
           f"<p>Suite {html.escape(suite_version())}. "
           "Unofficial; this does not certify conformance.</p>",
           "<table><tr><th>assertion</th>"
           + "".join(f"<th>{html.escape(r.split('@')[0])}</th>" for r in rids)
           + "</tr>"]
    last = None
    for pattern, aid, codes in rows:
        if pattern != last:
            hint = HINTS.get(pattern, "")
            out.append(f"<tr><td class=p colspan={len(rids) + 1}>"
                       f"{html.escape(pattern)} <small>{html.escape(hint)}</small></td></tr>")
            last = pattern
        cells = []
        for rid, c in zip(rids, codes):
            n = notes.get((rid, aid), "")
            cells.append(f"<td style='color:{color.get(c, '#555')}' "
                         f"title='{html.escape(n, True)}'>{html.escape(c)}</td>")
        out.append(f"<tr><td>{html.escape(aid)}<br><small>"
                   f"{html.escape(says.get(aid) or '')}</small></td>"
                   + "".join(cells) + "</tr>")
    out.append("</table>")
    Path(a.out).write_text("\n".join(out))
    print(f"wrote {a.out}")


def read_known(paths):
    """{assertion: (code, system pattern, reason)} from known/*.known."""
    out = {}
    for f in paths:
        for n, line in enumerate(Path(f).read_text().splitlines(), 1):
            line = line.split(" #", 1)[0].strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split(None, 3)
            if len(parts) < 4 or parts[1] not in CODES:
                raise SystemExit(f"{f}:{n}: expected: assertion code "
                                 f"system reason...")
            out[parts[0]] = (parts[1], parts[2], parts[3])
    return out


def cmd_calibrate(a):
    """A reference system (Linux, a BSD) is not the system being judged:
    its results judge the tests. Every non-PASS result should be a known,
    documented deviation; anything else means the test may be wrong."""
    con = connect(a.db)
    known = read_known(a.known)
    bad = 0
    for run_id, name, release in runs(con, a.run):
        print(f"== {name} ({release})")
        got = dict(((aid, (code, note)) for aid, code, note in con.execute(
            "SELECT assertion, code, note FROM results WHERE run_id=?",
            (run_id,))))
        for aid in sorted(set(got) | set(known)):
            code, note = got.get(aid, (None, ""))
            k = known.get(aid)
            if k and not fnmatch.fnmatch(name or "", k[1]):
                k = None
            if k:
                if code == k[0]:
                    print(f"EXPECTED  {aid:<34} {code:<10} {k[2]}")
                elif code is None:
                    continue        # not run here
                else:
                    print(f"STALE     {aid:<34} {code:<10} listed as {k[0]}: "
                          f"{k[2]}")
                    bad += 1
            elif code in ("FAIL", "UNINITIATED"):
                print(f"SUSPECT   {aid:<34} {code:<10} {note}")
                bad += 1
    print(f"calibrate: {bad} result(s) need a look (SUSPECT: the test or "
          f"the list is wrong; STALE: the list is out of date)")
    sys.exit(1 if bad else 0)


def main():
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)   # quiet under "| head"
    ap = argparse.ArgumentParser(description="analyze posixcheck logs")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("load")
    p.add_argument("db")
    p.add_argument("results", nargs="+")
    p.add_argument("--name")
    p.add_argument("--modules", type=Path, default=root / "modules")
    p.add_argument("--waivers", type=Path, default=root / "waivers")
    p.set_defaults(fn=cmd_load)
    for name, fn in (("runs", None), ("verdict", cmd_verdict),
                     ("failures", cmd_failures)):
        p = sub.add_parser(name)
        p.add_argument("db")
        p.add_argument("run", nargs="?")
        p.set_defaults(fn=fn or (lambda a: [print(r[0]) for r in runs(connect(a.db))]))
    for name, fn in (("matrix", cmd_matrix), ("coverage", cmd_coverage)):
        p = sub.add_parser(name)
        p.add_argument("db")
        p.set_defaults(fn=fn)
    p = sub.add_parser("diff")
    p.add_argument("db")
    p.add_argument("run_a")
    p.add_argument("run_b")
    p.set_defaults(fn=cmd_diff)
    p = sub.add_parser("calibrate")
    p.add_argument("db")
    p.add_argument("run")
    p.add_argument("known", nargs="+")
    p.set_defaults(fn=cmd_calibrate)
    p = sub.add_parser("html")
    p.add_argument("db")
    p.add_argument("out")
    p.set_defaults(fn=cmd_html)
    a = ap.parse_args()
    a.fn(a)


if __name__ == "__main__":
    main()
