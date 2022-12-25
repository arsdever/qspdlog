#!/bin/bash

# Enable exit on error
set -e

# Check if docker is installed
which docker
if [ $? -ne 0 ]; then
	echo "Docker is not installed"
	exit 1
fi

# Check the argument count
if [ $# -lt 1 ]; then
	echo "Usage: $0 <action>"
	echo "Actions:"
	echo "  build"
	echo "  push"
	echo "  build_push"
	echo "  delete"
	exit 1
fi

# Get the action to perform
action=$1

# Check if the action is valid
if [ "$action" != "build" ] && \
	[ "$action" != "push" ] && \
	[ "$action" != "build_push" ] && \
	[ "$action" != "delete" ]; then
	echo "Invalid action: $action"
	exit 1
fi

# Get the dockerfile name, image name and the tag
dockerfile=""
imagename=""
tag=""

if [ "$action" != "delete" ]; then
	# Check the argument count
	if [ $# -lt 4 ]; then
		echo "Usage: $0 $action <dockerfile> <imagename> <tag>"
		exit 1
	else
		dockerfile=$2
		imagename=$3
		tag=$4

		# Check if the dockerfile exists
		if [ ! -f "$dockerfile" ]; then
			echo "Dockerfile not found: $dockerfile"
			exit 1
		fi
	fi
else
	# Check the argument count
	if [ $# -lt 3 ]; then
		echo "Usage: $0 $action <imagename> <tag>"
		exit 1
	else
		imagename=$2
		tag=$3
	fi
fi

case $action in
	"build")
		docker build -f $dockerfile -t $imagename:$tag .
		;;
	"push")
		docker push $imagename:$tag
		;;
	"build_push")
		docker build -f $dockerfile -t $imagename:$tag .
		docker push $imagename:$tag
		;;
	"delete")
		docker rmi $imagename:$tag
		;;
esac
