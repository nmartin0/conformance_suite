# uudecode refuses to overwrite an existing file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref ow_refuse "$@"; }
pc_uu_install ow_refuse
