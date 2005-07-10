#!/bin/sh

composite -compose difference $1 .corners.png /tmp/composite.png
composite -geometry +13+12 /tmp/composite.png .background.png $1.jpg
echo "$1.jpg created"
