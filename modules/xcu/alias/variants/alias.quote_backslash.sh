# a conforming alias that writes values with each special character escaped by a backslash
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text asks only for quoting suitable for reinput (alias STDOUT)
PCM_FAULT=quote_backslash
. "${PC_MUTANT_FILE%/*}/../mutants/aliasref.sh"
