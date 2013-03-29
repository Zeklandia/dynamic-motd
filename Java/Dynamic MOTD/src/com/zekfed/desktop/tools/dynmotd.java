package com.zekfed.desktop.tools;

import java.io.File;
import java.io.IOException;
import jcurses.system.Toolkit;
import org.fusesource.jansi.AnsiConsole;

public class dynmotd {
     public static void main(String[] args) throws IOException {

          Runtime runTime = Runtime.getRuntime();
          Toolkit.clearScreen(null);
          System.out.println("Loading...");

          String userHome = System.getProperty("user.home");
          File folder = new File(userHome + "/.config");
          if(folder.exists()){
          File configFile = new File(userHome + "/.config/dynmotd.conf");
          if(configFile.exists()) {}
          }
          else{
          boolean wasDirectoryMade = folder.mkdirs();
          if(wasDirectoryMade);
          else
               System.out.println("Error creating configuration. Is your home foler mounted R/W?");
          }


/*
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
WLOC=`weather $ZIP | grep 'Current conditions at' | awk '{print $4}'`
WTEMP=`weather $ZIP | grep 'Temperature:' | awk '{print $2}'`
WCOND=`weather $ZIP | grep 'Weather:' | cut -d: -f2 | cut -c2-`
WSKY=`weather $ZIP | grep 'Sky conditions:' | cut -d: -f2 | cut -c2-`
*/
System.out.println("Done!");
     }
}
