# illumos triage, group unres

Counts: TEST-BUG 117, SYSTEM 141, STD-AMBIG 5, ENV 15, UNKNOWN 1.

| ID | result | class | reason |
|---|---|---|---|
| XBD.11.1-interface-characteristics.014 | UNRESOLVED | ENV | the test pty would not accept the terminal mode needed (EINVAL); no verdict on the system |
| XBD.11.1-interface-characteristics.016 | UNRESOLVED | ENV | the test pty would not accept the terminal mode needed (EINVAL); no verdict on the system |
| XBD.11.1-interface-characteristics.045 | UNRESOLVED | ENV | the test pty would not accept the terminal mode needed (EINVAL); no verdict on the system |
| XBD.2-conformance.008 | UNRESOLVED | SYSTEM | c17 is not found though the CD option is claimed (XCU c17); the test needs it to link four names |
| XBD.6.4-charmap-description.001 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.6.4-charmap-description.002 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.6.4-charmap-description.003 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.6.4-charmap-description.004 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.6.4-charmap-description.005 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.6.4-charmap-description.006 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.6.4-charmap-description.007 | UNRESOLVED | SYSTEM | a charmap WIDTH_DEFAULT declaration must be accepted (XBD 6.4); localedef refused the source, though this suite reports any refusal as UNRESOLVED |
| XBD.6.4-charmap-description.008 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.1-locale-general.001 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.1-locale-general.002 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.2-posix-locale.002 | UNRESOLVED | TEST-BUG | a pathname in LC_ALL selects the localedef output (XBD 8.2); inability to select it was UNRESOLVED, now FAIL (underlying SYSTEM defect) |
| XBD.7.3.1-lc-ctype.006 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.1-lc-ctype.007 | UNRESOLVED | SYSTEM | charclass is a recognised LC_CTYPE keyword (XBD 7.3.1); localedef refused it, reported UNRESOLVED by the helper |
| XBD.7.3.1-lc-ctype.008 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.1-lc-ctype.009 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.1-lc-ctype.010 | UNRESOLVED | TEST-BUG | a copy of an existing locale must be accepted; the helper called any refusal UNRESOLVED, now FAIL (the refusal itself is a system defect) |
| XBD.7.3.2-lc-collate.004 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.005 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.006 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.007 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.008 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.009 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.010 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.011 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.012 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.2-lc-collate.013 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.3-lc-monetary.001 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.3-lc-monetary.002 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.3-lc-monetary.003 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.3-lc-monetary.004 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.3-lc-monetary.005 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.4-lc-numeric.001 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.4-lc-numeric.002 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.4-lc-numeric.003 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.4-lc-numeric.004 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.3.5-lc-time.001 | UNRESOLVED | SYSTEM | LC_TIME keywords ab_alt_mon and alt_mon are mandatory-to-recognise (XBD 7.3.5); localedef refused the source |
| XBD.7.3.5-lc-time.002 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.5-lc-time.003 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.5-lc-time.004 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.5-lc-time.005 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.5-lc-time.006 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.5-lc-time.008 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.5-lc-time.009 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.7.3.6-lc-messages.001 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.4-locale-grammar.001 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.4-locale-grammar.002 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.4-locale-grammar.003 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.4-locale-grammar.004 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.4-locale-grammar.005 | UNRESOLVED | TEST-BUG | source padded other categories with copy of the POSIX locale, which the test did not need; now explicit categories |
| XBD.7.4-locale-grammar.006 | UNRESOLVED | TEST-BUG | a copy of an existing locale must be accepted; the helper called any refusal UNRESOLVED, now FAIL (the refusal itself is a system defect) |
| XBD.8.2-internationalization-variables.001 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.8.2-internationalization-variables.002 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.8.2-internationalization-variables.003 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.8.2-internationalization-variables.004 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.8.2-internationalization-variables.006 | UNRESOLVED | TEST-BUG | source needlessly carried alternative month names and copies of the POSIX locale for unrelated categories; now plain explicit categories |
| XBD.dirent.h.001 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.dirent.h.002 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.fcntl.h.011 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.fcntl.h.015 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.ftw.h.002 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.langinfo.h.004 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.pthread.h.012 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.sys_shm.h.003 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XBD.sys_stat.h.009 | UNRESOLVED | SYSTEM | a constant the header section requires is absent; the test correctly defers the missing-name verdict to the header presence check |
| XCU.1.5-large-files.002 | UNRESOLVED | TEST-BUG | large skip/seek by single bytes was too slow; now block-sized (dd may skip by reading) |
| XCU.admin.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.004 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.006 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.007 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.admin.008 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.awk.036 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.awk.037 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.awk.038 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.awk.039 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.c17.006 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.c17.009 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.c17.016 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.cflow.008 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.comm.006 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.compress.010 | UNRESOLVED | SYSTEM | compress -g is required under the claimed XSI option; the call failed |
| XCU.cp.029 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.cp.030 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.cp.031 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.csplit.009 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.csplit.010 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.cut.013 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.date.010 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.delta.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.delta.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.delta.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.delta.004 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.delta.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.delta.006 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.expr.010 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.expr.011 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.expr.012 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.false.005 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.find.034 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.find.035 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.find.036 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.fuser.001 | UNRESOLVED | SYSTEM | fuser (XSI option, claimed) was not found (status 127) |
| XCU.fuser.002 | UNRESOLVED | SYSTEM | fuser (XSI option, claimed) was not found (status 127) |
| XCU.fuser.003 | UNRESOLVED | SYSTEM | fuser (XSI option, claimed) was not found (status 127) |
| XCU.fuser.004 | UNRESOLVED | SYSTEM | fuser (XSI option, claimed) was not found (status 127) |
| XCU.fuser.005 | UNRESOLVED | SYSTEM | fuser (XSI option, claimed) was not found (status 127) |
| XCU.gencat.001 | UNRESOLVED | TEST-BUG | gencat is a base utility but the setup needed c17 by name; the driver now exports the compiler it chose |
| XCU.gencat.002 | UNRESOLVED | TEST-BUG | gencat is a base utility but the setup needed c17 by name; the driver now exports the compiler it chose |
| XCU.gencat.003 | UNRESOLVED | TEST-BUG | gencat is a base utility but the setup needed c17 by name; the driver now exports the compiler it chose |
| XCU.gencat.004 | UNRESOLVED | TEST-BUG | gencat is a base utility but the setup needed c17 by name; the driver now exports the compiler it chose |
| XCU.gencat.005 | UNRESOLVED | TEST-BUG | gencat is a base utility but the setup needed c17 by name; the driver now exports the compiler it chose |
| XCU.get.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.004 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.006 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.007 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.008 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.get.009 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.grep.009 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.grep.010 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.join.011 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.lex.001 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.002 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.003 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.004 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.005 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.006 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.007 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.008 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.lex.009 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.ln.009 | UNRESOLVED | TEST-BUG | ln -P failure was UNRESOLVED; with a control link succeeding, the failure is the utility's (linkat with no flags links the symlink itself) |
| XCU.ls.053 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.ls.054 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.ls.055 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.mv.015 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.mv.016 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.mv.017 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.nice.003 | UNRESOLVED | TEST-BUG | caller niceness of 20 was refused as too high; the legal range is 0 to 2*NZERO-1, so only its true top is skipped |
| XCU.nl.009 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.nl.010 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.nm.001 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.002 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.003 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.004 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.005 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.006 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.007 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.008 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.nm.009 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.paste.009 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.printf.015 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.prs.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.prs.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.prs.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.prs.004 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.prs.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.prs.006 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.rmdel.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.rmdel.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.rmdel.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.rmdel.004 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.rmdel.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.sact.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.sact.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.sact.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.sed.022 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.sed.023 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.sed.024 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.sh-environment.007 | UNRESOLVED | UNKNOWN | the test is a few trivial commands and passes locally; the guest timeout cause is not visible (rerun with a trace on the guest) |
| XCU.sort.017 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.sort.018 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.sort.019 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.sort.020 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.strip.001 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.strip.002 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.strip.004 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.tail.007 | UNRESOLVED | ENV | tail -f output arrived only after the test wait; no time bound is stated, so UNRESOLVED is right |
| XCU.test.015 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.tr.009 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.true.004 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.uncompress.007 | UNRESOLVED | SYSTEM | compress -g is required under the claimed XSI option; the call failed |
| XCU.unget.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.unget.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.unget.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.val.001 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.val.002 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.val.003 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.val.004 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.val.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.val.006 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.wc.010 | UNRESOLVED | TEST-BUG | generated locale padded categories with copies of the POSIX locale and optional alternative-month keywords; now explicit categories |
| XCU.what.005 | UNRESOLVED | SYSTEM | the SCCS utilities are required under the claimed XSI option; get/prs are missing or admin could not create the s-file (error text not captured) |
| XCU.yacc.001 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.002 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.003 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.004 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.005 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.006 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.007 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.008 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.009 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.010 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.011 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.013 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.014 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.015 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.016 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.017 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.018 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.019 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.yacc.020 | UNRESOLVED | SYSTEM | c17 is absent although its option is claimed (and the option for this utility); the required compile step cannot run |
| XCU.zcat.005 | UNRESOLVED | SYSTEM | compress -g is required under the claimed XSI option; the call failed |
| XSH.fwide.005 | UNRESOLVED | STD-AMBIG | fwide() only attempts to orient a stream; a refusal on a fresh stream is not clearly a breach, so UNRESOLVED is right (no disputed needed) |
| XSH.2.5.2-stream-orientation.005 | UNRESOLVED | STD-AMBIG | fwide() only attempts to orient a stream; a refusal on a fresh stream is not clearly a breach, so UNRESOLVED is right (no disputed needed) |
| XSH.2.5.2-stream-orientation.006 | UNRESOLVED | STD-AMBIG | fwide() only attempts to orient a stream; a refusal on a fresh stream is not clearly a breach, so UNRESOLVED is right (no disputed needed) |
| XSH.2.5.2-stream-orientation.007 | UNRESOLVED | STD-AMBIG | fwide() only attempts to orient a stream; a refusal on a fresh stream is not clearly a breach, so UNRESOLVED is right (no disputed needed) |
| XSH.close.007 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.close.009 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.close.010 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.confstr.006 | UNRESOLVED | SYSTEM | _CS_V8_ENV is required in unistd.h and was absent |
| XSH.exec.056 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.016 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.037 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.038 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.039 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.040 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.041 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.042 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fcntl.043 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.fmemopen.003 | UNRESOLVED | TEST-BUG | a read after a rejected write was a hard setup step; only the buffer content is judged now |
| XSH.fpathconf.007 | UNRESOLVED | ENV | the run had no login record or no controlling terminal |
| XSH.freopen.003 | UNRESOLVED | SYSTEM | fgetc() on a stream not open for reading must fail with the error indicator set; the setup showed it did not (test right to defer) |
| XSH.freopen.012 | UNRESOLVED | STD-AMBIG | fwide() only attempts to orient a stream; a refusal on a fresh stream is not clearly a breach, so UNRESOLVED is right (no disputed needed) |
| XSH.getlogin.004 | UNRESOLVED | ENV | the run had no login record or no controlling terminal |
| XSH.getlogin.005 | UNRESOLVED | ENV | the run had no login record or no controlling terminal |
| XSH.gettext.008 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.gettext.009 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.gettext.011 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.gettext.012 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.gettext.013 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.gettext.014 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.gettext.015 | UNRESOLVED | TEST-BUG | the picker took the first locale and gave up when it looked like the POSIX locale; now it tries each candidate |
| XSH.getutxent.001 | UNRESOLVED | TEST-BUG | the page does not forbid extra ut_type values; the type check was removed |
| XSH.iconv.006 | UNRESOLVED | ENV | //IGNORE or //TRANSLIT suffix support is optional and not offered here; UNRESOLVED is right |
| XSH.iconv.007 | UNRESOLVED | ENV | //IGNORE or //TRANSLIT suffix support is optional and not offered here; UNRESOLVED is right |
| XSH.iconv.008 | UNRESOLVED | ENV | //IGNORE or //TRANSLIT suffix support is optional and not offered here; UNRESOLVED is right |
| XSH.iconv.009 | UNRESOLVED | ENV | //IGNORE or //TRANSLIT suffix support is optional and not offered here; UNRESOLVED is right |
| XSH.kill.005 | UNRESOLVED | ENV | the run supplied no second user id |
| XSH.kill.012 | UNRESOLVED | ENV | the run supplied no second user id |
| XSH.lseek.012 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.lseek.013 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.lseek.014 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.open.050 | UNRESOLVED | SYSTEM | blocked by a required constant missing from its header (see the blocking assertion) |
| XSH.pclose.008 | UNRESOLVED | TEST-BUG | a SIGCHLD set to ignore makes waitpid report ECHILD; the test now restores the default first |
| XSH.posix_spawn.053 | UNRESOLVED | ENV | the attribute setter itself may reject an invalid policy (EINVAL), so spawn cannot be tried; UNRESOLVED is right |
| XSH.psiginfo.002 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.psiginfo.003 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.psiginfo.004 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.psiginfo.005 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.psiginfo.006 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.strsignal.001 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.strsignal.002 | UNRESOLVED | SYSTEM | psignal() is required to be declared in signal.h; it was not (build error), so dependents are blocked |
| XSH.sysconf.015 | UNRESOLVED | SYSTEM | _SC_NSIG or NSIG_MAX is required and absent |
| XSH.tcdrain.002 | UNRESOLVED | SYSTEM | terminal calls from concurrent processes must not fail with an error the page does not list (here ETIME); the setup call failed that way |
| XSH.tcsetattr.004 | UNRESOLVED | SYSTEM | terminal calls from concurrent processes must not fail with an error the page does not list (here ETIME); the setup call failed that way |
| XSH.tcsetattr.012 | UNRESOLVED | SYSTEM | terminal calls from concurrent processes must not fail with an error the page does not list (here ETIME); the setup call failed that way |
| XSH.wcstoimax.009 | UNRESOLVED | TEST-BUG | building the limit-plus-one wide string used a wide formatter that failed on the system; now narrow format then widen |
| XSH.wcstoimax.022 | UNRESOLVED | TEST-BUG | building the limit-plus-one wide string used a wide formatter that failed on the system; now narrow format then widen |
| XSH.wcstol.009 | UNRESOLVED | TEST-BUG | building the limit-plus-one wide string used a wide formatter that failed on the system; now narrow format then widen |
| XSH.wcstoul.009 | UNRESOLVED | TEST-BUG | building the limit-plus-one wide string used a wide formatter that failed on the system; now narrow format then widen |
| XSH.write.012 | UNRESOLVED | ENV | pipe capacity accounting left less than PIPE_BUF room after a 10 byte read; setup not met, so UNRESOLVED is right |
