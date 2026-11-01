# a lone dot-dot operand goes to the parent of the physical directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in
	..) command cd "$(command pwd -P)/.."; return $?;;
	-L) if [ "$2" = .. ]; then command cd -L "$(command pwd -P)/.."; return $?; fi;;
	esac
	command cd "$@"; }
