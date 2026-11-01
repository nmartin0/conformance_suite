# a conforming rm, a program of its own on PATH (status per variant: ref_status255)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the reimplementation follows the text; an error status may be any value above zero (EXIT STATUS).
pc_real=$(command -v rm)
mkdir -p pc-mutbin
{
	printf '#!/bin/sh\nPC_RMBIN=%s\nexport PC_RMBIN\n' "$pc_real"
	cat "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"
	printf 'pc_rm ref_status255 "$@"\n'
} >pc-mutbin/rm
chmod +x pc-mutbin/rm
PATH=$PWD/pc-mutbin:$PATH
