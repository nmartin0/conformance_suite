# uudecode creates no output file but exits 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref no_create "$@"; }
pc_uu_install no_create
