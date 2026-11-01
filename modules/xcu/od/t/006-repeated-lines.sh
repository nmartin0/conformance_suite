# Without -v od replaces groups of lines identical to the one before by a line of only an asterisk; -v writes everything.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.006
# MUTANT    od.star_never
# MUTANT    od.v_ignored
# MUTANT    od.star_first_replaced
# MUTANT    od.star_cmp_first
# MUTANT    od.star_first_token
# MUTANT    od.star_blank
# VARIANT   od.block8
# VARIANT   od.block32
# VARIANT   od.off_plain
# VARIANT   od.off_wide
# VARIANT   od.gap3
# VARIANT   od.trail_blank
. "$PC_CORE/pc.sh"
pc_begin XCU.od.006
LC_ALL=POSIX
export LC_ALL
NL='
'
B=" $(printf '\t')"
a64=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
data=
i=0
while [ $i -lt 16 ]; do
	data=$data$a64
	i=$((i + 1))
done
printf '%s' "$data" >f1024 || pc_fail "cannot create input"
# regions of equal bytes, one odd byte inside a region, a region repeated later
awk 'BEGIN { for (i = 0; i < 400; i++) printf "a"; printf "z"; for (i = 0; i < 400; i++) printf "a"; for (i = 0; i < 400; i++) printf "b"; for (i = 0; i < 400; i++) printf "a"; printf "xyz" }' >fmix || pc_fail "cannot create input"
awk 'BEGIN { for (i = 0; i < 400; i++) printf " 61"; printf " 7a"; for (i = 0; i < 400; i++) printf " 61"; for (i = 0; i < 400; i++) printf " 62"; for (i = 0; i < 400; i++) printf " 61"; printf " 78 79 7a\n" }' >fmix.want || pc_fail "cannot create input"
pc_test
od -A d -t x1 f1024 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od gave status $rc"
star=no
n=0
while IFS= read -r line; do
	n=$((n + 1))
	[ "$line" = '*' ] && star=yes
	last=$line
done <out
[ "$star" = yes ] || pc_fail "no line with only an asterisk for 1024 identical bytes"
set -- $last
[ $# -eq 1 ] || pc_fail "the last line is not a single offset"
z=${1%%[!0]*}
off=${1#"$z"}
[ "$off" = 1024 ] || pc_fail "the last offset is not 1024"
od -A d -v -t x1 f1024 >out 2>/dev/null </dev/null
total=0
star=no
while IFS= read -r line; do
	[ "$line" = '*' ] && star=yes
	set -- $line
	[ $# -gt 1 ] && total=$((total + $# - 1))
done <out
[ "$star" = no ] || pc_fail "-v still wrote an asterisk line"
[ "$total" -eq 1024 ] || pc_fail "-v did not write all 1024 bytes"
# expanding every asterisk line back into the lines it stands for must
# give the whole input again, and no line may be dropped otherwise
od -A d -t x1 fmix >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od on mixed data gave status $rc"
awk '
	$0 == "*" { star = 1; next }
	{
		z = $1; sub(/^0+/, "", z); if (z == "") z = 0
		o = z + 0
		if (NF == 1) {
			final = o
			if (star) {
				k = o - pos
				if (prevn <= 0 || k <= 0 || k % prevn != 0) { print "BAD final"; exit }
				for (c = 0; c < k / prevn; c++) data = data prev
				pos = o
			}
			if (o != pos) { print "BAD end"; exit }
			done = 1
			next
		}
		if (done) { print "BAD after final"; exit }
		if (star) {
			k = o - pos
			if (prevn <= 0 || k <= 0 || k % prevn != 0) { print "BAD gap"; exit }
			for (c = 0; c < k / prevn; c++) data = data prev
			pos = o
			star = 0
		}
		if (o != pos) { print "BAD offset"; exit }
		line = ""
		for (i = 2; i <= NF; i++) line = line " " $i
		data = data line
		prev = line
		prevn = NF - 1
		pos += prevn
	}
	END { if (!done) print "BAD no final"; else printf "%s\n", data }' out >got.exp 2>/dev/null
cmp -s got.exp fmix.want || pc_fail "the lines with asterisks do not stand for exactly the repeated lines"
n=0
stars=0
while IFS= read -r line; do
	n=$((n + 1))
	[ "$line" = '*' ] && stars=$((stars + 1))
done <out
[ "$stars" -ge 1 ] || pc_fail "repeated lines were not replaced by an asterisk line"
pc_pass
