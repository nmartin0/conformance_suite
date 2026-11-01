# the POSIX locale is always used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/locwrap.sh"
pc_locwrap 'unset LC_ALL LANG LC_NUMERIC'
