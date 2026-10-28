# commands run by awk
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.030
# Commands run by system, by output pipes and by command input use the PATH
# and the environment that awk started with.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.030
LC_ALL=POSIX
export LC_ALL
NL='
'
shpath=$(command -v sh) || pc_unresolved "setup: no sh found"
case $shpath in /*) ;; *) pc_unresolved "setup: no absolute path for sh" ;; esac
mkdir bin || pc_unresolved "setup: cannot make a directory"
printf '#!%s\necho ran-pc-cmd\n' "$shpath" >bin/pcawkcmd || pc_unresolved "setup: cannot write a command"
printf '#!%s\ncat\necho done-pc\n' "$shpath" >bin/pcawkcat || pc_unresolved "setup: cannot write a command"
chmod +x bin/pcawkcmd bin/pcawkcat || pc_unresolved "setup: cannot make the commands executable"
bindir=$(pwd)/bin
pc_test
got=$( PATH=$bindir:$PATH; export PATH; awk 'BEGIN { system("pcawkcmd") }' 2>/dev/null </dev/null )
[ "$got" = "ran-pc-cmd" ] || pc_fail "system() did not find the command through PATH: $got"
got=$( PATH=$bindir:$PATH; export PATH; awk 'BEGIN { "pcawkcmd" | getline x; print x }' 2>/dev/null </dev/null )
[ "$got" = "ran-pc-cmd" ] || pc_fail "command | getline did not find the command through PATH: $got"
got=$( PATH=$bindir:$PATH; export PATH; awk 'BEGIN { print "z" | "pcawkcat" }' 2>/dev/null </dev/null )
[ "$got" = "z${NL}done-pc" ] || pc_fail "print | command did not find the command through PATH: $got"
got=$( PCAWKV=orig; export PCAWKV; awk 'BEGIN { system("echo $PCAWKV") }' 2>/dev/null </dev/null )
[ "$got" = "orig" ] || pc_fail "system() did not see the environment variable: $got"
got=$( PCAWKV=orig; export PCAWKV; awk 'BEGIN { "echo $PCAWKV" | getline x; print x }' 2>/dev/null </dev/null )
[ "$got" = "orig" ] || pc_fail "command | getline did not see the environment variable: $got"
pc_pass
