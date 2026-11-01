# With set -f pathname expansion is off, with set +f it is on again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.011
# MUTANT    set.f_ignored
# MUTANT    set.plus_f_ignored
# MUTANT    set.f_default_on
# MUTANT    set.f_not_question
# MUTANT    set.f_not_bracket
# MUTANT    set.f_breaks_case
# MUTANT    set.f_breaks_trim
. "$PC_CORE/pc.sh"
pc_begin XCU.set.011
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
LC_ALL=POSIX
export LC_ALL
: >pc.f1
: >pc.f2
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
echo pc.f*
set -f
echo pc.f*
echo pc.f?
echo pc.[f]1
case pc.f1 in pc.f*) echo caseok ;; esac
pcv=pc.f1
echo ${pcv#pc.f*}
set +f
echo pc.f*
PCEOF
pc_test
pcrun
want="pc.f1 pc.f2${nl}pc.f*${nl}pc.f?${nl}pc.[f]1${nl}caseok${nl}1${nl}pc.f1 pc.f2"
[ "$out" = "$want" ] || pc_fail "got '$out'"
pc_pass
