#!/bin/sh

# source the common platform independent functionality and option parsing
script_location=$(dirname $(readlink --canonicalize $0))
. ${script_location}/common_test.sh

# run tests
retval=0
echo "running CernVM-FS test cases..."
cd ${SOURCE_DIRECTORY}/test
./run.sh $TEST_LOGFILE -x src/001-chksetup             \
                          src/004-davinci              \
                          src/005-asetup               \
                          src/007-testjobs             \
                          src/008-default_domain       \
                          src/024-reload-during-asetup \
                          src/5* || retval=$?

exit $retval