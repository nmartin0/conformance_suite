# a conforming chmod that ignores a request to set set-user-ID or set-group-ID when no execute bit is set
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text makes this implementation-defined (XCU chmod EXTENDED DESCRIPTION, symbolic mode on regular files).
. "${PC_MUTANT_FILE%/*}/../mutants/chmodref.sh"
chmod() { pc_chmodref s_ignored_noexec "$@"; }
