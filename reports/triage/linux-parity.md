# Linux parity triage: root run against ordinary-user run

Rule: a test that does not declare `needs root` gives the same verdict as root and as an ordinary user. 27 assertions differed between the two runs. Decisions: a = needs root, b = test behaved differently by mistake, c = difference the standard allows, d = environment or state effect.

| ID | difference | decision | what changed | verified both ways |
|---|---|---|---|---|
| XSH.2.9-threads.031 | user FAIL, root PASS | d | Nothing. Alone it fails for root and user alike (a read racing a write of the same bytes sees a mixed block); the root PASS in the full run was a timing accident (far fewer overlapping reads). A SYSTEM finding, not a parity one. | yes (4 of 4 FAIL each way) |
| XSH.2.9-threads.032 | user FAIL, root PASS | d | Same as 031 (fstat vs futimens pair). | yes |
| XBD.11.1-interface-characteristics.035 | root FAIL (leader got SIGINT twice), user PASS | b | Already fixed in HEAD after the parity snapshot (handler identity was a variable set after fork; now found by pid and group). | yes (root 3x PASS, user PASS, also under CPU load) |
| XBD.11.1-interface-characteristics.036 | as 035 (SIGQUIT) | b | as 035 | yes |
| XBD.11.1-interface-characteristics.037 | as 035 (SIGTSTP) | b | as 035 | yes |
| XSH.sched_setscheduler.001 | root FAIL, user skipped | a | Already `needs root`; the root FAIL is the known SYSTEM result (returns 0, not the former policy). | yes |
| XSH.setegid.004 | root FAIL, user skipped | a | Already `needs root` (arranges other IDs); root FAIL is the known SYSTEM result. | yes |
| XSH.seteuid.004 | root FAIL, user skipped | a | as setegid.004 | yes |
| XSH.setregid.006 | root FAIL, user skipped | a | Already `needs root`; root FAIL is the known SYSTEM result. | yes |
| XSH.chown.008 | root INSPECT, user skipped | a | Already `needs root` (chown of another user's file); disputed, unchanged. | yes |
| XSH.link.019 | root INSPECT, user skipped | a | Already `needs root` (other user's file); disputed, unchanged. | yes |
| XSH.2.7-xsi-ipc.001 | root INSPECT (id 0), user PASS | d | Nothing. The identifier value depends on the allocation state of the IPC namespace (first queue gets 0), not on the user. In a fresh IPC namespace both users give the same INSPECT; in the used one both PASS. | yes |
| XBD.10.1-directory-structure-and-files.007 | user UNRESOLVED (EACCES opening the console) | c | Nothing. The console need not be openable by an ordinary user; the assertion already says UNRESOLVED when it cannot be opened. | no (cannot be made equal) |
| XCU.newgrp.001 | user UNRESOLVED (shell not created) | d/b | Account's login shell is a no-login stub, and with SHELL unset newgrp starts it. The test now exports SHELL set to the shell under test (PC_SH, else sh) so every account gets a usable shell. | yes (PASS both) |
| XCU.newgrp.002 | as 001 | d/b | as 001 | yes (PASS both) |
| XSH.msgctl.009 | user UNRESOLVED (IPC_STAT EACCES) | b | After setting mode 0, set_mode() read the queue again with IPC_STAT, which mode 0 denies to the owner (root bypasses). The attributes are now read once before and the mode is restored with IPC_SET from that copy. | yes (PASS both) |
| XSH.msgrcv.012 | as msgctl.009 | b | same helper change | yes |
| XSH.msgsnd.009 | as msgctl.009 | b | same helper change | yes |
| XSH.semctl.011 | as msgctl.009 (semaphores) | b | same change with semid_ds | yes |
| XSH.semop.015 | as msgctl.009 (semaphores) | b | same change with semid_ds | yes |
| XSH.posix_spawnattr_getschedparam.003 | user UNRESOLVED (EPERM for real-time policy) | a | Added `needs root` (raising the scheduling class is privileged; the test asks for a real-time policy). User run now reports the standard "skipped: needs root". | yes (root PASS, user skipped) |
| XSH.posix_spawnattr_getschedpolicy.003 | as above | a | as above | yes |
| XSH.setregid.004 | user UNRESOLVED ("needs all IDs 0"), not marked skipped | a | Added `needs root` to the assertion (it sets IDs of other users). | yes (root PASS, user skipped) |
| XSH.setreuid.004 | as setregid.004 | a | as setregid.004 | yes |

Notes
- A scan of all assertions found no other test that demands uid/gid 0 without declaring `needs root`.
- XCU.newgrp.003 FAILs the same way for root and user (this system's newgrp rejects a numeric group operand); not a parity matter, left for the SYSTEM triage.
- Lint: only the expected "declares no MUTANT" lines.
