# -b below 9 is refused with gzip
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm b_gz_low_rejected "$@"; }
