# uudecode leaves out the last decoded byte
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref drop_last "$@"; }
pc_uu_install drop_last
