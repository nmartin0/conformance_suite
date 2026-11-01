# a -j beyond the input writes the diagnostic to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref j_short_stdout "$@"; }
