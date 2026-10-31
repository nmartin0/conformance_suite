# find: -exec ... {} ; collects the pathnames like {} +
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap exec_semi_batch
