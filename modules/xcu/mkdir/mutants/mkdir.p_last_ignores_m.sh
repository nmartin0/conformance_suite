# -p -m leaves the last directory at the default mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref p_last_ignores_m "$@"; }
