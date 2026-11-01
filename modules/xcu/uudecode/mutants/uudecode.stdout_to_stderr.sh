# uudecode writes the decoded bytes to standard error instead of standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref stdout_to_stderr "$@"; }
pc_uu_install stdout_to_stderr
