# m4: drops only blanks, not tabs or newlines, at the start of an argument
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F lead_ws_space_only "$@"; }
