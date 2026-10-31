# [:xdigit:] holds no lowercase hexadecimal letters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref xdigit_nolower "$@"; }
