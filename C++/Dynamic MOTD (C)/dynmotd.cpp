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
#include <sys/sysinfo.h>
#include <linux/sysinfo.h>
#include <linux/kernel.h>

using namespace std;

int main()
{
     //Begin loading information
     std::cout << "Loading..." << endl <<;
     system("reset");

     //Load Settings
     /*
     # Create configuration file if non-existant

     if [ ! -f $CONFIGFILE ];
     then
         echo -e "# Set options for main script\n# Turn debugging mode on or off (1 to enable, 0 to disable\nDEBUG=0\n# Color for Output\nTITLE_COLOR=\"$WHITE_BOLD\"\nHEADER_COLOR=\"$RED_BOLD\"\nBORDER_COLOR=\"$CYAN\"\nOUTPUT_COLOR=\"$GREEN_BOLD\"\n# Print full output of the command w or only the first line (1=full output, 0=first line only)\nFULLW=0\n# Print weather information (1=yes, 0=no)\nWEATHER=1\n# WEATHER_ZIP Code (For weather)\nWEATHER_ZIP=11234\n# Print banner (1=yes, 0=no)\nPRINTBAN=1\n# Print a custom banner (1=yes, 0=no)\nCUSTBAN=0\n# Text to be displayed in custom banner (backticks and \ escapes are allowed, use a backslash followed by the letter n to end a line(\ n without the space))\nCUSTBANTEXT=\"\"\n# Which banner to print (BANLINUX, BANLINUXBIG, BANUBUNTU, BANANDROID, or BANMACOSX)\nBANNER=BANLINUX\n# Custom Command Section (Set CUSTOM to 1 to activate, 0 to disable)\nCUSTOM=0\nCUSTOM_HEADER=\"Custom Command Stats\"\nCUSTOM_TITLE=\"Custom Command\"\nCUSTOM_COMMAND=\"echo Custom Command Output\"" >> $CONFIGFILE
     fi

     # Link executable if not already done

     if [ ! -f /usr/bin/dynmotd ];
     then
         ln -s "/usr/local/bin/dynmotd" "/usr/bin/"
     fi

     source $CONFIGFILE                                                                                 # Load settings
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

     //System Stats
     struct utsname uname_info;
     uname(&uname_info);
     const long minute = 60;
     const long hour = minute * 60;
     const long day = hour * 24;
     const double megabyte = 1024 * 1024;
     struct sysinfo uname_system_info;
     sysinfo (&uname_system_info);

     //Get time
     time_t hex_time;
     time ( &hex_time );
     string datetime;
     datetime = ctime( &hex_time );

     //Get hostname
     char hostname[128];
     gethostname(hostname, sizeof hostname);

     //Get WLAN IP address
     string wlan_ip;
     int wlan_ip_address;
     struct ifreq wlan_ip_address_numeric;
     wlan_ip_address = socket(AF_INET, SOCK_DGRAM, 0);
     wlan_ip_address_numeric.ifr_addr.sa_family = AF_INET;
     strncpy(wlan_ip_address_numeric.ifr_name, "wlan0", IFNAMSIZ-1);
     ioctl(wlan_ip_address, SIOCGIFADDR, &wlan_ip_address_numeric);
     close(wlan_ip_address);
     wlan_ip = inet_ntoa(((struct sockaddr_in *)&wlan_ip_address_numeric.ifr_addr)->sin_addr);

     //Get LAN IP address
     string lan_ip;
     int lan_ip_address;
     struct ifreq lan_ip_address_numeric;
     lan_ip_address = socket(AF_INET, SOCK_DGRAM, 0);
     lan_ip_address_numeric.ifr_addr.sa_family = AF_INET;
     strncpy(lan_ip_address_numeric.ifr_name, "eth0", IFNAMSIZ-1);
     ioctl(lan_ip_address, SIOCGIFADDR, &lan_ip_address_numeric);
     close(lan_ip_address);
     lan_ip = inet_ntoa(((struct sockaddr_in *)&lan_ip_address_numeric.ifr_addr)->sin_addr);

     //Get kernel version
     string kernel_version;
     kernel_version = uname_info.release;

     //Get RAM size in kB
     //MEMSIZE=`cat /proc/meminfo | grep MemTotal | awk {'print $2'}`
     //printf ("total RAM   : %5.1f MB" << endl <<, si.totalram / megabyte);
     string ram_size;
     ram_size = "RAM";

     //Get current users, jobs, login times, and locations
     //USERSTATS=`w`
     /*printf ("system uptime : %ld days, %ld:%02ld:%02ld" << endl <<,
     si.uptime / day, (si.uptime % day) / hour,
     (si.uptime % hour) / minute, si.uptime % minute);*/
     string user_stats;
     user_stats = system("w | awk 'NR==1' | cut -c2-");

     //Get username
     string username;
     username = getenv("USER");

     //Get time, location, length, and other details for last login
     //LASTLOGIN=`last -n 2 -R $USER | awk 'NR==2'`
     string last_login;
     last_login = system("last -n 2 -R $USER | awk 'NR==2'");

     //Get home directory path
     string home_path;
     home_path = getenv("HOME");

     //Get the current number of this user's active processes
     //PROCOUNT=`ps -Afl | wc -l`
     //printf ("process count : %d" << endl <<, si.procs);
     string process_count;
     process_count = (uname_system_info.procs);

     //Get this user's limit on processes
     //PROLIMIT=`ulimit -u`
     string process_limit;
     process_limit = system("cat /proc/meminfo | grep MemTotal | awk {'print $2'}");

     //Print the location and ZIP code being used for weather
     //WLOC=`weather $ZIP | grep 'Current conditions at' | awk '{print $4}'`
     string weather_location;
     weather_location = "location";
     string weather_zip;
     weather_zip = "zip";

     //Get the temperature
     //WTEMP=`weather $ZIP | grep 'Temperature:' | awk '{print $2}'`
     string weather_temperature;
     weather_temperature = "Temperature";

     //Get the weather conditions (if there are any)
     //WCOND=`weather $ZIP | grep 'Weather:' | cut -d: -f2 | cut -c2-`
     string weather_conditions;
     weather_conditions = "Weather conditions";

     //Get the sky conditions
     //WSKY=`weather $ZIP | grep 'Sky conditions:' | cut -d: -f2 | cut -c2-`
     string weather_sky_conditions;
     weather_sky_conditions = "Sky conditions";

     //Custom section
     string custom_header;
     custom_header = "Custom Command Stats";

     string custom_title;
     custom_title = "Custom Command";

     string custom_command;
     custom_command = "echo -e Custom Command";

     string custom_command_output;
     custom_command_output = system(custom_command);

     //Finished!
     std::cout << "Done!";
     system("reset");

     //Print the loaded information
     std::cout << "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=[ " << red_bold << "System Stats" << cyan << " ]=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:" << endl <<
             "" << cyan << ":   " << white_bold << "Date/Time" << cyan << " = " << green_bold << datetime << ""
             "" << cyan << ":    " << white_bold << "Hostname" << cyan << " = " << green_bold << hostname << endl
             "" << cyan << ":     " << white_bold << "WLAN IP" << cyan << " = " << green_bold << wlan_ip << endl
             "" << cyan << ":      " << white_bold << "LAN IP" << cyan << " = " << green_bold << lan_ip << endl
             "" << cyan << ":      " << white_bold << "Kernel" << cyan << " = " << green_bold << kernel_version << endl
             "" << cyan << ":      " << white_bold << "Memory" << cyan << " = " << green_bold << ram_size << " mB" << endl <<
             "" << cyan << ":       " << white_bold << "Stats" << cyan << " = " << green_bold << user_stats << endl
             "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:[ " << red_bold << "User Stats" << cyan << " ]:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:" << endl <<
             "" << cyan << ":    " << white_bold << "Username" << cyan << " = " << green_bold << username << endl
             "" << cyan << ":  " << white_bold << "Last Login" << cyan << " = " << green_bold << last_login << endl
             "" << cyan << ":        " << white_bold << "Home" << cyan << " = " << green_bold << home_path << endl
             "" << cyan << ":   " << white_bold << "Processes" << cyan << " = " << green_bold << process_count << " of " << process_limit << endl;

     std::cout << "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:[ " << red_bold << "Internet Weather Stats" << cyan << " ]:=:=:=:=:=:=:=:=:=:=:=:=:=:" << endl <<
             "" << cyan << ":    " << white_bold << "Location" << cyan << " = " << green_bold << weather_location << ", " << weather_zip << endl
             "" << cyan << ":        " << white_bold << "Temp" << cyan << " = " << green_bold << weather_temperature << endl
             "" << cyan << ":     " << white_bold << "Weather" << cyan << " = " << green_bold << weather_conditions << endl
             "" << cyan << ":         " << white_bold << "Sky" << cyan << " = " << green_bold << weather_sky_conditions << endl;

     std::cout << "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=[ " << red_bold << custom_header << cyan << " ]=:=:=:=:=:=:=:=:=:=:=:=:=:=:" << endl <<
             "" << cyan << ":    " << white_bold << custom_command << cyan << " = " << green_bold << custom_command_output << endl

             std::cout << "" << cyan << ":=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:[]:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:"
             "" << color_default << endl << endl;
     return 0;
}

