# newgrp exits with the status of the shell it created.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.newgrp.001
. "$PC_CORE/pc.sh"
pc_begin XCU.newgrp.001
LC_ALL=C; export LC_ALL
# Which shell newgrp creates is up to the system; name one with SHELL so that every account gets the same.
SHELL=${PC_SH:-/bin/sh}; export SHELL
pc_test
for n in 7 0 3; do
	printf 'echo shell-ran\nexit %s\n' "$n" | newgrp >out 2>/dev/null
	rc=$?
	[ "$rc" -eq "$n" ] && continue
	read -r l <out || l=
	[ "$l" = shell-ran ] || pc_unresolved "newgrp did not create the shell (status $rc)"
	pc_fail "the shell exited with $n but newgrp exited with $rc"
done
pc_pass
