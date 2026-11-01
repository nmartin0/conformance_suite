# ln -s stores an absolute path instead of the operand text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -s ] && [ $# -eq 3 ]; then
		case $2 in
		/*) ;;
		*) command ln -s "$PWD/$2" "$3"; return ;;
		esac
	fi
	command ln "$@"
}
