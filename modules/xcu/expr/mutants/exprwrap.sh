# Shared by the expr faults and variants: installs a standalone expr, first
# on PATH, that runs the real one and alters one thing (see exprbody.sh).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_exprmut NAME: NAME selects the behaviour in exprbody.sh. A shell
# function of the same name routes calls by the shell to the standalone too,
# so a built-in of the shell cannot bypass it.
pc_exprmut() {
	pcx_real=$(command -v expr)
	case $pcx_real in
	/*) ;;
	*) pcx_real=/usr/bin/expr ;;
	esac
	pcx_body=${PC_MUTANT_FILE%/*}/exprbody.sh
	[ -f "$pcx_body" ] || pcx_body=${PC_MUTANT_FILE%/*}/../mutants/exprbody.sh
	mkdir -p pc-mutbin
	{
		printf '#!/bin/sh\nREAL=%s\nM=%s\n' "'$pcx_real'" "'$1'"
		cat "$pcx_body"
	} >pc-mutbin/expr
	chmod +x pc-mutbin/expr
	PATH=$PWD/pc-mutbin:$PATH
	pcx_bin=$PWD/pc-mutbin/expr
	expr() { "$pcx_bin" "$@"; }
}
