# a conforming m4 with another trace and dumpdef format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: the format of trace output is unspecified (XCU m4, traceon and STDERR); it still goes to standard error.
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V trace_format "$@"; }
