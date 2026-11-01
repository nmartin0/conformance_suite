# m4: changequote uses three characters of the begin-quote
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F changequote_trunc3 "$@"; }
