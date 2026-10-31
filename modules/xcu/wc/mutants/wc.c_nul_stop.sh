# bytes are counted only up to the first null byte
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref c_nul_stop "$@"; }
