# uniq -f compares the whole line when it has fewer fields
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uniqref.sh"
uniq() { pc_uniqref short_f_whole "$@"; }
