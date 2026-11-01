# An -i after -f cancels it: rm prompts, and a negative reply keeps the file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.012
# MUTANT    rm.i_after_f_ignored
# MUTANT    rm.i_no_prompt
# MUTANT    rm.i_prompt_no_name
# MUTANT    rm.i_prompt_stdout
# MUTANT    rm.i_neg_removes
# VARIANT   rm.prompt_nl
# VARIANT   rm.prompt_quoted
# VARIANT   rm.prompt_abs
# VARIANT   rm.ref
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
