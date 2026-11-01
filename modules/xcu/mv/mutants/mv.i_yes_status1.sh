# an affirmative reply gives status 1 after moving
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_mvi yes_status1 "$2" "$3"; else command mv "$@"; fi; }
