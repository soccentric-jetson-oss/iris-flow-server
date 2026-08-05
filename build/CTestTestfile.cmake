# CMake generated Testfile for 
# Source directory: /home/sandesh/jetson/build/iris-flow-server
# Build directory: /home/sandesh/jetson/build/iris-flow-server/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[iris-flow-server-test]=] "/home/sandesh/jetson/build/iris-flow-server/build/iris-flow-server-test")
set_tests_properties([=[iris-flow-server-test]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/sandesh/jetson/build/iris-flow-server/CMakeLists.txt;46;add_test;/home/sandesh/jetson/build/iris-flow-server/CMakeLists.txt;0;")
subdirs("_deps/catch2-build")
