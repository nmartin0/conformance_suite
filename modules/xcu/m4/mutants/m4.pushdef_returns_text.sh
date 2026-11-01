# m4: pushdef gives the defining text as text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F pushdef_returns_text "$@"; }
