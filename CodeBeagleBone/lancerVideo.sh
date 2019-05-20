#!/bin/bash
export DISPLAY=:0


var=`egrep -oa "[0-9]{2}" /home/debian/Desktop/.bluetooth.log | tail -1`

if [ $var -eq 99 ]
then
	exit 0
fi

echo "Lancement de la video n°$var."

xset -display :0.0 dpms force on
mplayer -really-quiet -vo x11 /home/debian/videos/video$var.mp4 >> /dev/null
xset -display :0.0 dpms force off 

exit 0

