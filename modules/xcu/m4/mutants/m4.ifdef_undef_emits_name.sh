# m4: ifdef with an undefined name and no third argument gives the name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F ifdef_undef_emits_name "$@"; }
