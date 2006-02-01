#!/bin/sh

logDir="/net/cedre/chroot/var/www/artis/admin/Logs"

if [ ! -d $logDir ]
then
  logDir="/home/wwwartis/admin/Logs"
fi

output="/tmp/statsQGLViewerDownload"
statFile="$logDir/access_log"

if [ $# -gt 0 ]
then
  statFile=$1
  echo "Stat file set to $1"
fi

if [ ! -f $statFile ]
then
  echo "Unable to find $statFile log file"
  exit
fi

if [ `echo $statFile | grep "\.gz$" | wc -l` != 0  ]
then
  echo "Decompressing..."
  newName="/tmp/logDownloadQGLViewer"
  cp $statFile $newName.gz
  rm -f $newName
  gunzip $newName.gz
  statFile=$newName
fi

echo "Scanning $statFile"
fgrep '/QGLViewer/src/lib/' $statFile | awk '/GET\ [a-zA-Z0-9\/_\.]*\/QGLViewer\/src\/lib/ { print $7,$1 }' $statFile | sed s@".*src/"@@ > $output 

date=`date "+%d-%b-%Y"`
cp $output $output.$date
rm -f $output.$date.gz
gzip -9 $output.$date
echo "Log file saved in $output.$date.gz"
