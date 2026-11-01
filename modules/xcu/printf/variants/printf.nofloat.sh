# a conforming printf without the optional floating-point conversions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text says the a, A, e, E, f, F, g and G conversions
# need not be supported (XCU printf, EXTENDED DESCRIPTION). It also stands
# for a system whose other behaviour is exactly what the text asks for, so
# every other test must still pass.
. "${PC_MUTANT_FILE%/*}/../mutants/printfref.sh"
pc_pfref_both nofloat
