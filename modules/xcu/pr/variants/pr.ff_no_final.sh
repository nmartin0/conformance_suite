# pr: with -F or -f no form-feed follows the last page (allowed: a form-feed separates pages, so one after the last is not required)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pr() {
	case " $* " in *" -F "* | *" -f "*) ;; *) command pr "$@"; return ;; esac
	command pr "$@" | awk '{ if (held) print prev; prev = $0; held = 1 } END { if (held && prev != "\f") print prev }'
}
