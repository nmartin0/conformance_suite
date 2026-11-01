# Stands in for the -ok primary of find: asks on standard error, reads one
# line from standard input and runs the utility when the line is affirmative.
# Used by the find faults and variants; MODE chooses the one thing it does wrong or differently.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mode=$1
shift
case $mode in
prompt_noq) printf '%s ' "$*" >&2 ;;
prompt_nopath) printf '%s ? ' "$1" >&2 ;;
prompt_noutil) shift; printf 'file %s ? ' "$*" >&2 ;;
prompt_stdout) printf '< %s > ? ' "$*" ;;
prompt_nl) printf '< %s > ?\n' "$*" >&2 ;;
prompt_trail) printf 'run %s now?   ' "$*" >&2 ;;
prompt_pathfirst) pcf_l=$(for a; do :; done; printf %s "$a"); printf '%s: %s (y/n)? ' "$pcf_l" "$1" >&2 ;;
*) printf '< %s ... > ? ' "$*" >&2 ;;
esac
ans=
case $mode in
reply_onechar) ans=$(dd bs=1 count=1 2>/dev/null) ;;
*) IFS= read -r ans || ans= ;;
esac
ok=
case $mode in
reply_lower) case $ans in y*) ok=1 ;; esac ;;
reply_exact) [ "$ans" = y ] && ok=1 ;;
reply_onechar) case $ans in [yY]*) ok=1 ;; esac ;;
inverted) case $ans in [yY]*) ;; *) ok=1 ;; esac ;;
always) ok=1 ;;
*) case $ans in [yY]*) ok=1 ;; esac ;;
esac
if [ -z "$ok" ]; then
	[ "$mode" = declined_true ] && exit 0
	exit 1
fi
exec "$@"
