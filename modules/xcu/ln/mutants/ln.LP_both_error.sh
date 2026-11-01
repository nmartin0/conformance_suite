# ln rejects -L and -P given together
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_l= pc_p=
	for pc_a; do
		case $pc_a in -L) pc_l=1 ;; -P) pc_p=1 ;; esac
	done
	if [ -n "$pc_l" ] && [ -n "$pc_p" ]; then echo "ln: -L and -P conflict" >&2; return 1; fi
	command ln "$@"
}
