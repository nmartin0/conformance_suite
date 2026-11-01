# conforming nl: the lines that start a section are not written at all
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the page does not say what is written for a section-start line (XCU nl STDOUT gives the format only for numbered and unnumbered text lines).
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}/../mutants
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref delim_dropped "$@"; }
