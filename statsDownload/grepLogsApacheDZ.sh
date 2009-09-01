#!/bin/sh

input="/tmp/access_log"

if [ $1=="-f" ]
then
  shift
  input=$1
  echo "Stat file set to $1"
  shift
else
  logDir="/var/log/apache2/libqglviewer"
  accessFile="$logDir/access_log"
  scp root@192.168.0.10:$accessFile $input
fi

if [ ! -f $input ]
then
  echo "Unable to find $input input log file"
  exit
fi

if [ `echo $input | grep "\.gz$" | wc -l` != 0  ]
then
  echo "Decompressing..."
  newName="/tmp/logDownloadQGLViewer"
  cp $input $newName.gz
  rm -f $newName
  gunzip $newName.gz
  input=$newName
fi

if [ $# -ne 2 ]
then
  echo "Usage : $0 [-f file] month year"
  echo  "Example $0 03 2009"
  exit
fi

export LC_TIME="en_US.UTF-8"
month=`date -d "$1/01/2000" "+%b"` 

echo "Scanning $input for $month $2"

output="$2.$1"
grep 'GET /src/lib' $input | grep " 200 " | grep -i "/$month/$2:" | cut -d " " -f7 | sed s@"/src/"@@ | sort | uniq -c | awk '{print $2,$1;}'> $output
gzip -9 $output
echo "Log file saved in $output.gz"
