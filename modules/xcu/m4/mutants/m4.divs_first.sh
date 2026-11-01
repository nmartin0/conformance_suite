# m4: the buffers are written before the normal output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F divs_first "$@"; }
