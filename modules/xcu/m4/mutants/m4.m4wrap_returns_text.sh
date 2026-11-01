# m4: m4wrap gives its argument as text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F m4wrap_returns_text "$@"; }
