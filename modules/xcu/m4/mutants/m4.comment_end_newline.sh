# m4: ends every comment at a newline whatever the end string
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F comment_end_newline "$@"; }
