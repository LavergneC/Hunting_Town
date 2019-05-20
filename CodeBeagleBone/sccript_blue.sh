#!/usr/bin/expect

set timeout -1
set connecte 0
set nbnotify 0

exec xset -display :0.0 dpms force off

spawn "bluetoothctl"
expect "# "

while { 1 } {
	set boucle 1
	while { $connecte == 0 } {
	    send "connect F7:2E:52:00:47:AA\r"
	    expect {
	    	"No device connected" {
	    	}
		"Error.Failed" {
		}
	    	"Connection successful" {
	        	set connecte 1
	    	}
	    }
	}

	set timeout 10

	send "select-attribute /org/bluez/hci0/dev_F7_2E_52_00_47_AA/service0009/char000a\r"

	send "notify on\r"
	expect {
		"Notify started" {
		}
		"No attribute selected" {
		  set boucle 0
		}
	}
	incr nbnotify
	if { $nbnotify > 20 } {
		set nbnotify 0
		set connecte 0
	} 
	set timeout -1

	while { $boucle == 1 } {
          expect {
   	   "Value:" {
      		expect "\n"
		exec echo "$expect_out(buffer)" >> /home/debian/Desktop/.bluetooth.log
		exec /home/debian/Desktop/.lancerVideo.sh
     	  }
	  "Connected: no" { 
		set boucle 0
		set connecte 0
	  }
	 }
	}
}


send "exit\r"
interact
