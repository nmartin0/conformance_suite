# a conforming uudecode that leaves the permission bits of an existing
# output file as they were
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because for an existing file the mode bits are set only "if
# possible" and a failure to do so is not an error (XCU uudecode,
# DESCRIPTION, paragraph on existing files).
uudecode() {
	pc_kf=
	pc_ko=
	pc_kprev=
	for pc_ka; do
		if [ "$pc_kprev" = -o ]; then pc_ko=$pc_ka; fi
		pc_kprev=$pc_ka
	done
	if [ -z "$pc_ko" ]; then
		for pc_ka; do pc_kin=$pc_ka; done
		pc_ko=$(awk '/^begin(-base64)? [0-7]+ ./ { n = $0; sub(/^[^ ]+ [^ ]+ /, "", n); print n; exit }' "$pc_kin" 2>/dev/null)
	fi
	if [ -n "$pc_ko" ] && [ -f "$pc_ko" ]; then
		pc_km=$(ls -l "$pc_ko" | cut -c1-10)
		command uudecode "$@"
		pc_rc=$?
		# put the old bits back
		pc_ku=$(printf '%s' "$pc_km" | cut -c2-4 | sed 's/-//g;s/^/u=/')
		pc_kg=$(printf '%s' "$pc_km" | cut -c5-7 | sed 's/-//g;s/^/g=/')
		pc_ko2=$(printf '%s' "$pc_km" | cut -c8-10 | sed 's/-//g;s/^/o=/')
		chmod "$pc_ku,$pc_kg,$pc_ko2" "$pc_ko" 2>/dev/null
		return $pc_rc
	fi
	command uudecode "$@"
}
