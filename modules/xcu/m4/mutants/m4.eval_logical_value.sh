# m4: eval gives && and || the value of an operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F eval_logical_value "$@"; }
