# An -i after -f cancels it: rm prompts, and a negative reply keeps the file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.012
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.012
printf x >victim || pc_fail "cannot create victim"
pc_test
printf 'n\n' | rm -f -i victim >out 2>err
[ -f victim ] || pc_fail "victim was removed although -i came last and the reply was negative"
# the prompt goes to standard error and contains the pathname
perr=
while IFS= read -r pl || [ -n "$pl" ]; do perr="$perr$pl"; done <err
case $perr in
*victim*) ;;
*) pc_fail "no prompt naming victim on standard error although -i came last (got '$perr')" ;;
esac
pc_pass
