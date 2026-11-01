# A separate shell does not inherit aliases.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.009
# MUTANT    alias.inherit_in_child
#
# The new shell is run with -c and as a script file; in both it must not
# know the alias, neither by display nor by use.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.009
printf '%s\n' 'echo ran' 'alias pcal9' >s9.sh
pc_test
alias pcal9='echo x'
sh9=${PC_SH:-sh}
out=$("$sh9" -c 'echo ran; alias pcal9' 2>/dev/null)
rc=$?
# the first line shows that the shell ran at all
[ "$out" = ran ] || pc_fail "a new shell wrote '$out', wanted only 'ran' (exit $rc)"
[ "$rc" -gt 0 ] || pc_fail "a new shell knows the alias (exit $rc)"
out=$("$sh9" s9.sh 2>/dev/null)
rc=$?
[ "$out" = ran ] || pc_fail "a script run by a new shell wrote '$out', wanted only 'ran' (exit $rc)"
[ "$rc" -gt 0 ] || pc_fail "a script run by a new shell knows the alias (exit $rc)"
out=$("$sh9" -c 'echo ran; pcal9' 2>/dev/null)
case $out in
ran) ;;
*) pc_fail "a new shell substituted the alias: '$out'" ;;
esac
pc_pass
