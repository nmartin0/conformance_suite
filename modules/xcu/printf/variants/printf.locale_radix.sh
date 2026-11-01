# a conforming printf with the floating-point conversions that honours the locale
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the floating-point conversions are optional, and a system
# that provides them writes the radix character of the locale in effect
# (XCU printf, ENVIRONMENT VARIABLES, LC_NUMERIC; XBD 8.2).
. "${PC_MUTANT_FILE%/*}/../mutants/printfref.sh"
pc_pfref_both locale_radix
