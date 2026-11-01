# m4: gives the fifth argument of a five-argument ifelse when the strings differ
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifelse_5args_fifth "$@"; }
