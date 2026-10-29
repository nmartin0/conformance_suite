# Known deviations

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Records of what the suite reported on one system. They are observations of
that system, never the definition of correct. The checks were not changed to
fit them.

System of the first run: Ubuntu 24.04 (glibc 2.39), kernel 6.18, in a minimal
container; root and an ordinary user (`nobody`) gave the same verdicts.

| Assertion | Result | Triage | Observation |
|-----------|--------|--------|-------------|
| CONV.utf8.004 | FAIL | SYSTEM | In the `C.utf8` locale `mbrtowc()` decodes `F4 90 80 80` (U+110000), and the five- and six-octet forms up to `FD BF BF BF BF BF` (U+7FFFFFFF); RFC 3629 limits UTF-8 to U+10FFFF and its grammar has no lead byte above F4. Overlong forms, surrogates, truncated and stray bytes are rejected correctly (002, 003, 005, 006 pass). |
| CONV.utf8.008 | FAIL | SYSTEM | `wcrtomb()` encodes U+110000 as a four-byte sequence (and values up to 0x7FFFFFFF in up to six bytes); it refuses the surrogates correctly. Same cause as 004 on the encoding side. |
| CONV.etcdb.013 | INSPECT | SYSTEM | `/etc/protocols` lists `mptcp` as protocol 262. IP protocol numbers are eight bits (registry range 0 to 255); 262 is a number private to the kernel's socket interface, not an IP protocol number. A person decides whether this matters for the file's consumers. |
| CONV.toolchain.008 | INSPECT (alternate linker only) | SYSTEM | With the system's own `ld` (GNU) and with the `gold` linker the check passes. With `ld.lld` (LLVM linker, run through the suite by `PC_CONV_LD=/usr/bin/ld.lld`) an archive named before the object that needs it still satisfies the reference, i.e. lld resolves archive members independently of command-line position, where the GNU manual and the second linker family's manual both say an archive is searched once, where it appears. All other 20 toolchain checks pass with lld (and with the LLVM assembler `llvm-mc` and the clang driver). The linker's own documentation host could not be reached, so whether this is documented there is not known. Not a defect of the default toolchain of this system. |
| CONV.xdg.001, CONV.xdg.002 | NOTINUSE | not a defect | `XDG_RUNTIME_DIR` is not set in the environment of a root shell in the container. With a directory of mode 0700 supplied (`XDG_RUNTIME_DIR=/tmp/xrd-root sh conv/run.sh`) both pass, as root and as `nobody` with its own directory. |

No SOURCES-DISAGREE finding changed a check in this batch: where sources
differ (the random devices, `/dev/stdin`, the resolver directive set) the
assertion already accepts each variant or is NOTINUSE.

Second pass (toolchain batch, same system): CONV.toolchain.001 to .021 all
pass with the system's `cc`, `as`, `ld` and `ar` as root and as `nobody`. The
other non-PASS results are unchanged from the first run. The re-reading of
the etcdb, layout and devnodes assertions against the man7.org pages changed
no result on this system (see STATUS.md for the table of changes).
