# the question names the file with its full path
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the prompt must contain the pathname; an absolute form still contains it (STDERR)
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm prompt_abs "$@"; }
