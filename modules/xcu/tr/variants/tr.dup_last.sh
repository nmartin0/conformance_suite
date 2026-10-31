# a conforming tr that lets the last of a repeated character of string1 win
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text leaves the result open when a character occurs
# more than once in string1 (XCU tr EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/trref.sh"
tr() { pc_trref dup_last "$@"; }
