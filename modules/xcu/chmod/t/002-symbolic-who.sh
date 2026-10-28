# chmod with a symbolic mode that names who applies +, - and = to the named classes only, several clauses separated by commas.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.002
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.002
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
: >f3 || pc_fail "cannot create f3"
chmod 644 f1 || pc_fail "cannot chmod f1"
chmod 777 f2 || pc_fail "cannot chmod f2"
chmod 644 f3 || pc_fail "cannot chmod f3"
pc_test
chmod u+x,g+w,o-r f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod u+x,g+w,o-r exited with status $rc"
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "-rwxrw----" ] || pc_fail "u+x,g+w,o-r: mode is $mo, expected -rwxrw----"
chmod g=r f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "-rwxr--rwx" ] || pc_fail "g=r: mode is $mo, expected -rwxr--rwx"
chmod u= f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "----r--rwx" ] || pc_fail "u=: mode is $mo, expected ----r--rwx"
chmod a=,u+rw,g+r f3 >out 2>err </dev/null
modeof f3 || pc_fail "cannot list f3"
[ "$mo" = "-rw-r-----" ] || pc_fail "a=,u+rw,g+r: mode is $mo, expected -rw-r-----"
chmod a+x f3 >out 2>err </dev/null
modeof f3 || pc_fail "cannot list f3"
[ "$mo" = "-rwxr-x--x" ] || pc_fail "a+x: mode is $mo, expected -rwxr-x--x"
pc_pass
