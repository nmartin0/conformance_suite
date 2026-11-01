# Options grouped in one word are returned in order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.004
# MUTANT    getopts.cluster_skip_rest
# MUTANT    getopts.cluster_reverse
# MUTANT    getopts.cluster_whole
# MUTANT    getopts.cluster_attached_arg_ignored
# MUTANT    getopts.end_optind_minus1
# MUTANT    getopts.end_optind_plus1
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.004
pc_test
set -- -ab x
getopts ab pcopt
rc1=$?
v1=$pcopt
getopts ab pcopt
rc2=$?
v2=$pcopt
getopts ab pcopt
rc3=$?
ind=$OPTIND
[ "$rc1" -eq 0 ] && [ "$v1" = a ] || pc_fail "first of -ab: exit $rc1, name '$v1'"
[ "$rc2" -eq 0 ] && [ "$v2" = b ] || pc_fail "second of -ab: exit $rc2, name '$v2'"
[ "$rc3" -eq 1 ] || pc_fail "third call exited $rc3, wanted 1"
[ "$ind" = 2 ] || pc_fail "OPTIND is '$ind' at the end, wanted 2"
# an option with an argument ends a word: the argument is separate or attached
OPTIND=1
set -- -ac val x
getopts ac: pcopt
rc1=$?
v1=$pcopt
getopts ac: pcopt
rc2=$?
v2=$pcopt
[ "$rc1" -eq 0 ] && [ "$v1" = a ] || pc_fail "-ac val, first: exit $rc1, name '$v1'"
[ "$rc2" -eq 0 ] && [ "$v2" = c ] || pc_fail "-ac val, second: exit $rc2, name '$v2'"
[ "${OPTARG-unset}" = val ] || pc_fail "OPTARG is '${OPTARG-unset}' for -ac val, wanted val"
[ "$OPTIND" = 3 ] || pc_fail "OPTIND is '$OPTIND' after -ac val, wanted 3"
OPTIND=1
set -- -acval x
getopts ac: pcopt
rc1=$?
v1=$pcopt
getopts ac: pcopt
rc2=$?
v2=$pcopt
[ "$rc1" -eq 0 ] && [ "$v1" = a ] || pc_fail "-acval, first: exit $rc1, name '$v1'"
[ "$rc2" -eq 0 ] && [ "$v2" = c ] || pc_fail "-acval, second: exit $rc2, name '$v2'"
[ "${OPTARG-unset}" = val ] || pc_fail "OPTARG is '${OPTARG-unset}' for -acval, wanted val"
[ "$OPTIND" = 2 ] || pc_fail "OPTIND is '$OPTIND' after -acval, wanted 2"
pc_pass
