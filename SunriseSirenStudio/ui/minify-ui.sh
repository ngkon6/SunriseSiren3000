#!/bin/bash

DIR=$(dirname -- "$(readlink -f -- "$0")");

UI_CONTENTS=$(sed -E 's/"/\\"/g;s/^\s+//g' "$DIR/StudioUI.glade" | tr -d "\n")

if [ -z "$UI_CONTENTS" ]; then
    echo "\e[31;1mAn error occurred while fetching and minifying the UI."
    exit 9
fi

echo "const gchar* UI = \"$UI_CONTENTS\";" > $DIR/../include/UI.h
