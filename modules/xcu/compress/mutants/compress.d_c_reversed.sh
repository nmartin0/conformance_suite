# operands are decompressed to standard output in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm d_c_reversed "$@"; }
