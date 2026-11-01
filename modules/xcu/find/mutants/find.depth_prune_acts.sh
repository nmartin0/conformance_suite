# find: -prune stops the descent although -depth is given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap depth_prune_acts
