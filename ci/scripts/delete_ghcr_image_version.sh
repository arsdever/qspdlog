#!/bin/bash

set -e

# Check input arguments
if [ $# -ne 2 ]; then
	echo "Usage: $0 <token> <image_version>"
	exit 1
fi

# Get token
token=$1
image_version=$2

# Get image versions
id=$(curl \
	-X GET \
	-H "Accept: application/vnd.github.v3+json" \
	-H "Authorization: bearer $token" \
	https://api.github.com/users/arsdever/packages/container/qspdlog/versions | \
	jq '.[] | select(.name == "'"$image_version"'")' | \
	jq .id)

curl \
	-X DELETE \
	-H "Accept: application/vnd.github.v3+json" \
	-H "Authorization: bearer $token" \
	https://api.github.com/users/arsdever/packages/container/qspdlog/versions/$id
