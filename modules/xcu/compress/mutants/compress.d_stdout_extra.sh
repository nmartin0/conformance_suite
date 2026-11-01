# decompressed data written to standard output has one byte too many
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm d_stdout_extra "$@"; }
