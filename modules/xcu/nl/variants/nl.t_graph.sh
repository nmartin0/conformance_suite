# conforming nl: -b t, -h t and -f t number only lines that hold a graphic character
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the page ties the t type to the graph class of the locale (XCU nl ENVIRONMENT VARIABLES, LC_CTYPE).
PC_NLREF_DIR=${PC_MUTANT_FILE%/*}/../mutants
. "$PC_NLREF_DIR/nlref.sh"
nl() { pc_nlref t_graph "$@"; }
