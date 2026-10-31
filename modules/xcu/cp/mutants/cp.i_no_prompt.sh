# with -i no question is written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( . "${PC_MUTANT_FILE%/*}/cpref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_cpi no_prompt "$2" "$3"; else command cp "$@"; fi )
