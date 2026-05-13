# Spooky View Privacy Policy

This app will not collect any information.

## Update checker

This app will check for updates when started by default. This option can be disabled from the settings menu option in the notification area context menu.

When checking for updates the following details are send to https://updates.tyndomyn.net/spookyview/status 
- version number of app
- language of Windows for current user (for example: en-US)
- computer architecture (x64, x86, AArch64)
- packaging format (Microsoft Store, MSI, Portable)
- a `pre_release=1` flag (only when the running build was compiled as a pre-release build)

## Local diagnostics

SpookyView writes diagnostic artifacts to your local machine to help you
troubleshoot issues. These files **stay on the local machine** — SpookyView
never transmits them anywhere. You may delete them at any time.

### Log file

- **Location:** `%LOCALAPPDATA%\SpookyView\spookyview.log`
- **Rotation:** When the log reaches roughly 1 MiB, it is rotated to
  `spookyview.log.old` (a single previous generation is kept).
- **Transmission:** Stays on the local machine. Never transmitted by
  SpookyView.
- **Contents:** Timestamps, source `file:line` tags, and `INFO` / `ERROR`
  messages about application lifecycle and dialog events. The log does
  **not** contain window contents, keystrokes, or clipboard data.

### Crash minidumps

- **Location:**
  `%LOCALAPPDATA%\SpookyView\spookyview-crash-YYYYMMDD-HHMMSS-PID.dmp`
- **When written:** Produced by SpookyView's internal vectored exception
  handler when a fatal exception occurs.
- **Format:** Standard `MiniDumpNormal`-class dumps. They may contain stack
  memory (potentially small fragments of in-process data) but exclude full
  process memory unless you separately enable Windows WER LocalDumps with
  `DumpType=2`.
- **Transmission:** Stay on the local machine. Never transmitted by
  SpookyView. You may delete them at any time.
- **Windows WER LocalDumps:** If you enable Windows WER LocalDumps for
  `SpookyView.exe`, those dumps are also local-only by default — they are
  written by Windows itself and are not transmitted by SpookyView.
