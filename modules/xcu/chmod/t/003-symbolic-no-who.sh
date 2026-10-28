# Without who, + - and = affect the classes except the bits set in the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.003
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.003
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

for n in f1 f2 f3 f4; do
	: >"$n" || pc_fail "cannot create $n"
done
chmod 666 f1 || pc_fail "cannot chmod f1"
chmod 000 f2 || pc_fail "cannot chmod f2"
chmod 777 f3 || pc_fail "cannot chmod f3"
chmod 644 f4 || pc_fail "cannot chmod f4"
pc_test
umask 022
chmod -w f1 >out 2>err </dev/null
modeof f1 || pc_fail "cannot list f1"
[ "$mo" = "-r--rw-rw-" ] || pc_fail "-w under umask 022: mode is $mo, expected -r--rw-rw-"
chmod +w f2 >out 2>err </dev/null
modeof f2 || pc_fail "cannot list f2"
[ "$mo" = "--w-------" ] || pc_fail "+w under umask 022: mode is $mo, expected --w-------"
chmod =rw f3 >out 2>err </dev/null
modeof f3 || pc_fail "cannot list f3"
[ "$mo" = "-rw-r--r--" ] || pc_fail "=rw under umask 022: mode is $mo, expected -rw-r--r--"
umask 011
chmod +x f4 >out 2>err </dev/null
modeof f4 || pc_fail "cannot list f4"
[ "$mo" = "-rwxr--r--" ] || pc_fail "+x under umask 011: mode is $mo, expected -rwxr--r--"
pc_pass
