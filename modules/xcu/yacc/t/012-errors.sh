# erroneous invocations
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.012
# A missing grammar file, a missing operand and a non-terminal without a
# rule give a non-zero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '%%%%\ns : x ;\n' >bad.y
cat >ok.y <<'EOY'
%%
s : 'a' ;
EOY
pc_test
yacc nosuchgrammar.y </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "a nonexistent grammar file gave status 0"
yacc </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "yacc without the required operand gave status 0"
yacc bad.y </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "a non-terminal without a rule gave status 0"
yacc ok.y </dev/null >out 2>err || pc_fail "a valid grammar gave a nonzero status"
pc_pass
