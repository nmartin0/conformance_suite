# Prelude text for the faults and variants of "set -o" and "set +o".
# pcm_so replaces the line "set -o >FILE ..." and pcm_po replaces "set +o"
# in a test script; PCM_MODE says what they write.
pcm_so() {
	case $PCM_MODE in
	so_nothing) : ;;
	so_stderr) set -o >&2 ;;
	so_both) set -o; set -o >&2 ;;
	so_static) set -o | sed 's/[[:space:]]on$/ off/' ;;
	so_on_only) set -o | grep '[[:space:]]on$' ;;
	so_tab) set -o | sed 's/[[:space:]][[:space:]]*/	/' ;;
	*) set -o ;;
	esac
}
pcm_po() {
	case $PCM_MODE in
	po_nothing) : ;;
	po_stderr) set +o >&2 ;;
	po_both) set +o; set +o >&2 ;;
	po_only_on) set +o | sed -n '/^set -o/p' ;;
	po_all_off) set +o | sed 's/^set -o/set +o/' ;;
	po_all_on) set +o | sed 's/^set +o/set -o/' ;;
	po_head) set +o | sed -n 1p ;;
	po_oneline) set +o | tr '\n' ';' ;;
	po_short)
		set +o | sed \
			-e 's/^set \([-+]\)o allexport$/set \1a/' \
			-e 's/^set \([-+]\)o errexit$/set \1e/' \
			-e 's/^set \([-+]\)o noglob$/set \1f/' \
			-e 's/^set \([-+]\)o noexec$/set \1n/' \
			-e 's/^set \([-+]\)o noclobber$/set \1C/' \
			-e 's/^set \([-+]\)o nounset$/set \1u/' \
			-e 's/^set \([-+]\)o verbose$/set \1v/' \
			-e 's/^set \([-+]\)o xtrace$/set \1x/' \
			-e 's/^set \([-+]\)o notify$/set \1b/' \
			-e 's/^set \([-+]\)o monitor$/set \1m/'
		;;
	*) set +o ;;
	esac
}
