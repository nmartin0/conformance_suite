# the question has two lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the text fixes only that the prompt names the file and goes to standard error (STDERR)
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm prompt_nl "$@"; }
