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
     system("reset");

     //Load Settings
     /*
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
     */
     //Also need to link this file to /usr/bin/dynmotd-c

     //Set colors
     string color_default;
     color_default = "\033[0m";
     string white;
     white = "\033[0;37m";
     string white_bold;
     white_bold = "\033[1;37m";
     string cyan;
     cyan = "\033[0;36m";
     string cyan_bold;
     cyan_bold = "\033[1;36m";
     string red;
     red = "\033[0;31m";
     string red_bold;
     red_bold = "\033[1;31m";
     string green;
     green = "\033[0;32m";
     string green_bold;
     green_bold = "\033[1;32m";

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
     //MEMSIZE=`cat /proc/meminfo | grep MemTotal | awk {'print $2'}`
     string memsize;
     memsize = "mem";

     //Get current users, jobs, login times, and locations
     //USERSTATS=`w`
     string stats;
     stats = system("w");

     //Get username
     string user;
     user = getenv("USER");

     //Get time, location, length, and other details for last login
     //LASTLOGIN=`last -n 2 -R $USER | awk 'NR==2'`
     string last;
     last = system("last -n 2 -R $USER | awk 'NR==2'");

     //Get home directory path
     string homedir;
     homedir = getenv("HOME");

     //Get the current number of this user's active processes
     //PROCOUNT=`ps -Afl | wc -l`
     string proccount;
     proccount = system("ps -Afl | wc -l");

     //Get this user's limit on processes
     //PROLIMIT=`ulimit -u`
     string proclimit;
     proclimit = system("cat /proc/meminfo | grep MemTotal | awk {'print $2'}");

     //Print the location and ZIP code being used for weather
     //WLOC=`weather $ZIP | grep 'Current conditions at' | awk '{print $4}'`
     string wloc;
     wloc = "location";
     string wzip;
     wzip = "zip";

     //Get the temperature
     //WTEMP=`weather $ZIP | grep 'Temperature:' | awk '{print $2}'`
     string wtemp;
     wtemp = "Temperature";

     //Get the weather conditions (if there are any)
     //WCOND=`weather $ZIP | grep 'Weather:' | cut -d: -f2 | cut -c2-`
     string wcond;
     wcond = "Weather conditions";

     //Get the sky conditions
     //WSKY=`weather $ZIP | grep 'Sky conditions:' | cut -d: -f2 | cut -c2-`
     string wsky;
     wsky = "Sky conditions";

     //Finished!
     std::cout << "Done!\n";
     system("reset");

     //Print the loaded information
     std::cout << "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=[ " << red_bold << "System Stats" << cyan << " ]=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:\n"
             "" << cyan << ":   " << white_bold << "Date/Time" << cyan << " = " << green_bold << datetime << ""
             "" << cyan << ":    " << white_bold << "Hostname" << cyan << " = " << green_bold << hostname << "\n"
             "" << cyan << ":     " << white_bold << "WLAN IP" << cyan << " = " << green_bold << wlanip << "\n"
             "" << cyan << ":      " << white_bold << "LAN IP" << cyan << " = " << green_bold << lanip << "\n"
             "" << cyan << ":      " << white_bold << "Kernel" << cyan << " = " << green_bold << kernelver << "\n"
             "" << cyan << ":      " << white_bold << "Memory" << cyan << " = " << green_bold << memsize << " kB\n"
             "" << cyan << ":       " << white_bold << "Stats" << cyan << " = " << green_bold << stats << "\n"
             "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:[ " << red_bold << "User Stats" << cyan << " ]:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:\n"
             "" << cyan << ":    " << white_bold << "Username" << cyan << " = " << green_bold << user << "\n"
             "" << cyan << ":  " << white_bold << "Last Login" << cyan << " = " << green_bold << last << "\n"
             "" << cyan << ":        " << white_bold << "Home" << cyan << " = " << green_bold << homedir << "\n"
             "" << cyan << ":   " << white_bold << "Processes" << cyan << " = " << green_bold << proccount << " of " << proclimit << "\n"
             "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:[ " << red_bold << "Internet Weather Stats" << cyan << " ]:=:=:=:=:=:=:=:=:=:=:=:=:=:\n"
             "" << cyan << ":    " << white_bold << "Location" << cyan << " = " << green_bold << wloc << ", " << wzip << "\n"
             "" << cyan << ":        " << white_bold << "Temp" << cyan << " = " << green_bold << wtemp << "\n"
             "" << cyan << ":     " << white_bold << "Weather" << cyan << " = " << green_bold << wcond << "\n"
             "" << cyan << ":         " << white_bold << "Sky" << cyan << " = " << green_bold << wsky << "\n"
             "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:[]:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:"
             "" << color_default << endl << endl;
     return 0;
}

