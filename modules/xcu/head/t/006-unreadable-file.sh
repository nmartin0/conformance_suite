# A file operand that cannot be read gives a diagnostic and a nonzero status, and the other operands are still handled.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.006
# MUTANT    head.err_status0
# MUTANT    head.err_stdout
# MUTANT    head.err_stops
# MUTANT    head.err_status_last
# MUTANT    head.err_status_first
# VARIANT   head.err_status2
# VARIANT   head.err_verbose
# VARIANT   head.err_header
. "$PC_CORE/pc.sh"
pc_begin XCU.head.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
# The text does not say whether a header is written for the operand that
# cannot be read; both outputs are accepted, nothing else is.
got=$(head -n 1 nosuchfile f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
rc=${got##*rc=}
out=${got%rc=*}
case $rc in 0 | *[!0-9]*) pc_fail "status is not nonzero when an operand could not be read" ;; esac
case $out in
"==> f1 <==${NL}1${NL}" | "==> nosuchfile <==${NL}${NL}==> f1 <==${NL}1${NL}") ;;
*) pc_fail "the readable file after the unreadable one was not handled as required" ;;
esac
got=$(head -n 1 f1 nosuchfile 2>/dev/null </dev/null; printf 'rc=%s' "$?")
rc=${got##*rc=}
out=${got%rc=*}
case $rc in 0 | *[!0-9]*) pc_fail "status is not nonzero when the last operand could not be read" ;; esac
case $out in
"==> f1 <==${NL}1${NL}" | "==> f1 <==${NL}1${NL}${NL}==> nosuchfile <==${NL}") ;;
*) pc_fail "the readable file before the unreadable one was not handled as required" ;;
esac
got=$(head nosuchfile 2>/dev/null </dev/null; printf 'rc=%s' "$?")
rc=${got##*rc=}
case $rc in 0 | *[!0-9]*) pc_fail "status is not nonzero for a single unreadable operand" ;; esac
[ "${got%rc=*}" = "" ] || pc_fail "output was written for a single unreadable operand"
pc_pass
