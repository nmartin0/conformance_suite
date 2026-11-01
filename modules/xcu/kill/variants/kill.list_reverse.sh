# a conforming kill -l: names in another order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/killref.sh"
pc_killmut v_list_reverse "${PC_MUTANT_FILE%/*}/../mutants"
