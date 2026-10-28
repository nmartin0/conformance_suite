#!/usr/bin/env python3
# nullsys.py - generate the stub library of the negative control ("null
# system") from the spec model.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: gen/nullsys.py [--model build/spec/model.json] --out nullsys.c
#
# For every function the model lists (names and prototypes only: facts, no
# text of the standard) it writes a C definition that, while the test phase
# is on, records that it was called, sets errno and returns the failure
# value of its return kind, and otherwise passes the call on to the next
# definition of the name (the C library's). gen/nullsys_rt.c is the hand
# written runtime it relies on. See docs/negative-control.md.
#
# The generated file includes no system header, so it cannot disagree with
# the system's declarations. Parameters and results are therefore given by
# class, not by their exact types: pointers, integers of any width (as a
# long), float, double, long double and their complex forms, and the few
# structures returned by value. Passing an integer as a long to a function
# that takes an int, and the other way round, works on the usual calling
# conventions with 64-bit longs (the System V and AAPCS64 families); it does
# not on a convention that passes 64-bit integers in pairs (32-bit ABIs).
#
# Like the model, the output is derived from the standard and is kept under
# build/ (or wherever --out says), never committed.

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Not defined at all: a wrapper cannot stand in for them.
#   setjmp family, vfork: return twice / share the caller's frame
#   context functions: save the frame of the wrapper
#   dlsym: the runtime needs the real one to find all the others
NOT_STUBBED = {
    "setjmp": "returns twice", "sigsetjmp": "returns twice",
    "_setjmp": "returns twice", "vfork": "shares the caller's frame",
    "getcontext": "saves the wrapper's frame", "setcontext": "restores a frame",
    "swapcontext": "saves the wrapper's frame", "makecontext": "builds on a frame",
    "dlsym": "the runtime looks the real functions up with it",
}
# Written by hand in gen/nullsys_rt.c (they need care or extra behaviour).
IN_RUNTIME = {
    "getenv": "carries the test-phase switch",
    "malloc": "used by the dynamic loader while looking functions up",
    "calloc": "same", "realloc": "same", "free": "same",
    "confstr": "_CS_PATH names the stub utilities first outside the test phase",
}

# vprintf-style forwarding of variadic functions: name -> the function that
# takes a va_list.
VFORWARD = {
    "printf": "vprintf", "fprintf": "vfprintf", "dprintf": "vdprintf",
    "sprintf": "vsprintf", "snprintf": "vsnprintf", "scanf": "vscanf",
    "fscanf": "vfscanf", "sscanf": "vsscanf", "wprintf": "vwprintf",
    "fwprintf": "vfwprintf", "swprintf": "vswprintf", "wscanf": "vwscanf",
    "fwscanf": "vfwscanf", "swscanf": "vswscanf", "syslog": "vsyslog",
    "asprintf": "vasprintf",
}
# Variadic functions whose extra arguments are a known number of integers
# or pointers (read as long words and passed on).
LONGS = {"open": 1, "openat": 1, "fcntl": 1, "semctl": 1, "mq_open": 2,
         "sem_open": 2}
# Extra arguments: one double (a typical strfmon call).
DOUBLES = {"strfmon": 1, "strfmon_l": 1}
# execl family: argument list gathered into a vector for the v form.
EXECL = {"execl": ("execv", False), "execlp": ("execvp", False),
         "execle": ("execve", True)}

W64 = {"long", "long long", "size_t", "ssize_t", "off_t", "time_t", "intmax_t",
       "uintmax_t", "uint64_t", "int64_t", "clock_t", "ptrdiff_t", "unsigned long",
       "unsigned long long", "suseconds_t", "blkcnt_t", "fsblkcnt_t", "fsfilcnt_t"}
W16 = {"uint16_t", "int16_t", "short", "unsigned short", "in_port_t"}

STRUCT_RET = {
    "div_t": ("struct ns_div", "ns_zero_div()"),
    "ldiv_t": ("struct ns_ldiv", "ns_zero_ldiv()"),
    "lldiv_t": ("struct ns_lldiv", "ns_zero_lldiv()"),
    "imaxdiv_t": ("struct ns_lldiv", "ns_zero_lldiv()"),
    "datum": ("struct ns_datum", "ns_zero_datum()"),
}


class Fn:
    pass


def split_params(s):
    """Top-level comma separated pieces of the parameter text."""
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def param_class(p):
    if "*" in p or "[" in p or "(" in p:
        return "void *"
    if p.startswith("datum"):
        return "struct ns_datum"
    cx = "complex" in p
    if re.search(r"\blong\s+double\b", p):
        return "long double _Complex" if cx else "long double"
    if re.search(r"\bdouble\b", p):
        return "double _Complex" if cx else "double"
    if re.search(r"\bfloat\b", p):
        return "float _Complex" if cx else "float"
    return "long"


def width_of(rt):
    t = re.sub(r"\b(const|volatile|signed)\b", "", rt)
    t = re.sub(r"\s+", " ", t).strip()
    if t in W64:
        return 64
    if t in W16:
        return 16
    return 32


def analyse(entry):
    """Class of the prototype in a model entry, or None."""
    decl, name = entry.get("decl"), entry["name"]
    if not decl:
        return None
    m = re.search(r"\b" + re.escape(name) + r"\s*\(", decl)
    if not m:
        return None
    f = Fn()
    f.name = name
    rt = decl[:m.start()]
    f.noreturn = "_Noreturn" in rt
    rt = rt.replace("_Noreturn", "").strip()
    rest = decl[m.end():]
    depth, end = 1, len(rest)
    for i, ch in enumerate(rest):
        depth += ch == "("
        depth -= ch == ")"
        if depth == 0:
            end = i
            break
    plist = split_params(rest[:end])
    f.variadic = bool(plist) and plist[-1] == "..."
    if f.variadic:
        plist = plist[:-1]
    if plist == ["void"]:
        plist = []
    f.params = [param_class(p) for p in plist]
    cx = "complex" in rt
    if "*" in rt or "(" in rt:
        f.rkind, f.rtype, f.fail = "ptr", "void *", "0"
    elif rt == "void":
        f.rkind, f.rtype, f.fail = "void", "void", None
    elif rt in STRUCT_RET:
        f.rkind = "struct"
        f.rtype, f.fail = STRUCT_RET[rt]
    elif re.search(r"\blong\s+double\b", rt):
        s = "c" if cx else ""
        f.rkind, f.rtype = "float", "long double" + (" _Complex" if cx else "")
        f.fail = "ns_nan_ld%s()" % s
    elif re.search(r"\bdouble\b", rt):
        s = "c" if cx else ""
        f.rkind, f.rtype = "float", "double" + (" _Complex" if cx else "")
        f.fail = "ns_nan_d%s()" % s
    elif re.search(r"\bfloat\b", rt):
        s = "c" if cx else ""
        f.rkind, f.rtype = "float", "float" + (" _Complex" if cx else "")
        f.fail = "ns_nan_f%s()" % s
    else:
        w = width_of(rt)
        f.rkind, f.rtype, f.fail = "int", "long", "NS_FAIL%d" % w
    return f


def plist_types(f):
    return ", ".join(f.params) if f.params else "void"


def emit(f, idx, out):
    P = f.params
    names = ["a%d" % i for i in range(len(P))]
    formal = ", ".join("%s %s" % (t, n) for t, n in zip(P, names))
    ptype = ", ".join(P)
    if f.variadic:
        formal = (formal + ", ..." if formal else "...")
    R = f.rtype
    fnname = f.name

    def fail_body(ind):
        s = ind + "ns_seterr();\n"
        if f.noreturn:
            s += ind + "ns_die();\n"
        elif f.rkind == "void":
            s += ind + "return;\n"
        else:
            s += ind + "return %s;\n" % f.fail
        return s

    w = out.write
    w("%s %s(%s)\n{\n" % (R, fnname, formal if formal else "void"))

    def gate_on():
        w("\tif (ns_enter(%d, \"%s\")) {\n" % (idx, fnname))
        w(fail_body("\t\t"))
        w("\t}\n")

    def no_real():
        w("\tif (real == 0) {\n")
        w(fail_body("\t\t"))
        w("\t}\n")

    def finish(call, has_r):
        if f.noreturn:
            w("\t%s;\n\tns_die();\n" % call)
        elif f.rkind == "void":
            w("\t%s;\n" % call)
        else:
            w("\treturn %s;\n" % call)

    last = names[-1] if names else None
    if not f.variadic:
        w("\tstatic %s (*real)(%s);\n\n" % (R, plist_types(f)))
        gate_on()
        w("\tif (real == 0)\n\t\treal = (%s (*)(%s))ns_next(\"%s\");\n" %
          (R, plist_types(f), fnname))
        no_real()
        finish("real(%s)" % ", ".join(names), False)
    elif fnname in VFORWARD:
        w("\tstatic %s (*real)(%s);\n\tva_list ap;\n" %
          (R, ", ".join(P + ["va_list"])))
        if f.rkind != "void" and not f.noreturn:
            w("\t%s r;\n" % R)
        w("\n")
        gate_on()
        w("\tif (real == 0)\n\t\treal = (%s (*)(%s))ns_next(\"%s\");\n" %
          (R, ", ".join(P + ["va_list"]), VFORWARD[fnname]))
        no_real()
        w("\tva_start(ap, %s);\n" % last)
        if f.rkind == "void":
            w("\treal(%s);\n\tva_end(ap);\n" % ", ".join(names + ["ap"]))
        else:
            w("\tr = real(%s);\n\tva_end(ap);\n\treturn r;\n" %
              ", ".join(names + ["ap"]))
    elif fnname in LONGS or fnname in DOUBLES:
        isd = fnname in DOUBLES
        n = (DOUBLES if isd else LONGS)[fnname]
        vt = "double" if isd else "long"
        w("\tstatic %s (*real)(%s, ...);\n\tva_list ap;\n" % (R, ptype))
        for i in range(n):
            w("\t%s x%d;\n" % (vt, i))
        w("\n")
        gate_on()
        w("\tif (real == 0)\n\t\treal = (%s (*)(%s, ...))ns_next(\"%s\");\n" %
          (R, ptype, fnname))
        no_real()
        w("\tva_start(ap, %s);\n" % last)
        for i in range(n):
            w("\tx%d = va_arg(ap, %s);\n" % (i, vt))
        w("\tva_end(ap);\n")
        finish("real(%s)" % ", ".join(names + ["x%d" % i for i in range(n)]), False)
    elif fnname in EXECL:
        vname, has_env = EXECL[fnname]
        vparams = ["void *", "void *"] + (["void *"] if has_env else [])
        w("\tstatic %s (*real)(%s);\n\tva_list ap;\n\tvoid *argv[NS_MAXARGS];\n"
          "%s\tint n = 0;\n\n" % (R, ", ".join(vparams), "\tvoid *envp = 0;\n" if has_env else ""))
        gate_on()
        w("\tif (real == 0)\n\t\treal = (%s (*)(%s))ns_next(\"%s\");\n" %
          (R, ", ".join(vparams), vname))
        no_real()
        w("\targv[n++] = %s;\n\tva_start(ap, %s);\n" % (last, last))
        w("\twhile (n < NS_MAXARGS - 1) {\n\t\tvoid *p = va_arg(ap, void *);\n"
          "\t\targv[n++] = p;\n\t\tif (p == 0)\n\t\t\tbreak;\n\t}\n")
        if has_env:
            w("\tif (argv[n - 1] == 0)\n\t\tenvp = va_arg(ap, void *);\n")
        w("\tva_end(ap);\n\targv[n] = 0;\n")
        finish("real(%s)" % ", ".join([names[0], "argv"] + (["envp"] if has_env else [])), False)
    else:
        sys.stderr.write("nullsys.py: warning: variadic %s has no forwarder; "
                         "passing three long words\n" % fnname)
        w("\tstatic %s (*real)(%s, ...);\n\tva_list ap;\n\tlong x0, x1, x2;\n\n" %
          (R, ptype))
        gate_on()
        w("\tif (real == 0)\n\t\treal = (%s (*)(%s, ...))ns_next(\"%s\");\n" %
          (R, ptype, fnname))
        no_real()
        w("\tva_start(ap, %s);\n\tx0 = va_arg(ap, long);\n\tx1 = va_arg(ap, long);\n"
          "\tx2 = va_arg(ap, long);\n\tva_end(ap);\n" % last)
        finish("real(%s)" % ", ".join(names + ["x0", "x1", "x2"]), False)
    w("}\n\n")


PRELUDE = r'''/*
 * nullsys.c - the stubs of the negative control ("null system").
 *
 * GENERATED by gen/nullsys.py from the spec model; do not edit and do not
 * commit (RULES.md, rule 3). Names and parameter classes only.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * While the test phase is on (ns_enter() says so) a stub sets errno and
 * returns the failure value of its kind: a null pointer, an integer of all
 * ones (or, built with -DNS_POISON, a value no system returns), a NaN, a
 * zeroed structure, or nothing. Otherwise it passes the call on to the next
 * definition of its name. Built without any system header on purpose.
 */
#include <stdarg.h>

#define NS_MAXARGS 256

#ifdef NS_POISON
# define NS_FAIL16 0x5EEDL
# define NS_FAIL32 0x5EED5EEDL
# define NS_FAIL64 ((long)0x5EED5EED5EED5EEDLL)
#else
# define NS_FAIL16 (-1L)
# define NS_FAIL32 (-1L)
# define NS_FAIL64 (-1L)
#endif

/* gen/nullsys_rt.c */
extern int ns_enter(int idx, const char *name);
extern void *ns_next(const char *name);
extern void ns_seterr(void);
extern void ns_die(void);
extern double ns_nan_d(void);
extern float ns_nan_f(void);
extern long double ns_nan_ld(void);
extern double _Complex ns_nan_dc(void);
extern float _Complex ns_nan_fc(void);
extern long double _Complex ns_nan_ldc(void);

struct ns_div { int quot, rem; };
struct ns_ldiv { long quot, rem; };
struct ns_lldiv { long long quot, rem; };
struct ns_datum { void *dptr; unsigned long dsize; };

static struct ns_div ns_zero_div(void)
{
	struct ns_div r;
	r.quot = 0;
	r.rem = 0;
	return r;
}

static struct ns_ldiv ns_zero_ldiv(void)
{
	struct ns_ldiv r;
	r.quot = 0;
	r.rem = 0;
	return r;
}

static struct ns_lldiv ns_zero_lldiv(void)
{
	struct ns_lldiv r;
	r.quot = 0;
	r.rem = 0;
	return r;
}

static struct ns_datum ns_zero_datum(void)
{
	struct ns_datum r;
	r.dptr = 0;
	r.dsize = 0;
	return r;
}

'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--model", default=str(ROOT / "build" / "spec" / "model.json"))
    ap.add_argument("--out", required=True)
    args = ap.parse_args()
    try:
        model = json.load(open(args.model))
    except OSError as e:
        sys.exit("nullsys.py: cannot read the spec model (%s); build it with "
                 "gen/specmodel.py first" % e)
    seen, fns, skipped = set(), [], []
    for hdr in model:
        for e in hdr["functions"]:
            n = e["name"]
            if n in seen:
                continue
            seen.add(n)
            if n in NOT_STUBBED:
                skipped.append((n, NOT_STUBBED[n]))
                continue
            if n in IN_RUNTIME:
                skipped.append((n, "in the runtime: " + IN_RUNTIME[n]))
                continue
            f = analyse(e)
            if f is None:
                skipped.append((n, "no prototype in the model (a macro)"))
                continue
            fns.append(f)
    fns.sort(key=lambda f: f.name)
    with open(args.out, "w") as out:
        out.write(PRELUDE)
        for i, f in enumerate(fns):
            emit(f, i, out)
        out.write("/* %d stubs */\nint ns_stub_count = %d;\n" % (len(fns), len(fns)))
    sys.stderr.write("nullsys.py: %d stubs written to %s\n" % (len(fns), args.out))
    for n, why in sorted(skipped):
        sys.stderr.write("nullsys.py: not generated: %s (%s)\n" % (n, why))
    if len(fns) >= 3800:
        sys.exit("nullsys.py: too many functions for the runtime's table")


if __name__ == "__main__":
    main()
