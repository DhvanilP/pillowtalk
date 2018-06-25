# CMake generated Testfile for 
# Source directory: /home/dhp/Current/pillowtalk/test
# Build directory: /home/dhp/Current/pillowtalk/cmake-build-debug/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(basic "test_basic")
add_test(json_generator "test_json_generator" "/home/dhp/Current/pillowtalk/test")
add_test(iterator "test_iterator")
add_test(parser "test_parser" "/home/dhp/Current/pillowtalk/test")
add_test(changes_feed "test_changes_feed" "--log_level=warning")
