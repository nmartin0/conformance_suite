# a conforming expr whose diagnostics are several lines long; the format is unspecified
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/exprwrap.sh"
pc_exprmut diag_multiline
