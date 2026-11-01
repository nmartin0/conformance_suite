# m4: does not expand macros while collecting arguments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F args_not_expanded "$@"; }
