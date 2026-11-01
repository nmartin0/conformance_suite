# uudecode treats /dev/stdout from the data as a plain file name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref data_devstdout_file "$@"; }
pc_uu_install data_devstdout_file
