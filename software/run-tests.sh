#!/bin/bash
type nosetests >/dev/null 2>&1 || { echo >&2 "We need nosetests, from the python package nose, to run our test. Please install it (most easily through PIP) and then run this script again"; exit 1; }
DIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# run the python tests
nosetests -v -w ${DIR}/python/scan/test $*
