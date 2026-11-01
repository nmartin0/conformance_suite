# test and [ are one program that looks at the basename of its argv[0] to tell which it is
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text says the two utilities may be implemented as a
# single linked utility that examines the basename of the zeroth argument
# (XCU test DESCRIPTION).
mkdir -p pc-mutbin
cat >pc-mutbin/pcsingle <<'PC_EOF'
#!/bin/sh
case ${0##*/} in
'[')
	pc_k=$#
	pc_i=1
	while [ "$pc_i" -lt "$pc_k" ]; do
		set -- "$@" "$1"
		shift
		pc_i=$((pc_i + 1))
	done
	if [ "$pc_k" -lt 1 ] || [ "$1" != ] ]; then exit 2; fi
	shift
	;;
esac
test "$@"
PC_EOF
chmod +x pc-mutbin/pcsingle
ln -s pcsingle pc-mutbin/test
ln -s pcsingle 'pc-mutbin/['
PATH=$PWD/pc-mutbin:$PATH
