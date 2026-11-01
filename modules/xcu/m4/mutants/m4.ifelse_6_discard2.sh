# m4: discards two arguments, not three, when ifelse has six or more
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifelse_6_discard2 "$@"; }
