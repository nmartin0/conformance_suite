# a conforming diff whose -e script inserts before a line (i) where it could append after the previous one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the -e script may use a, c, d, i and s (Diff -e Output Format); appending after line N equals inserting before N+1.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref e_insert "$@"; }
