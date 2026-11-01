# Prelude text for the faults and variants of "set" with no arguments.
# pcm_list replaces the line "set >FILE" of a test script; PCM_MODE says
# how the listing is produced.
pcm_q() {
	printf "'"
	printf '%s' "$1" | sed "s/'/'\\\\''/g"
	printf "'"
}
pcm_names() {
	set | sed -n 's/^\([A-Za-z_][A-Za-z_0-9]*\)=.*/\1/p'
}
pcm_each() {
	while read -r pcm_n; do
		eval "pcm_v=\${$pcm_n}"
		case $1 in
		sq) printf '%s=%s\n' "$pcm_n" "$(pcm_q "$pcm_v")" ;;
		raw) printf '%s=%s\n' "$pcm_n" "$pcm_v" ;;
		naive_sq) printf "%s='%s'\n" "$pcm_n" "$pcm_v" ;;
		naive_dq) printf '%s="%s"\n' "$pcm_n" "$pcm_v" ;;
		dq) printf '%s="%s"\n' "$pcm_n" "$(printf '%s' "$pcm_v" | sed 's/[\\"$`]/\\&/g')" ;;
		bare)
			case $pcm_v in
			'' | *[!A-Za-z0-9_./-]*) printf '%s=%s\n' "$pcm_n" "$(pcm_q "$pcm_v")" ;;
			*) printf '%s=%s\n' "$pcm_n" "$pcm_v" ;;
			esac
			;;
		esac
	done
}
pcm_list() {
	case $PCM_MODE in
	reverse) pcm_names | sed '1!G;h;$!d' | pcm_each sq ;;
	casefold) pcm_names | LC_ALL=C sort -f | pcm_each sq ;;
	unquoted) pcm_names | pcm_each raw ;;
	naive_sq) pcm_names | pcm_each naive_sq ;;
	naive_dq) pcm_names | pcm_each naive_dq ;;
	exported) env | LC_ALL=C sort ;;
	stderr) set >&2 ;;
	spaced) pcm_names | pcm_each sq | sed 's/=/ = /' ;;
	nothing) : ;;
	sq) pcm_names | pcm_each sq ;;
	dq) pcm_names | pcm_each dq ;;
	bare) pcm_names | pcm_each bare ;;
	esac
}
