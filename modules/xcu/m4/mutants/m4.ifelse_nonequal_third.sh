# m4: gives the third argument of a three-argument ifelse when the strings differ
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifelse_nonequal_third "$@"; }
