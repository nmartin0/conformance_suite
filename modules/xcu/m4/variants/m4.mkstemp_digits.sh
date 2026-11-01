# a conforming m4 whose mkstemp names use digits only in the replaced part
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: the characters that replace the template are not stated; only the pathname is the defining text (XCU m4, mkstemp).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V mkstemp_digits "$@"; }
