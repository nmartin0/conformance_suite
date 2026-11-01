# chmod planted fault: copy_plus_noop (see chmodref.sh for what it changes)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/chmodref.sh"
chmod() { pc_chmodref copy_plus_noop "$@"; }
