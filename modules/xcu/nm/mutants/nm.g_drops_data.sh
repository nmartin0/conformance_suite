# -g leaves out the global data symbols
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref g_drops_data "$@"; }
