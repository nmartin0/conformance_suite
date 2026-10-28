# A file that is not in an executable format (a script with no #! line) is
# run by a shell, with its arguments, and its exit status is returned; for a
# name with a slash $0 is that name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.017
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.017
printf '%s\n' ': >"pc.mark.$1.$2"' 'printf "%s\n" "$0" >pc.zero' 'exit 4' >pc.sc || pc_unresolved "write"
chmod 755 pc.sc || pc_unresolved "chmod"
pc_test
./pc.sc a b
rc=$?
[ "$rc" -eq 4 ] || pc_fail "status $rc, expected 4"
[ -e pc.mark.a.b ] || pc_fail "the script did not run with its arguments"
[ "$(cat pc.zero)" = ./pc.sc ] || pc_fail "\$0 was '$(cat pc.zero)', expected ./pc.sc"
pc_pass
