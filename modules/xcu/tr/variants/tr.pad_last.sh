# a conforming tr that pads a short string2 with its last character
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text leaves open the result when string2 is shorter
# than string1, and names the padding with the last character as one of the
# historical behaviours (XCU tr EXTENDED DESCRIPTION, APPLICATION USAGE).
. "${PC_MUTANT_FILE%/*}/../mutants/trref.sh"
tr() { pc_trref pad_last "$@"; }
