# rm reimplemented without a fault
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the reference reimplementation follows the text; it must be tolerated
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm ref "$@"; }
