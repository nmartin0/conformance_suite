# -e leaves out the data symbols
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref e_drops_data "$@"; }
