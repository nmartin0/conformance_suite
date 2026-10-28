# A clause ended by ;& also runs the list of the next clause without testing
# its pattern, and so on until a clause ended by ;; (or the last one).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.009
nl='
'
# pcscript: write the arguments, one per line, to pc.s
pcscript() {
	printf '%s\n' "$@" >pc.s || pc_unresolved "cannot write pc.s"
}
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
pc_test
pcscript 'r=' 'case b in a) r=${r}A ;& b) r=${r}B ;& c) r=${r}C ;; d) r=${r}D ;; esac' 'echo "$r"'
pcrun
[ "$out" = BC ] || pc_fail "fall through from b: got '$out', expected BC"
pcscript 'r=' 'case a in a) r=${r}1 ;& x) r=${r}2 ;& y) r=${r}3 ;; esac' 'echo "$r"'
pcrun
[ "$out" = 123 ] || pc_fail "fall through from a: got '$out', expected 123"
pcscript 'r=' 'case z in a) r=${r}A ;& *) r=${r}S ;; esac' 'echo "$r"'
pcrun
[ "$out" = S ] || pc_fail "a clause before the match must not run: got '$out', expected S"
pc_pass
