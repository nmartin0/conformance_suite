# a conforming m4 whose line directives carry no file name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: -s asks for directives of the preprocessing kind and the text fixes no operands for them (XCU m4, OPTIONS, -s).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V sync_nofile "$@"; }
