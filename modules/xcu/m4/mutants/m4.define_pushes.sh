# m4: define keeps the earlier definition like pushdef
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F define_pushes "$@"; }
