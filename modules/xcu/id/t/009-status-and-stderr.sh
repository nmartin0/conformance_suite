# Exit status 0 and a quiet standard error for the usual forms.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.009
. "$PC_CORE/pc.sh"
pc_begin XCU.id.009
LC_ALL=C; export LC_ALL
# Names for IDs that cannot be mapped are not specified for -n; judge only
# when every ID of the default output has a name.
allnamed=yes
for w in $(id | tr ',' ' '); do
	case $w in
	uid=* | euid=* | gid=* | egid=* | groups=* | [0-9]*) case $w in *\(*) ;; *) allnamed=no ;; esac ;;
	esac
done
pc_test
for o in "" "-u" "-g" "-G" "-un" "-gn" "-Gn" "-ur" "-gr" "-urn" "-grn"; do
	case $o in
	*n) [ "$allnamed" = yes ] || continue ;;
	esac
	id $o >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "id $o exited with status $rc"
	[ -s err ] && pc_fail "id $o wrote to standard error"
	[ -s out ] || pc_fail "id $o wrote nothing"
done
pc_pass
