# The text and layout of the -v message are not fixed: here a different
# wording, a figure written another way and a blank line after it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/cmut.sh"
compress() { pc_cm v_text "$@"; }
