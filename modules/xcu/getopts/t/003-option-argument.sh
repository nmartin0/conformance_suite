# The option-argument is placed in OPTARG, separate or attached.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.003
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.003
pc_test
set -- -c val x
getopts c: pcopt
rc=$?
[ "$rc" -eq 0 ] && [ "$pcopt" = c ] || pc_fail "separate argument: exit $rc, name '$pcopt'"
[ "${OPTARG-unset}" = val ] || pc_fail "OPTARG is '${OPTARG-unset}', wanted val"
[ "$OPTIND" = 3 ] || pc_fail "OPTIND is '$OPTIND' after -c val, wanted 3"
OPTIND=1
set -- -cval x
getopts c: pcopt
rc=$?
[ "$rc" -eq 0 ] && [ "$pcopt" = c ] || pc_fail "attached argument: exit $rc, name '$pcopt'"
[ "${OPTARG-unset}" = val ] || pc_fail "OPTARG is '${OPTARG-unset}' for -cval, wanted val"
[ "$OPTIND" = 2 ] || pc_fail "OPTIND is '$OPTIND' after -cval, wanted 2"
pc_pass
