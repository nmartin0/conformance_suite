# uniq -s compares the whole line when -s equals its length
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref s_ge_whole "$@"; }
