# a conforming cp -i whose question quotes the destination and has no newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text fixes only that the prompt contains the
# destination pathname and goes to standard error (XCU cp STDERR).
cp() { . "${PC_MUTANT_FILE%/*}/../mutants/cpref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_cpi prompt_quoted "$2" "$3"; else command cp "$@"; fi; }
