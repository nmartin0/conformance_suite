# a conforming expr that exits 3 (another error) for a division or remainder by zero; the page leaves the class open
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/exprwrap.sh"
pc_exprmut divzero_exit3
