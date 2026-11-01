# uudecode -o - also creates the file named in the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref o_dash_also_data "$@"; }
pc_uu_install o_dash_also_data
