# a conforming chmod written independently of the real one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text fully defines the result of every mode used by the tests (XCU chmod EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/chmodref.sh"
chmod() { pc_chmodref none "$@"; }
