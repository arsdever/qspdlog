#!/bin/bash

# https://stackoverflow.com/a/3466183/10185183
# detect the machine type
EXE_SUFFIX=""
unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*);;
    Darwin*);;
    CYGWIN*|MINGW*)     EXE_SUFFIX=".exe";;
    *);;
esac

set -e

cmake -S . -B build/coverage -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DQSPDLOG_REPORT_COVERAGE=ON $@
cmake --build build/coverage -j
ctest --test-dir build/coverage --output-on-failure -j
llvm-profdata merge -sparse build/coverage/test/default.profraw -o coverage.profdata
llvm-cov show build/coverage/test/qspdlog_test_ui$EXE_SUFFIX --instr-profile=coverage.profdata --ignore-filename-regex=build/ --ignore-filename-regex=_autogen --format=html -o coverage_report
rm build/coverage/test/qspdlog_filter_history
rm -fr coverage.profdata
