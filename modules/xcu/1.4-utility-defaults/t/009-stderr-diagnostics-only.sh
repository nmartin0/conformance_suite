# When a standard utility is used as described and no error occurs, it writes nothing to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.009
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'alpha\nbeta\n' >f1 || pc_unresolved "cannot create a file"
pc_test
chk() {
	"$@" >/dev/null 2>err </dev/null
	[ ! -s err ] || pc_fail "$* wrote to standard error: $(cat err)"
}
chk cat f1
chk head -n 1 f1
chk tail -n 1 f1
chk sort f1
chk cut -c 1 f1
chk wc f1
chk sed -n p f1
chk grep -c a f1
chk uniq f1
chk fold f1
chk paste f1
chk comm f1 f1
chk join f1 f1
chk od f1
chk expand f1
chk cksum f1
chk cmp f1 f1
chk diff f1 f1
chk tee f2 f1
chk ls f1
chk echo hi
chk printf x
chk expr 1 + 1
chk basename /a/b
chk dirname /a/b
chk pwd
chk id
chk uname
chk date
chk test -f f1
chk true
chk mkdir d1
chk touch f3
chk cp f1 f4
chk mv f4 f5
chk ln f1 f6
chk chmod 600 f1
chk rmdir d1
chk rm f5
chk sleep 0
chk env true
chk nice true
pc_pass
