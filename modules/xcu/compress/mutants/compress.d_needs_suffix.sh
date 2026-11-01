# -d -c refuses a file without a known suffix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm d_needs_suffix "$@"; }
