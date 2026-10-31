# find: -exec ... ; is true whatever the utility exits with
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap exec_semi_true
