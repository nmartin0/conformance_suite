# m4: ifdef treats a macro with empty text as undefined
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifdef_empty_undefined "$@"; }
