# ln -s rejects -L and -P
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_s= pc_lp=
	for pc_a; do
		case $pc_a in -s) pc_s=1 ;; -L|-P) pc_lp=1 ;; esac
	done
	if [ -n "$pc_s" ] && [ -n "$pc_lp" ]; then echo "ln: -s conflicts with -L/-P" >&2; return 1; fi
	command ln "$@"
}
