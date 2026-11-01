# a conforming alias whose failure status is 255
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text asks only for a status greater than zero (alias EXIT STATUS)
PCM_FAULT=missing_status255
. "${PC_MUTANT_FILE%/*}/../mutants/aliasref.sh"
