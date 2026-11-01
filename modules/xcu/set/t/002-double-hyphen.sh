# set -- with no operands unsets all positional parameters; after --,
# operands that begin with - or + are plain parameters and set no options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.002
# MUTANT    set.dd_ignored
# MUTANT    set.dd_bare_noop
# MUTANT    set.dd_bare_lists
# MUTANT    set.hash_stale
# MUTANT    set.append
# MUTANT    set.droplast
# MUTANT    set.dropfirst
. "$PC_CORE/pc.sh"
pc_begin XCU.set.002
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
set a b
set --
echo "n=$# ${1+S}"
set -- -x -f +u
echo "$# $1 $2 $3"
case $- in *x*|*f*|*u*) echo opt ;; *) echo noopt ;; esac
pcx=-n
set -- "$pcx"
echo "$# $1"
pcy='-f z'
set -- $pcy
echo "$# $1 $2"
case $- in *f*) echo opt ;; *) echo noopt ;; esac
set -- --
echo "$# $1"
echo done
PCEOF
pc_test
pcrun
want="n=0 ${nl}3 -x -f +u${nl}noopt${nl}1 -n${nl}2 -f z${nl}noopt${nl}1 --${nl}done"
[ "$out" = "$want" ] || pc_fail "got '$out'"
[ "$rc" -eq 0 ] || pc_fail "status $rc"
pc_pass
