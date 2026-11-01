# a conforming printf that writes a null byte for %c of an empty argument
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text leaves it open whether nothing or a null byte is
# written for an empty argument to the c conversion (XCU printf, EXTENDED
# DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/printfref.sh"
pc_pfref_both c_empty_null
