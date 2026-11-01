# no diagnostic when a file cannot be removed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone rm, first on PATH, so that a child process finds it too.
pc_real=$(command -v rm)
mkdir -p pc-mutbin
{
	printf '#!/bin/sh\nPC_RMBIN=%s\nexport PC_RMBIN\n' "$pc_real"
	cat "${PC_MUTANT_FILE%/*}/rmref.sh"
	printf 'pc_rm perm_silent "$@"\n'
} >pc-mutbin/rm
chmod +x pc-mutbin/rm
PATH=$PWD/pc-mutbin:$PATH
