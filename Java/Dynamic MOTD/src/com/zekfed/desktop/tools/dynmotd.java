package com.zekfed.desktop.tools;

public class dynmotd {
     public static void main(String[] args) {

          System.out.println(((char) 27)+"[2J");
          System.out.println("Loading...");
  /*
     BASEDIR=$(dirname $0)
CONFIGFILE=~/.config/dynmotd.conf
if [ ! -d ~/.config ];
then
    mkdir ~/.config
fi
if [ ! -f $CONFIGFILE ];
then
    echo -e "# Set options for main script\n# ZIP Code (For weather)\nZIP=11234\n# Print banner (1=yes, 0=no)\nPRINTBAN=1\n# Which banner to print (BANLINUX, BANLINUXBIG, BANUBUNTU, BANANDROID, or BANMACOSX)\nBANNER=BANLINUX" >>$CONFIGFILE
fi
source $CONFIGFILE
TIME=`date`
HOST=`hostname`
WIFIADDR=`ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'`
ETHADDR=`ifconfig eth0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'`
KERNELVER=`uname -r`
USERSTATS=`w`
MEMSIZE=`cat /proc/meminfo | grep MemTotal | awk {'print $2'}`
USER=`whoami`
LASTLOGIN=`last -n 2 -R $USER | awk 'NR==2'`
PROCOUNT=`ps -Afl | wc -l`
PROLIMIT=`ulimit -u`
USERDIR=`echo ~`
WLOC=`weather $ZIP | grep 'Current conditions at' | awk '{print $4}'`
WTEMP=`weather $ZIP | grep 'Temperature:' | awk '{print $2}'`
WCOND=`weather $ZIP | grep 'Weather:' | cut -d: -f2 | cut -c2-`
WSKY=`weather $ZIP | grep 'Sky conditions:' | cut -d: -f2 | cut -c2-`
*/
System.out.println("Done!");
     }
}
