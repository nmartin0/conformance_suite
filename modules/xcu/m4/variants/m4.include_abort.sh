# a conforming m4 that stops at the first include that cannot read its file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: a utility may terminate early when it has difficulty accessing a file (XCU 1.4, CONSEQUENCES OF ERRORS); include of an unreadable file is an error (XCU m4, include).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V include_abort "$@"; }
