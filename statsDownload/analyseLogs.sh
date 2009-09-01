#!/bin/bash

tmp="/tmp"
tmpDir="$tmp/LogsQGLViewer"
statFile="stats.txt"
test -d $tmpDir && rm -f $tmpDir/*
test -d $tmpDir || mkdir $tmpDir

# Mind the order (src.rpm must be before rpm, lib3ds before the rest) !!
#types="lib3ds examples-linux examples-mac examples-win src.rpm devel rpm  tar dll zip "
#        11       10                9            8           7      6    5   4   3   2

types="lib3ds examples-linux examples-mac examples-win src.rpm devel rpm deb tar dll zip installer.exe"
#        2       3                4            5           6      7    8   9   10  11  12      13


for f in `ls -1 -- 200?.[0-9][0-9]* | sort -n`
do
  output="$tmpDir/$f"
  cp $f $output

  nogz=`echo $f | sed s@".gz"@@`

  if [ -f $statFile ]
  then
  if [[ `grep "^$nogz" $statFile` ]]
  then
      echo "$nogz - already computed"
      continue
  fi
  fi

  if [[ "$f" != "$nogz" ]]
  then
      gunzip $output >& /dev/null
      output="$tmpDir/$nogz"
  fi
  
  for t in $types
  do
    fgrep -- "$t" $output > $output.$t
    fgrep -v -- "$t" $output > $output.remain
    mv $output.remain $output
  done

  if [ ! -s $output ]
  then
      rm $output
  else
      mv $output $output.remain
      echo "Unrecognized lines remain : "`wc -l $output.remain`
  fi

  line=""
  ipAddress=$(awk '{print $2}' $output.rpm | grep "\." | wc -l)
  for t in $types
  do
  if [ $ipAddress -eq 0 ]
  then
    line="$line\t$(awk 'BEGIN {SUM=0} {SUM+=$2} END {print SUM}' $output.$t)"
  else
    line="$line\t`awk '{print $2}' $output.$t | sort -n | uniq | wc -l`"
  fi
  done
  line="$nogz\t$line"
  echo -e $line
  echo -e $line >> $statFile
done

echo "Creating images"
gnuplot plot

echo "Copy images to web dir"
makeDist="../makeDist"
if [[ ! -f  $makeDist ]]
then
  echo "Unable to find $makeDist, aborting"
  exit 1
fi

WebSuffix=`grep -a ^WebSuffix $makeDist | sed s/'^.*=\"\(.*\)\"$'/"\1"/`
WebDir=`grep -a ^WebDir $makeDist | sed s/'^.*=\"\(.*\)\"$'/"\1"/ | sed s%'$WebSuffix'%"$WebSuffix"%`
ImgDir="$WebDir/images"

if [[ ! -d  $ImgDir ]]
then
  echo "Unable to find web image directory ($ImgDir) - Aborting"
  exit 1
fi

cp downloads.png examples.png $ImgDir
touch $WebDir/download_statistics.html
touch ../doc/download_statistics.html
