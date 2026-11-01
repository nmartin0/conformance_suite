# find: a dangling link as -newer file counts as very old
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap newer_dangling_old
