# a conforming cp -i whose question is two lines ending in a newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text fixes only that the prompt contains the
# destination pathname and goes to standard error (XCU cp STDERR); the reply
# is one line of standard input.
cp() { . "${PC_MUTANT_FILE%/*}/../mutants/cpref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_cpi prompt_nl "$2" "$3"; else command cp "$@"; fi; }
