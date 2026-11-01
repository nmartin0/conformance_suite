# default format: the changes are written from the end of the file to the start
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_order_rev "$@"; }
