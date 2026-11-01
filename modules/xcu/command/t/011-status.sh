# command exits 127, 126 or the status of the command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.011
# MUTANT    command.st_notfound_1
# MUTANT    command.st_notfound_126
# MUTANT    command.st_noexec_127
# MUTANT    command.st_noexec_1
# MUTANT    command.st_run_clamp1
# MUTANT    command.st_zero_to_1
# MUTANT    command.st_run_zero
#
# Statuses 0 and 7 pass through; a missing utility gives 127; a file that
# is found but cannot be run (no execute permission; a directory) gives 126.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.011
mkdir bin dir || pc_fail "cannot make a directory"
printf 'exit 7\n' >bin/pccmd || pc_fail "cannot write a script"
printf 'exit 0\n' >bin/pcok || pc_fail "cannot write a script"
chmod +x bin/pccmd bin/pcok || pc_fail "cannot make the scripts executable"
printf 'exit 0\n' >bin/pcnoexec || pc_fail "cannot write a file"
chmod 644 bin/pcnoexec || pc_fail "cannot set the mode"
PATH=$PWD/bin:$PATH
pc_test
command pccmd 2>/dev/null
rc=$?
[ "$rc" -eq 7 ] || pc_fail "command pccmd exited $rc, wanted 7"
command pcok 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command pcok exited $rc, wanted 0"
command pcnosuch7 2>/dev/null
rc=$?
[ "$rc" -eq 127 ] || pc_fail "command of a missing utility exited $rc, wanted 127"
command ./bin/pcnoexec 2>/dev/null
rc=$?
[ "$rc" -eq 126 ] || pc_fail "command of a file that cannot be run exited $rc, wanted 126"
command ./dir 2>/dev/null
rc=$?
[ "$rc" -eq 126 ] || pc_fail "command of a directory exited $rc, wanted 126"
pc_pass
