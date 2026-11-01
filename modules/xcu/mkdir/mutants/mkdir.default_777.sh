# default mode ignores the file creation mask
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref default_777 "$@"; }
