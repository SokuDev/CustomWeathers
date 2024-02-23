#!/bin/sh
export SHADY="$(pwd)/shady/shady-cli.exe"
for file in `find $(pwd) -name "*.png" -o -name "*.xml" -o -name "*.csv"`; do
	cd $(dirname $file)
	$SHADY convert $file
	cd - >/dev/null
done
rm -rf output
cp -r data output
find output -name "*.png" -delete -print -o -name "*.xml" -delete -print -o -name "*.csv" -delete -print
find data   -name "*.cv0" -delete -print -o -name "*.cv1" -delete -print -o -name "*.cv2" -delete -print -o -name "*.pat" -delete -print
