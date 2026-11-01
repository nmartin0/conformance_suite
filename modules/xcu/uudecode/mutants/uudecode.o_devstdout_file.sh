# uudecode treats -o /dev/stdout as a plain file name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref o_devstdout_file "$@"; }
pc_uu_install o_devstdout_file
