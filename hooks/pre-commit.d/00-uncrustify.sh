#!/bin/sh

# git pre-commit hook that runs an Uncrustify stylecheck.
# Features:
#  - abort commit when commit does not comply with the style guidelines
#  - create a patch of the proposed style changes
#
# More info on Uncrustify: http://uncrustify.sourceforge.net/

# This file is part of a set of unofficial pre-commit hooks available
# at github.
# Link:    https://github.com/githubbrowser/Pre-commit-hooks
# Contact: David Martin, david.martin.mailbox@googlemail.com


##################################################################
# SETTINGS
# set path to uncrustify binary
# UNCRUSTIFY="/usr/bin/uncrustify"
UNCRUSTIFY="/usr/bin/uncrustify"

# set uncrustify config location
# CONFIG="/home/user/.config/uncrustify.cfg"
CONFIG="uncrustify.cfg"

# the source language: C, CPP, D, CS, JAVA, PAWN, VALA, OC, OC+
# SOURCE_LANGUAGE="CPP"
SOURCE_LANGUAGE="C"

# remove any older patches from previous commits. Set to true or false.
# DELETE_OLD_PATCHES=false
DELETE_OLD_PATCHES=false

# only parse files with the extensions in FILE_EXTS. Set to true or false.
# if false every changed file in the commit will be parsed with Uncrustify.
# if true only files matching one of the extensions are parsed with Uncrustify.
# PARSE_EXTS=true
PARSE_EXTS=true

# file types to parse. Only effective when PARSE_EXTS is true.
# FILE_EXTS=".c .h .cpp .hpp"
FILE_EXTS=".c .h"

##################################################################
# There should be no need to change anything below this line.

. "$(dirname -- "$0")/../support/canonicalize_filename.sh"

# exit on error
set -e

# check whether the given file matches any of the set extensions
matches_extension() {
    local filename="$(basename -- "$1")"
    local extension=".${filename##*.}"
    local ext

    for ext in $FILE_EXTS; do [ "$ext" = "$extension" ] && return 0; done

    return 1
}

# necessary check for initial commit
if git rev-parse --verify HEAD >/dev/null 2>&1 ; then
    against=HEAD
else
    # Initial commit: diff against an empty tree object
    against=4b825dc642cb6eb9a060e54bf8d69288fbee4904
fi

# make sure the config file and executable are correctly set
if [ ! -f "$CONFIG" ] ; then
    printf "Error: uncrustify config file not found.\n"
    printf "Set the correct path in $(canonicalize_filename "$0").\n"
    exit 1
fi

if [ ! -x "$UNCRUSTIFY" ] ; then
    printf "Error: uncrustify executable not found.\n"
    printf "Set the correct path in $(canonicalize_filename "$0").\n"
    exit 1
fi

# create a random filename to store our generated patch
prefix="pre-commit-uncrustify"
suffix="$(date +%s)"
patch="/tmp/$prefix-$suffix.patch"

# clean up any older uncrustify patches
$DELETE_OLD_PATCHES && rm -f /tmp/$prefix*.patch

# create one patch containing all changes to the files
git diff-index --cached --diff-filter=ACMR --name-only $against -- | while read file;
do
    # ignore file if we do check for file extensions and the file
    # does not match any of the extensions specified in $FILE_EXTS
    if $PARSE_EXTS && ! matches_extension "$file"; then
        continue;
    fi

    # uncrustify our sourcefile, create a patch with diff and append it to our $patch
    # The sed call is necessary to transform the patch from
    #    --- $file timestamp
    #    +++ - timestamp
    # to both lines working on the same file and having a a/ and b/ prefix.
    # Else it can not be applied with 'git apply'.
    "$UNCRUSTIFY" -c "$CONFIG" -l "$SOURCE_LANGUAGE" -f "$file" | \
        diff -u -- "$file" - | \
        sed -e "1s|--- |--- a/|" -e "2s|+++ -|+++ b/$file|" >> "$patch"
done

# if no patch has been generated all is ok, clean up the file stub and exit
if [ ! -s "$patch" ] ; then
    printf "Files in this commit comply with the uncrustify rules.\n"
    rm -f "$patch"
    exit 0
fi

# a patch has been created, notify the user and exit
printf "\nThe following differences were found between the code to commit "
printf "and the uncrustify rules:\n\n"
cat "$patch"

printf "\nYou can apply these changes with:\n git apply $patch\n"
printf "(may need to be called from the root directory of your repository)\n"
printf "Aborting commit. Apply changes and commit again or skip checking with"
printf " --no-verify (not recommended).\n"

exit 1