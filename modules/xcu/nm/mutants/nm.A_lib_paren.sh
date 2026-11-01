# -A prefix of a library writes the member in parentheses
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref A_lib_paren "$@"; }
