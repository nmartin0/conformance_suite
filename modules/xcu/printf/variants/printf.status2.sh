# a conforming printf whose error status is 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text asks only for a status other than zero when an
# argument cannot be converted (XCU printf, EXTENDED DESCRIPTION and EXIT
# STATUS).
. "${PC_MUTANT_FILE%/*}/../mutants/printfref.sh"
pc_pfref_both status2
