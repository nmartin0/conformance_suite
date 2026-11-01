# a conforming expr that exits 100 for a division or remainder by zero; any status above 1 is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/exprwrap.sh"
pc_exprmut divzero_exit100
