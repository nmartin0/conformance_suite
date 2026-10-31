# a conforming sort -u that keeps the last of each set of lines with equal keys
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: the text asks that all but one line of each set of equal keys be
# suppressed and does not say which one remains (XCU sort -u, RATIONALE).
PC_SR_DIR=${PC_MUTANT_FILE%/*}/../mutants
. "$PC_SR_DIR/sortref.sh"
sort() { pc_sortref u_last "$@"; }
