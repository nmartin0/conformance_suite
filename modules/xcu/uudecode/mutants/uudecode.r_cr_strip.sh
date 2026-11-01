# uudecode removes carriage returns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_cr_strip "$@"; }
pc_uu_install r_cr_strip
