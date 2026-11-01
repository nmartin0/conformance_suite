# Headings of the -o names in the POSIX locale.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.002
# MUTANT    ps.hdr_pid
# MUTANT    ps.hdr_ppid
# MUTANT    ps.hdr_pgid
# MUTANT    ps.hdr_user
# MUTANT    ps.hdr_ruser
# MUTANT    ps.hdr_group
# MUTANT    ps.hdr_rgroup
# MUTANT    ps.hdr_nice
# MUTANT    ps.hdr_vsz
# MUTANT    ps.hdr_etime
# MUTANT    ps.hdr_time
# MUTANT    ps.hdr_tty
# MUTANT    ps.hdr_pcpu
# MUTANT    ps.hdr_comm
# MUTANT    ps.hdr_args
# MUTANT    ps.hdr_lower
# MUTANT    ps.o_blank_first
# MUTANT    ps.o_comma_first
# MUTANT    ps.names_reversed
# VARIANT   ps.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.002
LC_ALL=C; export LC_ALL
pc_test
for pair in pid:PID ppid:PPID pgid:PGID user:USER ruser:RUSER group:GROUP \
    rgroup:RGROUP nice:NI vsz:VSZ etime:ELAPSED time:TIME tty:TT pcpu:%CPU \
    comm:COMMAND args:COMMAND; do
	name=${pair%%:*}
	want=${pair#*:}
	ps -o "$name" -p "$$" >out 2>err </dev/null || pc_fail "ps -o $name -p $$ failed"
	read -r l <out
	set -- $l
	[ "$#" -eq 1 ] && [ "$1" = "$want" ] || pc_fail "heading for $name is '$l', expected $want"
done
for spec in "pid,ppid,comm" "pid ppid comm"; do
	ps -o "$spec" -p "$$" >out 2>err </dev/null || pc_fail "ps -o '$spec' failed"
	read -r l <out
	set -- $l
	[ "$*" = "PID PPID COMMAND" ] || pc_fail "headings for '$spec' are '$l'"
done
pc_pass
