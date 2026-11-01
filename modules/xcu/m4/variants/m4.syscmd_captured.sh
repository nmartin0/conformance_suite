# a conforming m4 that collects the output of a command and inserts it, not scanned
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: the defining text of syscmd is the string result of the command, which is not scanned again (XCU m4, syscmd).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V syscmd_captured "$@"; }
