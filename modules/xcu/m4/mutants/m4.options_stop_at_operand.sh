# m4: treats everything after the first operand as an operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F options_stop_at_operand "$@"; }
