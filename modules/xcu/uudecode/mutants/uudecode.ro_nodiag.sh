# uudecode refuses an unwritable file with no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ro_nodiag "$@"; }
pc_uu_install ro_nodiag
