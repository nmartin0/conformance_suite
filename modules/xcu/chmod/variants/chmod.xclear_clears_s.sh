# a conforming chmod where clearing all execute bits of a regular file also clears set-user-ID and set-group-ID
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text makes this implementation-defined (XCU chmod EXTENDED DESCRIPTION, symbolic mode on regular files).
. "${PC_MUTANT_FILE%/*}/../mutants/chmodref.sh"
chmod() { pc_chmodref xclear_clears_s "$@"; }
