# the last variable gets one field instead of the rest of the line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A shell function shadows the built-in; "command read" reaches the real one.
pcm_nl="
"
pcm_tab=$(printf "\t")
pcm_real() {
	if [ -n "$pcm_hasd" ]; then command read $pcm_r -d "$pcm_d" "$@"
	else command read $pcm_r "$@"; fi
}
pcm_ifs_save() { pcm_isset=${IFS+x}; pcm_iv=${IFS-}; }
pcm_ifs_back() { if [ -n "$pcm_isset" ]; then IFS=$pcm_iv; else unset IFS; fi; }
pcm_rawline() {
	pcm_ifs_save; IFS=
	command read -r pcm_line; pcm_rrc=$?
	pcm_ifs_back
	return "$pcm_rrc"
}
pcm_refeed() {
	pcm_hasd= pcm_real "$@" <<PCM_EOF
$pcm_line
PCM_EOF
}
# pcm_via SED VAR...: take one raw line, edit it with sed, hand it to read.
pcm_via() {
	pcm_sed=$1; shift
	pcm_rawline
	if [ -n "$pcm_sed" ]; then pcm_line=$(printf "%s\n" "$pcm_line" | sed "$pcm_sed"); fi
	pcm_finish "$@"
}
pcm_finish() {
	pcm_refeed "$@"; pcm_rc=$?
	[ "$pcm_rrc" -ne 0 ] && return "$pcm_rrc"
	return "$pcm_rc"
}
read() {
	pcm_r= pcm_hasd= pcm_d=
	while [ $# -gt 0 ]; do
		case $1 in
		-r) pcm_r=-r; shift ;;
		-d) pcm_hasd=1; pcm_d=$2; shift 2 ;;
		*) break ;;
		esac
	done
	pcm_real "$@"; pcm_rc=$?
	for pcm_last; do :; done
	eval "pcm_v=\${$pcm_last}"
	pcm_f=${IFS-" $pcm_tab$pcm_nl"}
	while [ -n "$pcm_f" ]; do
		pcm_c=${pcm_f%"${pcm_f#?}"}; pcm_f=${pcm_f#?}
		pcm_v=${pcm_v%%"$pcm_c"*}
	done
	eval "$pcm_last=\$pcm_v"
	return "$pcm_rc"
}
