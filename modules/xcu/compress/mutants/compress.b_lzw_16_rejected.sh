# -b 16 is refused with lzw
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm b_lzw_16_rejected "$@"; }
