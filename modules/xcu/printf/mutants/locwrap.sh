# Shared by the locale faults: a standalone printf placed first on PATH that
# alters the locale variables and then runs the next printf on PATH.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_locwrap 'COMMANDS' : COMMANDS run in the wrapper before the real utility.
pc_locwrap() {
	pc_lwdir=$PWD/pc-mutbin
	mkdir -p pc-mutbin
	{
		echo '#!/bin/sh'
		echo "PATH=\${PATH#\"$pc_lwdir:\"}"
		echo "$1"
		echo 'exec printf "$@"'
	} >pc-mutbin/printf
	chmod +x pc-mutbin/printf
	PATH=$pc_lwdir:$PATH
}
