# directory entries are removed in the reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the order in which a directory's entries are taken is not specified (DESCRIPTION step 2c)
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm reverse_order "$@"; }
