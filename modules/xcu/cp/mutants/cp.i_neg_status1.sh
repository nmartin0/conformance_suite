# cp -i fault: neg_status1 (see pc_cpi in cpref.sh)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( . "${PC_MUTANT_FILE%/*}/cpref.sh"; if [ "${1-}" = -i ] && [ $# -eq 3 ]; then pc_cpi neg_status1 "$2" "$3"; else command cp "$@"; fi )
