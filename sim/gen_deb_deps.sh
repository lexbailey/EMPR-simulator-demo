#!/usr/bin/env bash
echo "python3-venv"
python3 <(echo -e "try:\n\timport gi\nexcept ImportError:\n\tprint(\"libgirepository1.0-dev\\\nlibcairo2-dev\\\npython3-dev\")")
