# m4: m4exit gives the status modulo 16
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F m4exit_mod16 "$@"; }
