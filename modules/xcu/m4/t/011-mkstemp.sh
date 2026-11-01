# mkstemp
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.011
# mkstemp gives the pathname of a file it created from the template
# argument.
# MUTANT    m4.mkstemp_nocreate
# MUTANT    m4.mkstemp_template
# MUTANT    m4.mkstemp_fixed
# MUTANT    m4.mkstemp_dir_dropped
# MUTANT    m4.mkstemp_prefix_dropped
# MUTANT    m4.mkstemp_short
# MUTANT    m4.mkstemp_nl
# VARIANT   m4.engine
# VARIANT   m4.mkstemp_digits
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub || pc_unresolved "cannot make a directory"
cat >in5 <<'EOM'
mkstemp(`pcmXXXXXX')|mkstemp(`pcmXXXXXX')|mkstemp(`sub/pcnXXXXXX')|
EOM
pc_test
m4 in5 >out 2>err </dev/null || pc_fail "mkstemp: nonzero status"
got=$(cat out)
case $got in
*'|'*'|'*'|') ;;
*) pc_fail "mkstemp output was '$got'" ;;
esac
n1=${got%%'|'*}
rest=${got#*'|'}
n2=${rest%%'|'*}
rest=${rest#*'|'}
n3=${rest%%'|'*}
case $n1 in pcm??????) ;; *) pc_fail "first mkstemp gave '$n1'" ;; esac
case $n2 in pcm??????) ;; *) pc_fail "second mkstemp gave '$n2'" ;; esac
case $n3 in sub/pcn??????) ;; *) pc_fail "mkstemp with a directory gave '$n3'" ;; esac
[ "$n1" != pcmXXXXXX ] && [ "$n2" != pcmXXXXXX ] && [ "$n3" != sub/pcnXXXXXX ] ||
	pc_fail "mkstemp returned the template unchanged"
[ "$n1" != "$n2" ] || pc_fail "two calls gave the same name '$n1'"
[ -f "$n1" ] || pc_fail "mkstemp did not create '$n1'"
[ -f "$n2" ] || pc_fail "mkstemp did not create '$n2'"
[ -f "$n3" ] || pc_fail "mkstemp did not create '$n3'"
rm -f "$n1" "$n2" "$n3"
pc_pass
