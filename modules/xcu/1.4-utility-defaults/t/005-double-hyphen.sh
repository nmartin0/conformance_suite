# Standard utilities that accept no options but accept operands treat a first argument of two hyphens as the end of options and discard it, so operands may start with a hyphen
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.005
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'data\n' >./-pcf || pc_unresolved "cannot create a file whose name starts with a hyphen"
cksum ./-pcf >want.ck || pc_unresolved "cksum cannot read the file"
sed 's|\./||' want.ck >want.ck2 || pc_unresolved "sed"
pc_test
chk() {
	label=$1; want=$2; shift 2
	got=$("$@" 2>err)
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$label: exit status $rc ($(cat err))"
	[ "$got" = "$want" ] || pc_fail "$label: got '$got', wanted '$want'"
}
chk "printf --" x printf -- '%s\n' x
chk "printf -- with a leading hyphen in the format" "-a" printf -- '-%s\n' a
chk "basename --" b basename -- /a/b
chk "basename -- with a hyphen operand" -x basename -- -x
chk "dirname --" /a dirname -- /a/b
chk "dirname -- with a hyphen operand" -x dirname -- -x/y
chk "expr --" 3 expr -- 1 + 2
chk "sleep --" "" sleep -- 0
got=$(cksum -- -pcf 2>err)
[ "$got" = "$(cat want.ck2)" ] || pc_fail "cksum -- with a hyphen operand: got '$got'"
pc_pass
