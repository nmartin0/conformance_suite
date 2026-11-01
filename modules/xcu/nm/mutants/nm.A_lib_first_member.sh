# -A prefix of a library names the first member for every symbol
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref A_lib_first_member "$@"; }
