# sh command_file argument... reads the commands from the file, which need
# not be executable, sets $0 to the file name as given and $1, $2 ... to the
# arguments; standard input is not read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.005
printf '%s\n' 'echo "$0 $1 $2"' >pc.cf || pc_unresolved "write"
chmod 644 pc.cf || pc_unresolved "chmod"
mkdir pc.dir || pc_unresolved "mkdir"
cp pc.cf pc.dir/pc.cf2 || pc_unresolved "cp"
pc_test
r=$(${PC_SH:-sh} pc.cf a b </dev/null)
[ "$r" = "pc.cf a b" ] || pc_fail "name without slash: got '$r'"
r=$(${PC_SH:-sh} ./pc.cf a b </dev/null)
[ "$r" = "./pc.cf a b" ] || pc_fail "name with slash: got '$r'"
r=$(${PC_SH:-sh} pc.dir/pc.cf2 x y </dev/null)
[ "$r" = "pc.dir/pc.cf2 x y" ] || pc_fail "name in a directory: got '$r'"
r=$(printf 'echo from-stdin\n' | ${PC_SH:-sh} pc.cf a b)
[ "$r" = "pc.cf a b" ] || pc_fail "standard input was read when a file was given: '$r'"
pc_pass
