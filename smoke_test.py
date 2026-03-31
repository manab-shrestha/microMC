#!/usr/bin/env python3

import math
import pathlib
import re
import subprocess
import sys


ROOT = pathlib.Path(__file__).resolve().parent
TALLY_PATH = ROOT / "tallies" / "flux_tally.bin"


def run_case(enable_flux: bool) -> float:
    arg = "true" if enable_flux else "false"
    proc = subprocess.run(
        ["./micromc", arg],
        cwd=ROOT,
        capture_output=True,
        text=True,
        check=False,
    )
    if proc.returncode != 0:
        print(proc.stdout)
        print(proc.stderr)
        raise RuntimeError(f"micromc failed for arg={arg}")

    match = re.search(r"final k_eff =\s*([0-9.+\-eE]+)", proc.stdout)
    if not match:
        raise RuntimeError(f"final k_eff line not found for arg={arg}")

    k_eff = float(match.group(1))
    if not math.isfinite(k_eff):
        raise RuntimeError(f"non-finite k_eff for arg={arg}")
    if not (0.5 <= k_eff <= 2.0):
        raise RuntimeError(f"out-of-range k_eff={k_eff} for arg={arg}")
    return k_eff


def main() -> int:
    run_case(False)

    if TALLY_PATH.exists():
        TALLY_PATH.unlink()
    run_case(True)

    if not TALLY_PATH.exists() or TALLY_PATH.stat().st_size == 0:
        raise RuntimeError("flux tally file was not produced in scoring mode")

    print("smoke tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
