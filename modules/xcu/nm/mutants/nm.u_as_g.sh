# -u acts as -g
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref u_as_g "$@"; }
