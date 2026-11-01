# with -P a trailing dot-dot is removed textually
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = -P ]; then
	case $2 in */..) pc_m_d=${2%/..}
		case $pc_m_d in */*) pc_m_d=${pc_m_d%/*};; *) pc_m_d=.;; esac
		command cd -P "$pc_m_d"; return $?;; esac
	fi; command cd "$@"; }
