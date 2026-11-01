# a conforming chmod -R that changes a directory after the files below it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text lets implementations change directories before or after their contents (XCU chmod APPLICATION USAGE).
. "${PC_MUTANT_FILE%/*}/../mutants/chmodref.sh"
chmod() { pc_chmodref R_dirs_last "$@"; }
