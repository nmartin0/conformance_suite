# m4: gives nothing for a four-argument ifelse when the strings differ
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifelse_4args_empty "$@"; }
