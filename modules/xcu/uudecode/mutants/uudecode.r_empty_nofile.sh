# uudecode creates no file for empty data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_empty_nofile "$@"; }
pc_uu_install r_empty_nofile
