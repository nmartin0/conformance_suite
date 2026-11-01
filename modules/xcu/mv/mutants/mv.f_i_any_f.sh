# any -f stops mv prompting
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; pc_mvopt anyf "$@"; }
