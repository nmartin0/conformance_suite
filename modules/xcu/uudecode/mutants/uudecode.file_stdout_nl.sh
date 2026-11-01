# uudecode writes a newline to standard output when the output is a file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref file_stdout_nl "$@"; }
pc_uu_install file_stdout_nl
