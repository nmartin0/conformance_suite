# With -s, the options -L and -P are silently ignored and a symbolic link is made.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.011
# MUTANT    ln.sL_hard
# MUTANT    ln.sP_hard
# MUTANT    ln.sLP_error
# MUTANT    ln.sL_warns
# MUTANT    ln.sL_follows
# MUTANT    ln.ok_status1
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.011
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

printf 'd\n' >f || pc_fail "cannot create f"
ln -s f sl || pc_fail "cannot create symbolic link"
pc_test
n=0
for opts in "-s -L" "-s -P" "-L -s" "-P -s"; do
	for src in f sl; do
		n=$((n + 1))
		ln $opts $src s$n >out 2>err </dev/null
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "ln $opts $src exited with status $rc"
		[ ! -s err ] || pc_fail "ln $opts $src wrote a message (the option must be silently ignored)"
		[ ! -s out ] || pc_fail "ln $opts $src wrote to standard output"
		[ -h s$n ] || pc_fail "ln $opts $src: s$n is not a symbolic link"
		linkof s$n || pc_fail "cannot list s$n"
		[ "$lk" = $src ] || pc_fail "ln $opts $src: contents are '$lk', expected $src"
	done
done
pc_pass
