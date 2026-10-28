# The case word and the patterns are expanded (tilde, parameter, command and
# arithmetic substitution, quote removal); an expanded pattern keeps its
# special characters unless quoted; no pattern after the first match is
# expanded.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.008
HOME=/pc/h
pc_test
pcx='a*'
case abc in $pcx) r=match ;; *) r=no ;; esac
[ "$r" = match ] || pc_fail "unquoted pattern from a variable: '$r'"
case abc in "$pcx") r=match ;; *) r=no ;; esac
[ "$r" = no ] || pc_fail "quoted pattern from a variable matched wildcards: '$r'"
case 'a*' in "$pcx") r=match ;; *) r=no ;; esac
[ "$r" = match ] || pc_fail "quoted pattern did not match itself: '$r'"
case /pc/h in ~) r=match ;; *) r=no ;; esac
[ "$r" = match ] || pc_fail "tilde in a pattern: '$r'"
case x in $(printf x)) r=match ;; *) r=no ;; esac
[ "$r" = match ] || pc_fail "substitution in a pattern: '$r'"
case $(printf ab)c in abc) r=match ;; *) r=no ;; esac
[ "$r" = match ] || pc_fail "substitution in the word: '$r'"
case $((1 + 1)) in 2) r=match ;; *) r=no ;; esac
[ "$r" = match ] || pc_fail "arithmetic in the word: '$r'"
case a in a) : ;; $(: >pc.pm)) : ;; esac
[ -e pc.pm ] && pc_fail "a pattern after the matching clause was expanded"
pc_pass
