# uudecode pads the output to a multiple of three bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref r_pad3 "$@"; }
pc_uu_install r_pad3
