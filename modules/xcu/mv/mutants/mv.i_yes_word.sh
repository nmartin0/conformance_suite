# only the full word yes is taken as affirmative
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_mvi yes_word "$2" "$3"; else command mv "$@"; fi; }
