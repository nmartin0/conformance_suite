# Shared by the set faults and variants (not a fault itself).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# The tests run each script they write under ${PC_SH:-sh} script. The
# caller sets some of these variables, then sources this file; it plants
# a small wrapper as the shell that runs those scripts. The wrapper hands
# the real shell a rewritten script, so the system under test behaves as
# the faulty (or the other conforming) system would.
#   PCM_PRE   text placed before the script
#   PCM_SED   sed program applied to the script text
#   PCM_OSED  sed program applied to the script's standard output
#   PCM_RC    every non-zero exit status becomes this number
#   PCM_RCZ   exit status zero becomes this number
#   PCM_ERR   hide: standard error is discarded; out: it joins standard output
# A fault or variant file sources this one as
#   . "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
PCM_REAL=$PC_SH
: "${PCM_PRE=}" "${PCM_SED=}" "${PCM_OSED=}" "${PCM_RC=}" "${PCM_RCZ=}" "${PCM_ERR=}"
export PCM_REAL PCM_PRE PCM_SED PCM_OSED PCM_RC PCM_RCZ PCM_ERR
cat >pcm_w.sh <<'PCM_EOF'
pcm_f=$1
shift
pcm_x=$pcm_f.pcm
{
	printf '%s\n' "$PCM_PRE"
	sed -e "$PCM_SED" "$pcm_f"
} >"$pcm_x"
"$PCM_REAL" "$pcm_x" "$@" >"$pcm_x.o" 2>"$pcm_x.e"
pcm_rc=$?
if [ -n "$PCM_OSED" ]; then sed -e "$PCM_OSED" "$pcm_x.o"; else cat "$pcm_x.o"; fi
case $PCM_ERR in
hide) ;;
out) cat "$pcm_x.e" ;;
*) cat "$pcm_x.e" >&2 ;;
esac
rm -f "$pcm_x" "$pcm_x.o" "$pcm_x.e"
if [ "$pcm_rc" -ne 0 ]; then
	[ -z "$PCM_RC" ] || pcm_rc=$PCM_RC
elif [ -n "$PCM_RCZ" ]; then
	pcm_rc=$PCM_RCZ
fi
exit "$pcm_rc"
PCM_EOF
PC_SH="$PCM_REAL $PWD/pcm_w.sh"
export PC_SH
