# sort leaves lines that collate equally in input order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sort() (
	if [ $# -ne 0 ]; then command sort "$@"; exit; fi
	awk '{ printf "%d\t%s\n", NR, $0 }' | command sort -t "$(printf '\t')" -k 2 -k 1,1n | sed 's/^[0-9]*	//'
)
