# a conforming system where directories made in the test tree carry set-group-ID
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text leaves the meaning of the set-group-ID bit on
# directories to the implementation (XCU chmod APPLICATION USAGE, XSH mkdir()).
command find . ! -name . -type d -exec chmod g+s {} + >/dev/null 2>&1
