# a conforming chmod that rejects a mode written like an option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the utility follows the syntax guidelines (XCU chmod OPTIONS): only -R is an option, and a leading-hyphen mode needs the end-of-options marker (APPLICATION USAGE shows chmod -- -w file).
. "${PC_MUTANT_FILE%/*}/../mutants/chmodref.sh"
chmod() { pc_chmodref strict_opts "$@"; }
