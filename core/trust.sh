# trust.sh - known-answer checks of the shell features posixcheck relies on.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The driver sources this file (with PCT_SOURCED=yes) and calls
# pc_trust_run DIR, which prints the name of the first failed check and
# returns 1, or prints nothing and returns 0. Run directly as
#     sh trust.sh DIR
# it checks the shell that runs it; the driver does this with the shell
# the tests will use.
#
# Each check runs in a subshell so one failure cannot disturb the next.
# DIR must be an existing, empty, writable directory.

pct_printf() {
	[ "$(printf '%s|%d|%3s|%-2s|' a 42 b c)" = 'a|42|  b|c |' ] || return 1
	[ "$(printf '%s\n' x y)" = "$(printf 'x\ny')" ] || return 1
	t=$(printf '\t')
	[ "${#t}" -eq 1 ] || return 1
	[ "$(printf '%%')" = '%' ]
}

pct_test() {
	[ a = a ] && [ a != b ] && [ -n a ] && [ -z "" ] || return 1
	[ 3 -lt 10 ] && [ 10 -gt 3 ] && [ 2 -eq 2 ] && [ 2 -ne 3 ] || return 1
	[ 2 -le 2 ] && [ 2 -ge 2 ] || return 1
	! [ a = b ] || return 1
	[ -d / ] || return 1
	! [ -e "$pct_dir/no-such-file" ] || return 1
	: >"$pct_dir/empty"
	printf 'x' >"$pct_dir/full"
	[ -f "$pct_dir/empty" ] && ! [ -s "$pct_dir/empty" ] && [ -s "$pct_dir/full" ]
}

pct_case() {
	case abc in a*c) ;; *) return 1 ;; esac
	case x.c in *.sh) return 1 ;; *.c) ;; *) return 1 ;; esac
	case "a b" in "a b") ;; *) return 1 ;; esac
	case X in [A-Z]) ;; *) return 1 ;; esac
	case x in [!a-w]) ;; *) return 1 ;; esac
	case 'a|b' in *'|'*) ;; *) return 1 ;; esac
	case '' in '') ;; *) return 1 ;; esac
}

pct_params() {
	v=a/b/c.d
	[ "${v%/*}" = a/b ] && [ "${v%%/*}" = a ] || return 1
	[ "${v#*/}" = b/c.d ] && [ "${v##*/}" = c.d ] || return 1
	[ "${#v}" -eq 7 ] || return 1
	unset w
	[ "${w:-d}" = d ] && [ -z "${w:-}" ] || return 1
	w=
	[ "${w:+x}" = "" ] || return 1
	w=z
	[ "${w:+x}" = x ] || return 1
	p='  lead'
	[ "${p#"${p%%[! ]*}"}" = lead ]
}

pct_arith() {
	[ $((3 + 4 * 2)) -eq 11 ] && [ $(((7 - 2) / 2)) -eq 2 ] || return 1
	x=5
	x=$((x + 1))
	[ "$x" -eq 6 ]
}

pct_status() {
	(exit 3)
	[ $? -eq 3 ] || return 1
	false | true
	[ $? -eq 0 ] || return 1
	true | false
	[ $? -ne 0 ] || return 1
	pct_f() { return 4; }
	pct_f
	[ $? -eq 4 ] || return 1
	! true && return 1
	return 0
}

pct_loop() {
	n=0
	while [ "$n" -lt 5 ]; do n=$((n + 1)); done
	[ "$n" -eq 5 ] || return 1
	s=
	for i in a b c; do
		[ "$i" = b ] && continue
		s=$s$i
	done
	[ "$s" = ac ] || return 1
	for i in 1 2 3; do [ "$i" = 2 ] && break; done
	[ "$i" = 2 ]
}

pct_subst() {
	[ "$(printf 'a\n\n')" = a ] || return 1
	[ "$(printf '%s' "$(printf inner)")" = inner ]
}

pct_read() {
	printf 'k  v1 v2\n' >"$pct_dir/r1"
	read -r a b <"$pct_dir/r1"
	[ "$a" = k ] && [ "$b" = "v1 v2" ] || return 1
	printf 'a\\b\n' >"$pct_dir/r2"
	read -r x <"$pct_dir/r2"
	[ "$x" = 'a\b' ] || return 1
	printf '  x\n' >"$pct_dir/r3"
	IFS= read -r y <"$pct_dir/r3"
	[ "$y" = '  x' ] || return 1
	printf 'one\ntwo\nthree\n' >"$pct_dir/r4"
	n=0
	while IFS= read -r l; do n=$((n + 1)); done <"$pct_dir/r4"
	[ "$n" -eq 3 ]
}

pct_redir() {
	printf 'a\n' >"$pct_dir/w"
	printf 'b\n' >>"$pct_dir/w"
	{ read -r p; read -r q; } <"$pct_dir/w"
	[ "$p$q" = ab ] || return 1
	{ printf 'e\n' >&2; } 2>"$pct_dir/e"
	read -r e <"$pct_dir/e"
	[ "$e" = e ] || return 1
	printf 'x\n' >/dev/null
}

pct_trap() {
	[ "$( (trap 'printf done' EXIT; :) )" = done ]
}

pct_eval() {
	eval "pct_v=7"
	[ "$pct_v" = 7 ] || return 1
	n=v
	eval "pct_w=\${pct_$n}"
	[ "$pct_w" = 7 ]
}

pct_noglob() {
	set -f
	v='*'
	set -- $v
	set +f
	[ "$1" = '*' ]
}

pct_ifs() {
	old=$IFS
	IFS=:
	set -- a:b::c
	set -- $1
	IFS=$old
	[ "$#" -eq 4 ] && [ "$3" = "" ] && [ "$4" = c ]
}

pct_glob() {
	mkdir "$pct_dir/g" || return 1
	: >"$pct_dir/g/b"
	: >"$pct_dir/g/a"
	: >"$pct_dir/g/c"
	set -- "$pct_dir"/g/*
	[ "$#" -eq 3 ] && [ "${1##*/}${2##*/}${3##*/}" = abc ] || return 1
	set -- "$pct_dir"/g/zz*
	[ "$1" = "$pct_dir/g/zz*" ]
}

pct_mkdir() {
	mkdir -p "$pct_dir/m/n" && [ -d "$pct_dir/m/n" ] || return 1
	! mkdir "$pct_dir/m" 2>/dev/null
}

pct_command() {
	command -v printf >/dev/null || return 1
	! command -v pct_no_such_command_ >/dev/null 2>&1 || return 1
	printf() { return 9; }
	command printf '' || return 1
	unset -f printf
}

pc_trust_run() {
	pct_dir=$1
	for pct_c in printf test case params arith status loop subst read \
	    redir trap eval noglob ifs glob mkdir command; do
		if ("pct_$pct_c") >/dev/null 2>&1; then
			:
		else
			printf '%s\n' "$pct_c"
			return 1
		fi
	done
	return 0
}

case ${PCT_SOURCED:-} in
yes) ;;
*)
	pc_trust_run "${1:?usage: sh trust.sh DIR}"
	exit $?
	;;
esac
