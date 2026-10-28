# When a utility creates a regular file that already exists as a regular file, the file is truncated to zero length, its owner, group and permission bits do not change, and its modification time is updated; tee, a shell redirection, sort -o, dd of=, sed w and an awk redirection all behave so
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.004
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
check() {
	# check label command...  (the command truncates f)
	label=$1; cmd=$2
	printf 'old data\n' >f || pc_unresolved "cannot write f"
	chmod 604 f || pc_unresolved "chmod"
	touch -t 200001010000 f || pc_unresolved "touch -t"
	set -- $(ls -ln f); u=$3; g=$4
	( umask 077; eval "$cmd" ) >/dev/null 2>&1 </dev/null || pc_fail "$label: failed"
	[ ! -s f ] || pc_fail "$label: the file was not truncated"
	m=$(ls -ln f | cut -c 1-10)
	[ "$m" = "-rw----r--" ] || pc_fail "$label: the mode changed to $m"
	set -- $(ls -ln f)
	[ "$3" = "$u" ] && [ "$4" = "$g" ] || pc_fail "$label: the owner or group changed"
	[ "$(find f -mtime -1 -print)" = f ] || pc_fail "$label: the modification time was not updated"
}
check "tee" "tee f"
check "redirect" ": >f"
check "sort -o" "sort -o f /dev/null"
check "dd" "dd if=/dev/null of=f"
check "sed w" "sed -n 'w f' /dev/null"
check "awk" "awk 'BEGIN { printf \"\" > \"f\" }'"
pc_pass
