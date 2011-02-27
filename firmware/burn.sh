#!/bin/bash
#
# Fuses: 0xDF (low), 0xD6 (high)
# -U lfuse:w:0xdf:m -U hfuse:w:0xd6:m
#
if [ "$1" = "" ]; then
    echo "burn.sh <hexfile>"
    exit 1
fi

avrdude -pm168 -cavrisp2 -Pusb -Uflash:w:$1
