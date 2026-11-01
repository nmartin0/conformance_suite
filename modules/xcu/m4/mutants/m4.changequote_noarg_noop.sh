# m4: leaves the quote strings alone when changequote has no arguments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F changequote_noarg_noop "$@"; }
