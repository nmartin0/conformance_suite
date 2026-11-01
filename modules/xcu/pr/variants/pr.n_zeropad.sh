# pr: line numbers are padded with zeros instead of blanks (allowed: the text fixes the width of the number field, not the padding)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pr() {
	pc_w=
	for pc_a
	do
		case $pc_a in -n) pc_w=5 ;; -n[0-9]*) pc_w=${pc_a#-n} ;; -n[!0-9]*) pc_w=${pc_a#-n}; pc_w=${pc_w#?}; pc_w=${pc_w:-5} ;; esac
	done
	if [ -z "$pc_w" ]; then command pr "$@"; return; fi
	command pr "$@" | awk -v w="$pc_w" '{ f = substr($0, 1, w); gsub(/ /, "0", f); print f substr($0, w + 1) }'
}
