# uudecode historical format: drops a byte when the size leaves remainder 2 mod 3
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_uu_tail2 "$@"; }
pc_uu_install r_uu_tail2
