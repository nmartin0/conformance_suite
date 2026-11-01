# uudecode decodes the Base64 format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.004
# MUTANT    uudecode.b64_unsupported
# MUTANT    uudecode.b64_pad_kept
# MUTANT    uudecode.b64_first_line_only
# MUTANT    uudecode.drop_last
# MUTANT    uudecode.append_nl
# MUTANT    uudecode.status1
# Data in the begin-base64 format ending in the "====" line decodes to the
# original bytes: with no padding, with one or two padding characters (the
# decoded size is not a multiple of three) and over more than one line.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin-base64 644 b.out\nQ2F0\n====\n' >in1 || pc_unresolved "setup: cannot write input"
printf 'begin-base64 644 b2.out\nQ2E=\n====\n' >in2 || pc_unresolved "setup: cannot write input"
printf 'begin-base64 644 b3.out\nQw==\n====\n' >in3 || pc_unresolved "setup: cannot write input"
{
	printf 'begin-base64 644 b4.out\n'
	i=0; while [ $i -lt 19 ]; do printf Q2F0; i=$((i + 1)); done; printf '\n'
	printf 'Q2F0Q2F0\n====\n'
} >in4 || pc_unresolved "setup: cannot write input"
printf Cat >exp1; printf Ca >exp2; printf C >exp3
i=0; while [ $i -lt 21 ]; do printf Cat; i=$((i + 1)); done >exp4 || pc_unresolved "setup: cannot write expectations"
pc_test
for n in 1 2 3 4; do
	uudecode in$n >stdout 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "input $n: exit status $rc"
	f=b.out; [ "$n" -gt 1 ] && f=b$n.out
	[ -f "$f" ] || pc_fail "input $n: $f was not created"
	cmp -s "exp$n" "$f" || pc_fail "input $n: decoded content is wrong"
done
pc_pass
