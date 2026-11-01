# uudecode writes the skipped lines into the output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref lead_output "$@"; }
pc_uu_install lead_output
