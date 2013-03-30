#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>
#include <sys/utsname.h>

using namespace std;

int main()
{
     std::cout << "Loading...\n";
     system("clear");
     system("source $HOME/.config/dynmotd.conf");
     time_t rawtime;
     struct tm * timeinfo;
     time (&rawtime);
     timeinfo = localtime (&rawtime);
     string datetime;
     datetime = asctime(timeinfo);
     char hostname[128];
     gethostname(hostname, sizeof hostname);
     string wlanip;
     wlanip = system("ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'");
     string lanip;
     lanip = system("ifconfig eth0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'");
     string kernelver;
     kernelver = system("uname -r");
     string memsize;
     memsize = system("cat /proc/meminfo | grep MemTotal | awk {'print $2'}");
     string stats;
     stats = system("w");
     string user;
     user = getenv("USER");
     string last;
     last = system("last -n 2 -R $USER | awk 'NR==2'");
     string homedir;
     homedir = getenv("HOME");
     string proccount;
     proccount = system("ps -Afl | wc -l");
     string proclimit;
     proclimit = system("cat /proc/meminfo | grep MemTotal | awk {'print $2'}");
     string wloc;
     wloc = system("weather $ZIP | grep 'Current conditions at' | awk '{print $4}'");
     string zip;
     zip = "ZIP code";
     string wtemp;
     wtemp = "Temperature";
     string wcond;
     wcond = "Weather conditions";
     string wsky;
     wsky = "Sky conditions";
     std::cout << "Done!\n";
     system("clear");

     std::cout << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=[ " << "System Stats" << " ]=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:\n"
             ":   Date/Time = " << datetime << ""
             ":    Hostname = " << hostname << "\n"
             ":     WLAN IP = " << wlanip << "\n"
             ":      LAN IP = " << lanip << "\n"
             ":      Kernel = " << kernelver << "\n"
             ":      Memory = "<< memsize << "kB\n"
             ":       Stats = " << stats << "\n"
             ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:[ " << "User Stats" << " ]:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:\n"
             ":    Username = " << user << "\n"
             ":  Last Login = " << last << "\n"
             ":        Home = " << homedir << "\n"
             ":   Processes = " << proccount << " of " << proclimit << "\n"
             ":=:=:=:=:=:=:=:=:=:=:=:=:=:[ " << "Internet Weather Stats" << " ]:=:=:=:=:=:=:=:=:=:=:=:=:=:\n"
             ":    Location = " << wloc << ", " << zip << "\n"
             ":        Temp = " << wtemp << "\n"
             ":     Weather = " << wcond << "\n"
             ":         Sky = " << wsky << "\n"
             ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:[]:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:";
     return 0;
     /*
      #!/bin/bash

reset

# Load information and settings

echo Loading...
BASEDIR=$(dirname $0)
CONFIGFILE=~/.config/dynmotd.conf
if [ ! -d ~/.config ];
then
    mkdir ~/.config
fi
if [ ! -f $CONFIGFILE ];
then
    echo -e # Set options for main script\n# ZIP Code (For weather)\nZIP=11234\n# Print banner (1=yes, 0=no)\nPRINTBAN=1\n# Which banner to print (BANLINUX, BANLINUXBIG, BANUBUNTU, BANANDROID, or BANMACOSX)\nBANNER=BANLINUX >>$CONFIGFILE
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

echo Done!
reset
      */
}

