# sed exits with a status above zero and writes a diagnostic to standard error for a script it cannot use and for a file it cannot read, and goes on with the other files.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.015
# MUTANT    sed.status_always_0
# MUTANT    sed.script_error_status0
# MUTANT    sed.file_error_status0
# MUTANT    sed.script_file_status0
# MUTANT    sed.script_error_no_diag
# MUTANT    sed.file_error_no_diag
# MUTANT    sed.script_file_no_diag
# MUTANT    sed.success_status_1
# MUTANT    sed.errors_to_stdout
# MUTANT    sed.file_error_stops
# VARIANT   sed.status2
# VARIANT   sed.status255
# VARIANT   sed.warns
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.015
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'x\n' >in || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
# XCU 1.4 (Consequences of errors, Exit status): an error gives a diagnostic
# on standard error and a status above zero; an unreadable operand does not
# stop the others.
sed 'k' in >o1 2>e1 </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "an unknown command gave status 0"
[ -s e1 ] || pc_fail "no diagnostic for an unknown command"
[ ! -s o1 ] || pc_fail "output was written for an invalid script"
sed 's/a/b' in >o2 2>e2 </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "an unterminated s command gave status 0"
[ -s e2 ] || pc_fail "no diagnostic for an unterminated s command"
sed -n p nosuchfile >o3 2>e3 </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a missing input file gave status 0"
[ -s e3 ] || pc_fail "no diagnostic for a missing input file"
sed p nosuchfile in >o4 2>e4 </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a missing input file before a good one gave status 0"
[ -s e4 ] || pc_fail "no diagnostic for a missing input file before a good one"
[ "$(cat o4 2>/dev/null)" = "x${NL}x" ] || pc_fail "the file after the unreadable one was not edited"
sed p >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "empty standard input gave a nonzero status"
sed -f nosuchfile in >o5 2>e5 </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a missing script file gave status 0"
[ -s e5 ] || pc_fail "no diagnostic for a missing script file"
sed -n p in >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "a successful run did not give status 0"
pc_pass
