# delta and file data keywords
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.002
# The keywords :I: :R: :L: :B: :S: :DT: :Li: :Ld: :Lu: :DS: :DP: :M: :F: :Z:
# :W: :D: :T: and their parts give the stored values; \t and \n in the data
# specification are a tab and a newline.
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
prs -r1.1 -d':I:|:R:|:L:|:B:|:S:|:DT:|:Li:|:Ld:|:Lu:|:DS:|:DP:' s.f </dev/null >out 2>err || pc_fail "prs -d: nonzero status"
awk -F'|' '{ ok = ($1 == "1.1" && $2 + 0 == 1 && $3 + 0 == 1 && $4 + 0 == 0 && $5 + 0 == 0 && $6 == "D" && $7 + 0 == 3 && $8 + 0 == 0 && $9 + 0 == 0 && $10 ~ /^[0-9]+$/ && $11 ~ /^[0-9]+$/) } END { exit !ok }' out || pc_fail "numeric keywords read back as: $(cat out)"
prs -r1.1 -d':M:|:F:|:Z:' s.f </dev/null >out 2>err || pc_fail "prs -d (file keywords): nonzero status"
[ "$(cat out)" = 'f|s.f|@(#)' ] || pc_fail ":M: :F: :Z: gave '$(cat out)'"
prs -r1.1 -d':W:' s.f </dev/null >out 2>err || pc_fail "prs -d :W:: nonzero status"
case $(cat out) in
'@(#)f'?'1.1') ;;
*) pc_fail ":W: gave '$(cat out)'" ;;
esac
prs -r1.1 -d':D:|:T:|:Dy:|:Dm:|:Dd:|:Th:|:Tm:|:Ts:' s.f </dev/null >out 2>err || pc_fail "prs -d (date keywords): nonzero status"
awk -F'|' '{ ok = ($1 ~ /^[0-9][0-9]\/[0-9][0-9]\/[0-9][0-9]$/ && $2 ~ /^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]$/ && $1 == $3 "/" $4 "/" $5 && $2 == $6 ":" $7 ":" $8) } END { exit !ok }' out || pc_fail "date and time keywords are inconsistent: $(cat out)"
prs -r1.1 -d':I:\t:I:\n' s.f </dev/null >out 2>err || pc_fail "prs -d with \\t and \\n: nonzero status"
printf '1.1\t1.1\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "\\t and \\n were not converted: $(od -c out | head -2)"
pc_pass
