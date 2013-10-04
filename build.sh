#!/bin/bash/

find src -regex ".*\.cpp" | sed s/^/CPP_FILES+=/ > inc.mk

cd src
find . -type d -exec mkdir -p ../intermediate/\{\} \;
cd ..

make $@
