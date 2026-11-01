# m4: the contents of the included file are not scanned for macros
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F include_unscanned "$@"; }
