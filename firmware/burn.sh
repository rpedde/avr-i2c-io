#!/bin/bash

if [ "$1" = "" ]; then
    echo "burn.sh <hexfile>"
    exit 1
fi

avrdude -pm168 -cavrisp2 -Pusb -Uflash:w:$1
