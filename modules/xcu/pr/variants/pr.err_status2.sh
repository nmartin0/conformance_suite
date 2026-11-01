# pr: a failure is reported with exit status 2 (allowed: any value above zero means an error occurred)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pr() {
	command pr "$@"
	pc_vr=$?
	[ "$pc_vr" -eq 0 ] || return 2
	return 0
}
