# find: a null LC_ALL overrides the other variables
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap loc_all_null_wins
