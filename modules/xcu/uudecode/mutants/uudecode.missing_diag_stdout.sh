# uudecode prints the diagnostic on standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref missing_diag_stdout "$@"; }
pc_uu_install missing_diag_stdout
