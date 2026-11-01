# m4: translit repeats the last replacement character instead of deleting
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F translit_pad_last "$@"; }
