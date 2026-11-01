# m4: syscmd discards the command standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F syscmd_stderr_dropped "$@"; }
