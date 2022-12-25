#!/bin/bash

# Enable exit on error
set -e

# Check if the docker image is being built on main brach
# and set the correct outputs for the step
if [ "$GITHUB_EVENT_NAME" == "push" ] && \
	[ "$GITHUB_REF" == "refs/heads/main" ]; then
	echo "tag=latest" > $GITHUB_OUTPUT;
else
	echo "tag=pr-$GITHUB_REF" > $GITHUB_OUTPUT;
fi
