#!/usr/bin/env bash
fail=0
missing=""
./gen_deb_deps.sh > deb-deps
./gen_python_requirements.sh > requirements.txt
while read dep; do
    echo -n "Checking for $dep..."
    dpkg-query -W $dep >/dev/null 2>/dev/null
    got_dep=$?
    if [ $got_dep -eq 0 ]; then
        echo " Found"
    else
        echo " Not found"
        missing="$missing $dep"
        fail=1
    fi
done < deb-deps

if [ $fail -eq 1 ]; then
    echo ""
    echo "Failed to find some required packages. Please install these packages."
    echo "    apt install$missing"
    exit 1
else
    echo "All deb requirements satisfied"
fi

rm -rf venv
python3 -m venv --system-site-packages venv
source venv/bin/activate
pip install -r requirements.txt
