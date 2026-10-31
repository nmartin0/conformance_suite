# with -d and -s a class in string2 is taken as plain characters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref s2_class_literal "$@"; }
