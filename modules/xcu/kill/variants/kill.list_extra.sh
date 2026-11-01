# a conforming kill -l: extra names for signals the implementation adds
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/killref.sh"
pc_killmut v_list_extra "${PC_MUTANT_FILE%/*}/../mutants"
