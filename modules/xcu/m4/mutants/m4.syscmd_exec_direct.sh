# m4: syscmd does not use a shell for the command line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F syscmd_exec_direct "$@"; }
