# [:alnum:] holds no digits
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref alnum_nodigit "$@"; }
