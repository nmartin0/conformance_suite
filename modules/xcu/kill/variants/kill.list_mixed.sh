# a conforming kill -l: five names per line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/killref.sh"
pc_killmut v_list_mixed "${PC_MUTANT_FILE%/*}/../mutants"
