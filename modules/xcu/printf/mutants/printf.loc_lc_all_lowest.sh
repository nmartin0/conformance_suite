# LC_ALL is consulted only when LC_NUMERIC is unset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/locwrap.sh"
pc_locwrap 'if [ -n "${LC_NUMERIC:-}" ]; then unset LC_ALL; fi'
