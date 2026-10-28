# No environment variable affects false: with a made locale and odd catalog variables it still exits 1 and writes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.false.005
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.false.005
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pc_test
pcl_out=$(env LANG="$PCLOC" LC_ALL="$PCLOC" LC_CTYPE="$PCLOC" LC_MESSAGES="$PCLOC" NLSPATH=/pc-no-such-dir/%N LANGUAGE=pc-no-such-locale false 2>&1 </dev/null)
pcl_rc=$?
[ "$pcl_rc" -eq 1 ] || pc_fail "false exited with status $pcl_rc with a made locale and odd catalog variables"
[ -z "$pcl_out" ] || pc_fail "false wrote [$pcl_out] with a made locale and odd catalog variables"
pc_pass
