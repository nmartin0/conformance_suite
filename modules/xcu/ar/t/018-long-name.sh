# A long member name is not truncated
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.018
# A long member name is not truncated: the archive lists the full
# name and the member extracts under it.
# MUTANT    ar.trunc15
# MUTANT    ar.trunc16
# MUTANT    ar.trunc31
# MUTANT    ar.trunc32
# MUTANT    ar.trunc64
# MUTANT    ar.trunc100
# MUTANT    ar.trunc200
# MUTANT    ar.trunc254
# MUTANT    ar.x_trunc16
# MUTANT    ar.t_trunc16
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
base=0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
# Names of several lengths, up to the longest the file system allows (at
# most 255 here); each is archived, listed and extracted whole.
max=$(getconf NAME_MAX . 2>/dev/null) || max=
case $max in ''|*[!0-9]*) max=255 ;; esac
[ "$max" -gt 255 ] && max=255
all=
for len in 15 16 17 31 32 33 63 64 65 100 127 128 190 200 "$max"; do
	[ "$len" -le "$max" ] || continue
	name=$(printf '%s%s%s%s%s' "$base" "$base" "$base" "$base" "$base" | cut -c1-$((len - 4)))
	name=$name.txt
	all="$all $name"
done
for name in $all; do
	printf 'content of %s\n' "${#name}" >"$name" 2>/dev/null || pc_notinuse "the file system does not allow a name of ${#name} bytes"
done
pc_test
for name in $all; do
	ar -rc a "$name" >out 2>err </dev/null || pc_fail "ar -rc of a ${#name}-byte name: nonzero status"
done
ar -t a >out 2>err </dev/null || pc_fail "ar -t: nonzero status"
for name in $all; do printf '%s\n' "$name"; done >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the listing does not show the full names: $(cat out)"
ar -t a $all >out 2>err </dev/null || pc_fail "ar -t with the long names as operands: nonzero status"
[ "$(wc -l <out)" -eq "$(printf '%s\n' $all | wc -l)" ] || pc_fail "ar -t with the long names as operands did not list them all"
for name in $all; do
	ar -t a "$name" >out 2>err </dev/null || pc_fail "ar -t a ${#name}-byte name: nonzero status"
	printf '%s\n' "$name" >exp
	cmp out exp >/dev/null 2>&1 || pc_fail "ar -t of a ${#name}-byte name lists '$(cat out)'"
done
rm -f $all
ar -x a >out 2>err </dev/null || pc_fail "ar -x: nonzero status"
for name in $all; do
	[ "$(cat "$name" 2>/dev/null)" = "content of ${#name}" ] || pc_fail "the ${#name}-byte member did not extract under its full name"
done
rm -f $all
ar -x a $all >out 2>err </dev/null || pc_fail "ar -x with long names as operands: nonzero status"
for name in $all; do
	[ "$(cat "$name" 2>/dev/null)" = "content of ${#name}" ] || pc_fail "the ${#name}-byte member did not extract by name"
done
pc_pass
