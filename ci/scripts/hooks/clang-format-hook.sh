#!/bin/bash

clang_format_executable=clang-format
if [ -z "$(which $clang_format_executable)" ]; then
  echo "$clang_format_executable not found"
  exit 1
fi

STYLE=$(git config --get hooks.clangformat.style)
if [ -n "${STYLE}" ] ; then
  STYLEARG="-style=${STYLE}"
else
  STYLEARG=""
fi

format_file() {
  file="${1}"
  if [ -f "$file" ]; then
    $clang_format_executable -i "${STYLEARG}" "${1}"
    git add "${1}"
  fi
}

case "${1}" in
  --about )
    echo "Runs clang-format on source files"
    ;;
  * )
    for file in $(git diff-index --cached --name-only HEAD | grep -iE '\.(cpp|cc|h|hpp)$' ) ; do
      format_file "${file}"
    done
    ;;
esac
