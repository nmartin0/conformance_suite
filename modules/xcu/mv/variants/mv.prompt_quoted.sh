# a conforming mv -i whose question quotes the destination and has no newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text fixes only that the prompt contains the destination pathname and goes to standard error (XCU mv STDERR).
mv() { . "${PC_MUTANT_FILE%/*}/../mutants/mvref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_mvi prompt_quoted "$2" "$3"; else command mv "$@"; fi; }
