# ln -s -L stores the text a source symbolic link refers to
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_s= pc_l=
	for pc_a; do
		case $pc_a in -s) pc_s=1 ;; -L) pc_l=1 ;; esac
	done
	if [ -n "$pc_s" ] && [ -n "$pc_l" ] && [ $# -ge 3 ]; then
		eval pc_src=\${$(($# - 1))} pc_dst=\${$#}
		if [ -h "$pc_src" ]; then
			pc_t=$(ls -ld "$pc_src"); pc_t=${pc_t##* -> }
			command ln -s "$pc_t" "$pc_dst"; return
		fi
	fi
	command ln "$@"
}
