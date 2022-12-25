#!/bin/bash

# Enable exit on error
set -e

# Extract branch name from the ref
branch="${GITHUB_REF#refs/heads/}"

# Check if the docker image is being built on main brach
# and set the correct outputs for the step
if [ "$GITHUB_EVENT_NAME" == "push" ] && \
	[ $branch == "main" ]; then
	echo "tag=latest" > $GITHUB_OUTPUT;
else
	echo "tag=pr-$branch" > $GITHUB_OUTPUT;
fi
