#!/usr/bin/env bash
python3 <(echo -e "try:\n\timport gi\nexcept ImportError:\n\tprint(\"pygobject>=3.27.1\")")
