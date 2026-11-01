# an affirmative reply does not move the file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_mvi yes_ignored "$2" "$3"; else command mv "$@"; fi; }
