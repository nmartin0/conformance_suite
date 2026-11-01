# find: -name with a slash in the pattern matches the whole pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/findref.sh"
pc_findwrap name_slash_path
