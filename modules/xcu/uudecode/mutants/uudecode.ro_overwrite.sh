# uudecode makes an unwritable existing file writable and overwrites it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ro_overwrite "$@"; }
pc_uu_install ro_overwrite
