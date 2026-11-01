# a conforming kill -l: all names on one line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/killref.sh"
pc_killmut v_list_horizontal "${PC_MUTANT_FILE%/*}/../mutants"
