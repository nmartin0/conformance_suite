# The numbers of the options agree with the default output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.002
. "$PC_CORE/pc.sh"
pc_begin XCU.id.002
LC_ALL=C; export LC_ALL
pc_test
out=$(id) || pc_fail "id failed"
set -- $out
euid= egid=
case $1 in uid=*) u=${1#uid=} ;; *) pc_fail "first word '$1' is not uid=" ;; esac
case $2 in gid=*) g=${2#gid=} ;; *) pc_fail "second word '$2' is not gid=" ;; esac
shift 2
for w in "$@"; do
	case $w in
	euid=*) euid=${w#euid=} ;;
	egid=*) egid=${w#egid=} ;;
	esac
done
u=${u%%\(*}
g=${g%%\(*}
euid=${euid%%\(*}
egid=${egid%%\(*}
[ -n "$euid" ] || euid=$u
[ -n "$egid" ] || egid=$g
ru=$(id -ur) || pc_fail "id -ur failed"
rg=$(id -gr) || pc_fail "id -gr failed"
eu=$(id -u) || pc_fail "id -u failed"
eg=$(id -g) || pc_fail "id -g failed"
[ "$ru" = "$u" ] || pc_fail "id -ur is '$ru', uid= is '$u'"
[ "$rg" = "$g" ] || pc_fail "id -gr is '$rg', gid= is '$g'"
[ "$eu" = "$euid" ] || pc_fail "id -u is '$eu', the effective user ID in the default output is '$euid'"
[ "$eg" = "$egid" ] || pc_fail "id -g is '$eg', the effective group ID in the default output is '$egid'"
pc_pass
