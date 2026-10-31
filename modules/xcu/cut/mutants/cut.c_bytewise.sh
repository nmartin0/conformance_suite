# cut -c selects bytes, not characters of the locale
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cut() {
	case $1 in
	-c*) o=-b${1#-c}; shift; command cut "$o" "$@" ;;
	*) command cut "$@" ;;
	esac
}
