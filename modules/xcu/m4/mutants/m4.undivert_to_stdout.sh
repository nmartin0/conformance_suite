# m4: undivert writes to the normal output whatever the current stream
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F undivert_to_stdout "$@"; }
