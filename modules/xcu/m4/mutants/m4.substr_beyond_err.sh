# m4: substr reports an error when the start is past the end
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F substr_beyond_err "$@"; }
