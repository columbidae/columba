#!/bin/sh

for hook_path in hooks/*; do
	hook=$(basename $hook_path);
	if [ \( ! -x .git/hooks/$hook \) -a \( -f hooks/$hook \) ]; then
		ln -s ../../$hook_path .git/hooks/$hook;
	fi;
done

./gitlog-to-changelog > ChangeLog
libtoolize
autoreconf -iv || exit 1
./configure "$@"
