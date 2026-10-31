# uniq writes no diagnostic when input_file cannot be read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref missing_no_diag "$@"; }
