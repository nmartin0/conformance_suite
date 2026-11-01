# m4: translit keeps characters with no replacement
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F translit_nodelete "$@"; }
