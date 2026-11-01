# a conforming m4 whose define replaces the whole stack of definitions of a pushed macro
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: it is unspecified whether define changes only the newest definition or replaces all of them (XCU m4, define, and APPLICATION USAGE).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V define_replaces_stack "$@"; }
