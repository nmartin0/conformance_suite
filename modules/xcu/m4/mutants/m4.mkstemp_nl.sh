# m4: mkstemp adds a newline to the name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F mkstemp_nl "$@"; }
