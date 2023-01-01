#!/bin/bash

# Enable exit on error
set -e

# Check if docker is installed
which docker
if [ $? -ne 0 ]; then
	echo "Docker is not installed"
	exit 1
fi

# Get the dockerfile name, image name and the tag
dockerfile=""
imagename=""
tag=""

	# Check the argument count
if [ $# -lt 2 ]; then
	echo "Usage: $0 <dockerfile> <imagename> <tag>"
	exit 1
else
	dockerfile=$1
	imagename=$2
	tag=$3
fi

docker build -f $dockerfile -t $imagename:$tag .
docker push $imagename:$tag
