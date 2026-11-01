# m4: output before m4exit is lost
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F m4exit_discards "$@"; }
