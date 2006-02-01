#!/bin/sh

if [ $# != 1 ]
then
  echo "Usage : $0 awstat_file"
  exit
fi

if [ ! -f $1 ]
then
  echo "Unable to find $1"
  exit
fi

if [ $(echo $1 | grep "/") ]
then
  dir=$(echo $1 | sed s%"\(.*/\)[^/]*"%"\1"%)
fi
src=$dir$(grep mainright $1 | sed s%".*src=\"\([^\"]*\)\".*"%"\1"%)
date=$(grep "<title>" $src | sed s%".*(\([0-9-]*\)).*"%"\1"% | sed s%"-"%"."%)

echo "Scanning $src - Date is $date"

grep "^<tr>" $src | grep "aws" | grep -v "analyse" | grep -v "Reported" | head --lines=-1 | sed s%".*href=\"[^\"]*/\([^/\"]*\)\".*</a></td><td>\([0-9]*\).*"%"\1 \2"% > $date

echo $(wc -l $date | sed s%"\([0-9]*\).*"%"\1"%) "lines saved in $date.gz"
gzip -9 $date
