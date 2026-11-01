# the prompt does not name the destination
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_mvi prompt_no_name "$2" "$3"; else command mv "$@"; fi; }
