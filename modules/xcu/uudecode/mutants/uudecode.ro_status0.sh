# uudecode refuses an unwritable file but exits 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ro_status0 "$@"; }
pc_uu_install ro_status0
