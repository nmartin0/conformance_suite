# On a regular file with an execute bit, u+s sets and u-s clears set-user-ID, octal 4755 sets it and 755 clears it, and o+s changes nothing and is not an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.007
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.007
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

for n in f1 f2 f3; do
	: >"$n" || pc_fail "cannot create $n"
done
chmod 755 f1 || pc_fail "cannot chmod f1"
chmod 755 f2 || pc_fail "cannot chmod f2"
chmod 4755 f3 || pc_fail "cannot chmod f3"
pc_test
chmod u+s f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod u+s exited with status $rc"
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "-rwsr-xr-x" ] || pc_fail "u+s: mode is $mo, expected -rwsr-xr-x"
chmod u-s f1 >out 2>err </dev/null
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "-rwxr-xr-x" ] || pc_fail "u-s: mode is $mo, expected -rwxr-xr-x"
chmod 4755 f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "-rwsr-xr-x" ] || pc_fail "4755: mode is $mo, expected -rwsr-xr-x"
chmod 755 f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "-rwxr-xr-x" ] || pc_fail "755 after 4755: mode is $mo, expected -rwxr-xr-x"
chmod o+s f3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod o+s exited with status $rc"
modeof f3 || pc_fail "cannot list f3"
[ "$mo" = "-rwsr-xr-x" ] || pc_fail "o+s changed the mode to $mo"
pc_pass
