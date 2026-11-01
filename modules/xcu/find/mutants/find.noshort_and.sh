# find: the right operand of -a is evaluated although the left is false
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap noshort_and
