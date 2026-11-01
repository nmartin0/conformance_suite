# m4: mkstemp always gives the same name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F mkstemp_fixed "$@"; }
