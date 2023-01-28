#!/bin/bash

# setup git hooks
cp ci/scripts/hooks/clang-format-hook.sh .git/hooks/pre-commit
