# a conforming tr that lists the members of a class in descending order, except for case conversion
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text says the characters of a class go into the
# array in an unspecified order, case conversion excepted (XCU tr EXTENDED
# DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/trref.sh"
tr() { pc_trref class_reverse "$@"; }
