# a conforming ar that handles several file operands last to first
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text does not fix the order in which several file
# operands are listed, written, moved, deleted or added (XCU ar OPERANDS and
# STDOUT give the lines per file but no order among operands). Names are
# assumed to hold no blank or pattern character.
ar() {
	pc_h=0; pc_skip=0
	for pc_w in "$@"; do
		if [ "$pc_skip" -eq 1 ]; then pc_skip=0; pc_h=$((pc_h + 1)); continue; fi
		case $pc_w in
		-a|-b|-i) pc_skip=1; pc_h=$((pc_h + 1)) ;;
		-?*) pc_h=$((pc_h + 1)) ;;
		*) break ;;
		esac
	done
	if [ "$pc_h" -eq 0 ] || [ $# -le $((pc_h + 2)) ]; then command ar "$@"; return; fi
	pc_head=; pc_i=0
	while [ "$pc_i" -le "$pc_h" ]; do pc_head="$pc_head $1"; shift; pc_i=$((pc_i + 1)); done
	pc_ops=
	for pc_x; do pc_ops="$pc_x $pc_ops"; done
	command ar $pc_head $pc_ops
}
