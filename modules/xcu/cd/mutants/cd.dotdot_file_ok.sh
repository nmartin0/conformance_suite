# dot-dot after a regular file is removed textually and succeeds
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in */..) pc_m_d=${1%/..}
	if [ -f "$pc_m_d" ]; then case $pc_m_d in */*) pc_m_d=${pc_m_d%/*};; *) pc_m_d=.;; esac
		command cd "$pc_m_d"; return $?; fi;; esac
	command cd "$@"; }
