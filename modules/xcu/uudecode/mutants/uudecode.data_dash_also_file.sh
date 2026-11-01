# uudecode - from the data also creates a file named -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref data_dash_also_file "$@"; }
pc_uu_install data_dash_also_file
