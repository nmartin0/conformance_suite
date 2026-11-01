# ln -f removes the destination but makes no link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	for pc_a; do
		if [ "$pc_a" = -f ]; then
			for pc_last; do :; done
			[ -d "$pc_last" ] || rm -f "$pc_last"
			return 0
		fi
	done
	command ln "$@"
}
