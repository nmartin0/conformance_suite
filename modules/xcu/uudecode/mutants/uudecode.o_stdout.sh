# uudecode with -o writes the bytes to standard output instead of the file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref o_stdout "$@"; }
pc_uu_install o_stdout
