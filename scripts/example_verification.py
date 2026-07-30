#!/usr/bin/env python3
"""
Verifies output.log produced by the ErrorLogger threading example.

Checks:
  1. Every line matches the expected format (timestamp, level, message, file/line).
  2. No line is truncated/garbled (i.e. every capture group is non-empty and well-formed).
  3. Exactly one TRACE line ("Program started.") is present.
  4. Exactly 1000 ERROR lines are present, indices 0..999, each appearing exactly once
     (catches both dropped messages and duplicated/overwritten buffers).
"""

import re
import sys
from pathlib import Path

LOG_PATH = Path("output.log")
EXPECTED_COUNT = 1000

# Example well-formed line:
# Sat Jul 25 10:16:41 2026\t[ERROR]\t\tLogging message 42: main.cpp[38]
LINE_RE = re.compile(
    r"^(?P<timestamp>.+?)\t"
    r"\[(?P<level>TRACE|DEBUG|WARN|ERROR|FATAL)\]\t\t"
    r"(?P<message>.*?):\s+"
    r"(?P<file>[^\[\]]+)\[(?P<lineno>\d+)\]$"
)

ERROR_MESSAGE_RE = re.compile(r"^Logging message (\d+)$")


def main() -> int:
    if not LOG_PATH.exists():
        print(f"FAIL: {LOG_PATH} does not exist.")
        return 1

    raw_lines = LOG_PATH.read_text(encoding="utf-8", errors="strict").splitlines()

    corrupt_lines = []
    trace_lines = []
    error_indices = []
    seen_indices = {}

    for lineno, raw in enumerate(raw_lines, start=1):
        if raw.strip() == "":
            continue

        m = LINE_RE.match(raw)
        if not m:
            corrupt_lines.append((lineno, raw))
            continue

        level = m.group("level")
        message = m.group("message")

        if message.strip() == "":
            corrupt_lines.append((lineno, raw))
            continue

        if level == "TRACE":
            trace_lines.append((lineno, message))
        elif level == "ERROR":
            em = ERROR_MESSAGE_RE.match(message)
            if not em:
                corrupt_lines.append((lineno, raw))
                continue
            idx = int(em.group(1))
            error_indices.append(idx)
            seen_indices.setdefault(idx, []).append(lineno)

    ok = True

    if corrupt_lines:
        ok = False
        print(f"FAIL: {len(corrupt_lines)} malformed/corrupt line(s):")
        for lineno, raw in corrupt_lines[:20]:
            print(f"  line {lineno}: {raw!r}")
        if len(corrupt_lines) > 20:
            print(f"  ...and {len(corrupt_lines) - 20} more")

    if len(trace_lines) != 1:
        ok = False
        print(f"FAIL: expected exactly 1 TRACE line, found {len(trace_lines)}")
    elif trace_lines[0][1] != "Program started.":
        ok = False
        print(f"FAIL: TRACE line has unexpected message: {trace_lines[0][1]!r}")

    expected = set(range(EXPECTED_COUNT))
    found = set(error_indices)

    missing = sorted(expected - found)
    if missing:
        ok = False
        print(f"FAIL: {len(missing)} missing message index(es): {missing[:20]}"
              + (" ..." if len(missing) > 20 else ""))

    unexpected = sorted(found - expected)
    if unexpected:
        ok = False
        print(f"FAIL: {len(unexpected)} out-of-range index(es): {unexpected[:20]}")

    duplicates = {idx: lines for idx, lines in seen_indices.items() if len(lines) > 1}
    if duplicates:
        ok = False
        print(f"FAIL: {len(duplicates)} duplicated message index(es):")
        for idx, lines in list(duplicates.items())[:20]:
            print(f"  index {idx} appears on lines {lines}")

    if len(error_indices) != EXPECTED_COUNT:
        ok = False
        print(f"FAIL: expected {EXPECTED_COUNT} ERROR lines, found {len(error_indices)}")

    if ok:
        print(f"OK: {len(error_indices)} ERROR lines + 1 TRACE line, all well-formed, "
              "no duplicates, no gaps.")
        return 0

    return 1


if __name__ == "__main__":
    sys.exit(main())