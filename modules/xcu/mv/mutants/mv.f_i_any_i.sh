# any -i makes mv prompt
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { . "${PC_MUTANT_FILE%/*}/mvref.sh"; pc_mvopt anyi "$@"; }
