# cp -i fault: yes_ignored (see pc_cpi in cpref.sh)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( . "${PC_MUTANT_FILE%/*}/cpref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_cpi yes_ignored "$2" "$3"; else command cp "$@"; fi )
