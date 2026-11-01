# the compressed file is kept after it was decompressed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm d_keeps_input "$@"; }
