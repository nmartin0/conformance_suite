# chmod planted fault: g_minus_s_noop (see chmodref.sh for what it changes)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/chmodref.sh"
chmod() { pc_chmodref g_minus_s_noop "$@"; }
