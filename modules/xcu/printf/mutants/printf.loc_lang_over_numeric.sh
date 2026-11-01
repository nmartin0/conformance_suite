# LANG wins over LC_NUMERIC
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/locwrap.sh"
pc_locwrap 'if [ -n "${LANG:-}" ] && [ -z "${LC_ALL:-}" ]; then unset LC_NUMERIC; fi'
