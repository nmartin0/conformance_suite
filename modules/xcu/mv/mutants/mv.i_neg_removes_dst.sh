# a negative reply removes the destination
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_mvi neg_removes_dst "$2" "$3"; else command mv "$@"; fi; }
