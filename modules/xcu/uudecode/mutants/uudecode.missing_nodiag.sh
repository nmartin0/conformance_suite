# uudecode prints no diagnostic for a missing input file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref missing_nodiag "$@"; }
pc_uu_install missing_nodiag
