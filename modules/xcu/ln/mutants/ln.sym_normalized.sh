# ln -s removes dot components from the stored text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -s ] && [ $# -eq 3 ]; then
		pc_t=$(printf '%s\n' "$2" | sed 's|/\./|/|g')
		command ln -s "$pc_t" "$3"; return
	fi
	command ln "$@"
}
