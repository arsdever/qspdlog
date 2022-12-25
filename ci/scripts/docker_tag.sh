#!/bin/bash

# Enable exit on error
set -e

echo "The branch name is $GITHUB_REF_NAME"

# Check if the docker image is being built on main brach
# and set the correct outputs for the step
if [ "$GITHUB_EVENT_NAME" == "push" ] && \
	[ "$GITHUB_REF_NAME" == "main" ]; then
	echo "tag=latest" > $GITHUB_OUTPUT;
else
	echo "tag=pr-$GITHUB_REF_NAME" > $GITHUB_OUTPUT;
fi
