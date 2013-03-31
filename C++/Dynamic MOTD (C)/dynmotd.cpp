#include <iostream>
#include <string>
#include <string.h>
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
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

using namespace std;

int main()
{
     //Begin loading information
     std::cout << "Loading...\n";
     system("clear");

     //Load Settings
     //WIP

     //Get time
     time_t rawtime;
     time ( &rawtime );
     string datetime;
     datetime = ctime( &rawtime );

     //Get hostname
     char hostname[128];
     gethostname(hostname, sizeof hostname);

     //Get WLAN IP address
     string wlanip;
     int wlanipaddr;
     struct ifreq wlanipnumaddr;
     wlanipaddr = socket(AF_INET, SOCK_DGRAM, 0);
     wlanipnumaddr.ifr_addr.sa_family = AF_INET;
     strncpy(wlanipnumaddr.ifr_name, "wlan0", IFNAMSIZ-1);
     ioctl(wlanipaddr, SIOCGIFADDR, &wlanipnumaddr);
     close(wlanipaddr);
     wlanip = inet_ntoa(((struct sockaddr_in *)&wlanipnumaddr.ifr_addr)->sin_addr);

     //Get LAN IP address
     string lanip;
     int lanipaddr;
     struct ifreq lanipnumaddr;
     lanipaddr = socket(AF_INET, SOCK_DGRAM, 0);
     lanipnumaddr.ifr_addr.sa_family = AF_INET;
     strncpy(lanipnumaddr.ifr_name, "eth0", IFNAMSIZ-1);
     ioctl(lanipaddr, SIOCGIFADDR, &lanipnumaddr);
     close(lanipaddr);
     lanip = inet_ntoa(((struct sockaddr_in *)&lanipnumaddr.ifr_addr)->sin_addr);

     //Get kernel version
     struct utsname u;
     uname(&u);
     string kernelver;
     kernelver = u.release;

     //Get RAM size in kB
     string memsize;
     memsize = "mem";

     //Get current users, jobs, login times, and locations
     string stats;
     stats = system("w");

     //Get username
     string user;
     user = getenv("USER");

     //Get time, location, length, and other details for last login
     string last;
     last = system("last -n 2 -R $USER | awk 'NR==2'");

     //Get home directory path
     string homedir;
     homedir = getenv("HOME");

     //Get the current number of this user's active processes
     string proccount;
     proccount = system("ps -Afl | wc -l");

     //Get this user's limit on processes
     string proclimit;
     proclimit = system("cat /proc/meminfo | grep MemTotal | awk {'print $2'}");

     //Print the location and ZIP code being used for weather
     string wloc;
     wloc = system("weather $ZIP | grep 'Current conditions at' | awk '{print $4}'");
     string zip;
     zip = "zip";

     //Get the temperature
     string wtemp;
     wtemp = "Temperature";

     //Get the weather conditions (if there are any)
     string wcond;
     wcond = "Weather conditions";

     //Get the sky conditions
     string wsky;
     wsky = "Sky conditions";

     //Finished!
     std::cout << "Done!\n";
     system("clear");

     //Print the loaded information
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

