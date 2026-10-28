# A utility given an invalid RE generates an error condition.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.9.1-definitions.005
#
# grep exits above 1 on an error; sed exits with a nonzero value.
. "$PC_CORE/pc.sh"
pc_begin XBD.9.1-definitions.005

# Valid forms of the same patterns must work, or nothing is learned.
printf 'aa\n' | grep -e '\(a\)\1' >/dev/null 2>&1 || pc_unresolved "grep does not match a valid back-reference"
printf 'a\n' | grep -E -e '[[.a.]]' >/dev/null 2>&1 || pc_unresolved "grep does not accept a valid collating symbol"
r=$(printf 'aa\n' | sed -n '/\(a\)\1/p' 2>/dev/null) || pc_unresolved "sed rejects a valid back-reference"
[ "$r" = aa ] || pc_unresolved "sed does not match a valid back-reference"

pc_test
printf 'aa\n' | grep -e '\(a\)\2' >/dev/null 2>&1
rc=$?
[ "$rc" -gt 1 ] || pc_fail "grep with a missing group in a back-reference exited $rc"
printf 'aa\n' | grep -e '[[.xyzzy.]]' >/dev/null 2>&1
rc=$?
[ "$rc" -gt 1 ] || pc_fail "grep with an unknown collating symbol exited $rc"
printf 'aa\n' | grep -E -e '[[.xyzzy.]]' >/dev/null 2>&1
rc=$?
[ "$rc" -gt 1 ] || pc_fail "grep -E with an unknown collating symbol exited $rc"
printf 'aa\n' | sed -n '/\(a\)\2/p' >/dev/null 2>&1
rc=$?
[ "$rc" -ne 0 ] || pc_fail "sed with a missing group in a back-reference exited 0"
printf 'aa\n' | sed 's/[[.xyzzy.]]/x/' >/dev/null 2>&1
rc=$?
[ "$rc" -ne 0 ] || pc_fail "sed with an unknown collating symbol exited 0"
pc_pass
