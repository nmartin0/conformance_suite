# find: a dangling link is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap dangling_error
