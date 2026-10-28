# -D, -U, -I as for c17, in order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.007
# -D defines and -U removes a macro, the later of the two for a name wins,
# and -I names a directory searched for quoted include files.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >pp.c <<'EOS'
#ifdef FEAT
int featvar;
#endif
int plainvar;
EOS
mkdir inc || pc_unresolved "setup: cannot make a directory"
cat >inc/h.h <<'EOS'
#define USEIT 1
EOS
cat >ph.c <<'EOS'
#include "h.h"
#if USEIT
int viahdr;
#endif
int always;
EOS
has() { grep -w "$2" "$1" >/dev/null; }
pc_test
cxref pp.c </dev/null >o0 2>err || pc_fail "cxref: nonzero status"
has o0 plainvar || pc_fail "plainvar missing: $(cat o0)"
has o0 featvar && pc_fail "featvar listed without -D"
cxref -D FEAT pp.c </dev/null >o1 2>err || pc_fail "cxref -D: nonzero status"
has o1 featvar || pc_fail "-D FEAT did not bring in featvar: $(cat o1)"
cxref -D FEAT -U FEAT pp.c </dev/null >o2 2>err || pc_fail "cxref -D -U: nonzero status"
has o2 featvar && pc_fail "-U after -D did not remove FEAT"
cxref -U FEAT -D FEAT pp.c </dev/null >o3 2>err || pc_fail "cxref -U -D: nonzero status"
has o3 featvar || pc_fail "-D after -U did not define FEAT"
cxref -I inc ph.c </dev/null >o4 2>err || pc_fail "cxref -I: nonzero status"
has o4 viahdr || pc_fail "-I inc did not find h.h: $(cat o4)"
has o4 always || pc_fail "always missing with -I"
pc_pass
