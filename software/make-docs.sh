#!/bin/bash
type dos2unix >/dev/null 2>&1 || { echo >&2 "We need dos2unix to create the documentation. Please install it and then run this script again"; exit 1; }
DIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIB_DIR="${DIR}/lib"
PYTHON_SRC="${DIR}/python/scan"
LIBS=""
LIBS="${LIBS} -i ${LIB_DIR}/python-pcl"
echo $LIBS
PYTHON_FOLDERS=""
for FOLDER in `ls ${PYTHON_SRC}`
do
    PYTHON_FOLDERS="$PYTHON_FOLDERS -i ${PYTHON_SRC}/$FOLDER"
done
OUTPUT="${DIR}/docs"
mkdir -p $OUTPUT
PROJECT="${DIR}/lib/natural-docs/data"
./lib/natural-docs/NaturalDocs -i $PYTHON_SRC $LIBS -xi ${LIB_DIR}/natural-docs -o framedHTML $OUTPUT -p $PROJECT $*
dos2unix "${DIR}/docs/javascript/main.js"
dos2unix "${DIR}/docs/styles/main.css"
echo "Done!"
