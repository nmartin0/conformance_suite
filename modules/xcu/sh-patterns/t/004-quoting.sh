# A quoted or backslash-escaped pattern character matches only itself; a
# pattern that comes from an expansion keeps its special characters unless
# the expansion was quoted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.004
pc_test
case 'a*' in a"*") r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "a\"*\" did not match a*"
case abc in a"*") r=yes ;; *) r=no ;; esac
[ "$r" = no ] || pc_fail "a\"*\" matched abc"
case 'a?' in a\?) r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "a\\? did not match a?"
case ab in a\?) r=yes ;; *) r=no ;; esac
[ "$r" = no ] || pc_fail "a\\? matched ab"
case '[a]' in "[a]") r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "quoted [a] did not match itself"
case a in "[a]") r=yes ;; *) r=no ;; esac
[ "$r" = no ] || pc_fail "quoted [a] matched a"
case 'a\b' in 'a\b') r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "quoted backslash did not match itself"
case 'a\b' in 'a\\b') r=yes ;; *) r=no ;; esac
[ "$r" = no ] || pc_fail "single-quoted double backslash matched a single backslash"
case 'a\b' in a\\b) r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "an escaped backslash (a\\\\b) did not match a\\b"
pcq='*'
case abc in $pcq) r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "unquoted expansion of * did not act as a wildcard"
case abc in "$pcq") r=yes ;; *) r=no ;; esac
[ "$r" = no ] || pc_fail "quoted expansion of * acted as a wildcard"
case '*' in "$pcq") r=yes ;; *) r=no ;; esac
[ "$r" = yes ] || pc_fail "quoted expansion of * did not match *"
pc_pass
