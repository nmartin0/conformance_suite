# -t c writes lower-case letters in octal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref c_printable_oct "$@"; }
