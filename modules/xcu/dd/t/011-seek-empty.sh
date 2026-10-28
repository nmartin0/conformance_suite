# dd seek= with an empty input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.011
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Empty input: the output size becomes the seek offset when it is beyond
# the old end or the file is new; a smaller offset shortens the file.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
rep() { r=0; while [ "$r" -lt "$1" ]; do printf '%s' "$2"; r=$((r+1)); done; }
sz() { set -- $(wc -c <"$1"); echo "$1"; }
: >empty || pc_fail "cannot write empty"
pc_test
chk() {
	dd if=empty of=nf obs=100 seek="$1" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd seek=$1 exited with status $rc ($2)"
	[ -f nf ] || pc_fail "dd seek=$1 left no output file ($2)"
	s=$(sz nf)
	[ "$s" -eq "$3" ] || pc_fail "seek=$1 with an empty input, $2: output size $s, wanted $3"
}
rm -f nf
chk 3 "new output file" 300
rep 50 X >nf || pc_fail "cannot write nf"
chk 3 "old size 50" 300
gen 0 5 >nf || pc_fail "cannot write nf"
chk 2 "old size 500" 200
pc_pass
