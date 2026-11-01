# a conforming cd that also exports OLDPWD
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because it is unspecified whether OLDPWD is exported when cd sets it (XCU cd DESCRIPTION).
cd() {
	command cd "$@"
	pc_rc=$?
	if [ "$pc_rc" -eq 0 ]; then export OLDPWD; fi
	return $pc_rc
}
