# ln refuses a target directory written with a trailing slash
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	for pc_last; do :; done
	case $pc_last in
	*/) echo "ln: $pc_last: bad target" >&2; return 1 ;;
	esac
	command ln "$@"
}
