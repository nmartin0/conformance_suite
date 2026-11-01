# m4: eval gives << and >> a higher precedence than + and -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F eval_shift_above_add "$@"; }
