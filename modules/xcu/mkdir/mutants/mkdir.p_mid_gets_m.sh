# -p -m gives the intermediates the -m mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref p_mid_gets_m "$@"; }
