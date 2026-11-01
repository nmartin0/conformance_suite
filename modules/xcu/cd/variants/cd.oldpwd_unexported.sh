# a conforming cd that keeps OLDPWD out of the environment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because it is unspecified whether OLDPWD is exported when cd sets it (XCU cd DESCRIPTION).
cd() {
	command cd "$@"
	pc_rc=$?
	if [ "$pc_rc" -eq 0 ]; then
		pc_ov=${OLDPWD-}
		unset OLDPWD
		OLDPWD=$pc_ov
	fi
	return $pc_rc
}
