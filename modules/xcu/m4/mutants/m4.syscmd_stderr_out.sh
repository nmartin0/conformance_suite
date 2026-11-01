# m4: syscmd sends the command standard error to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F syscmd_stderr_out "$@"; }
