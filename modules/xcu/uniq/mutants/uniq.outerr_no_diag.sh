# uniq writes no diagnostic when output_file cannot be written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref outerr_no_diag "$@"; }
