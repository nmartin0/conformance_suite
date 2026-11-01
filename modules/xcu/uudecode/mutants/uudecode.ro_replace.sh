# uudecode removes an unwritable existing file and creates it anew
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ro_replace "$@"; }
pc_uu_install ro_replace
