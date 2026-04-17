#!/usr/bin/env python3
"""Fetch the official doctest single-header and write it to tests/vendor/doctest.h

This script downloads the raw `doctest.h` from the doctest GitHub repository.
It overwrites `tests/vendor/doctest.h` so the vendored header is the official one.
If the download fails, the existing file is left untouched.
"""
import os
import sys

URL = "https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h"
HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.normpath(os.path.join(HERE, '..', 'vendor', 'doctest.h'))

def fetch():
    try:
        # prefer urllib (std lib)
        from urllib.request import urlopen
        with urlopen(URL, timeout=20) as resp:
            data = resp.read()
            if not data:
                print('Empty response when fetching doctest.h', file=sys.stderr)
                return 1
            # ensure vendor dir exists
            vendor_dir = os.path.dirname(OUT)
            os.makedirs(vendor_dir, exist_ok=True)
            with open(OUT, 'wb') as f:
                f.write(data)
        print('Wrote', OUT)
        return 0
    except Exception as e:
        print('Failed to fetch doctest.h:', e, file=sys.stderr)
        return 2

if __name__ == '__main__':
    sys.exit(fetch())
