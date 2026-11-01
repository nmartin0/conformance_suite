# uudecode with -o also creates the file from the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref o_both "$@"; }
pc_uu_install o_both
