# uudecode clears bit 7 of every byte
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_hibit "$@"; }
pc_uu_install r_hibit
