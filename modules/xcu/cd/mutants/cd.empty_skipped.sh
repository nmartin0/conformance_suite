# empty CDPATH entries are ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_r=; pc_m_l=${CDPATH-}:
	while [ -n "$pc_m_l" ]; do pc_m_e=${pc_m_l%%:*}; pc_m_l=${pc_m_l#*:}
		if [ -n "$pc_m_e" ]; then pc_m_r=$pc_m_r${pc_m_r:+:}$pc_m_e; fi; done
	CDPATH=$pc_m_r command cd "$@"; }
