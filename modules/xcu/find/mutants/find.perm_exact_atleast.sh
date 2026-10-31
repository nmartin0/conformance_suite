# find: -perm mode with no hyphen matches at-least
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap perm_exact_atleast
