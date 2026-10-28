# command -v writes just the name for functions, special built-ins and reserved words.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.005
. "$PC_CORE/pc.sh"
pc_begin XCU.command.005
pcfn() { :; }
pc_test
out=$(command -v pcfn 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v for a function exited $rc"
[ "$out" = pcfn ] || pc_fail "for a function command -v wrote '$out', wanted pcfn"
out=$(command -v exec 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v for a special built-in exited $rc"
[ "$out" = exec ] || pc_fail "for a special built-in command -v wrote '$out', wanted exec"
out=$(command -v if 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v for a reserved word exited $rc"
[ "$out" = if ] || pc_fail "for a reserved word command -v wrote '$out', wanted if"
pc_pass
