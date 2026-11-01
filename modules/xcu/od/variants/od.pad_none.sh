# numbers are written without leading zeros
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: The width and zero padding of numbers is not fixed by the page.
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref pad_none "$@"; }
