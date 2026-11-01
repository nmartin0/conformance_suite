# pr: multi-column body lines are padded with blanks to the full line width (allowed: the text fixes the width of the line and of each column, not whether trailing blanks are written)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pr() {
	pc_m= pc_w= pc_s= pc_prev=
	for pc_a
	do
		case $pc_a in -m | -[0-9] | -[0-9][0-9]) pc_m=1 ;; -s*) pc_s=1 ;; esac
		[ "$pc_prev" = -w ] && pc_w=$pc_a
		case $pc_a in -w?*) pc_w=${pc_a#-w} ;; esac
		pc_prev=$pc_a
	done
	if [ -z "$pc_m" ] || [ -n "$pc_s" ]; then command pr "$@"; return; fi
	command pr "$@" | awk -v w="${pc_w:-72}" '{
		if ($0 ~ / Page [0-9]+$/) { print; next }
		col = 0
		for (i = 1; i <= length($0); i++) { if (substr($0, i, 1) == "\t") col = (int(col / 8) + 1) * 8; else col++ }
		out = $0
		if (col > 0) for (; col < w; col++) out = out " "
		print out }'
}
