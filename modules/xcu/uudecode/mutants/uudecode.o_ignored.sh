# uudecode ignores -o and uses the name from the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref o_ignored "$@"; }
pc_uu_install o_ignored
