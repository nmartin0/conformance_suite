# the question quotes the pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the format of the prompt is unspecified beyond containing the pathname (STDERR)
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm prompt_quoted "$@"; }
