# -f leaves out static symbols that the default lists
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref f_drops_local "$@"; }
