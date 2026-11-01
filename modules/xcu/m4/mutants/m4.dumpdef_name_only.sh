# m4: dumpdef writes only the names
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F dumpdef_name_only "$@"; }
