# m4: gives the fourth argument of an ifelse with six or more arguments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifelse_6_stop "$@"; }
