# -p replaces a file in the path prefix by a directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref p_file_replaced "$@"; }
