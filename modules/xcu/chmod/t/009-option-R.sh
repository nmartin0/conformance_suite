# chmod -R changes the mode of a directory operand and of every file below it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.009
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.009
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

mkdir t t/sub || pc_fail "cannot create directories"
: >t/f || pc_fail "cannot create t/f"
: >t/sub/g || pc_fail "cannot create t/sub/g"
chmod 755 t t/sub || pc_fail "cannot chmod"
chmod 644 t/f t/sub/g || pc_fail "cannot chmod"
pc_test
chmod -R go-rwx t >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chmod -R exited with status $rc"
for p in t:drwx------ t/sub:drwx------ t/f:-rw------- t/sub/g:-rw-------; do
	n=${p%%:*}
	want=${p##*:}
	modeof "$n" || pc_fail "cannot list $n"
	[ "$mo" = "$want" ] || pc_fail "$n has mode $mo, expected $want"
done
pc_pass
