# find: -print0 separates pathnames by null bytes and writes none at the end
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap print0_sep
