#!/bin/bash

# Take hosts file and convert to addr_list preference line
# Usage: ./make_addr_list.sh [url|file] default: stdin
#
# eg
# ./make_addr_list.sh FILE                       # parse file
# ./make_addr_list.sh URL                        # pull data from URL
# grep acq /etc/hosts | ./make_addr_list.sh      # load filtered data from stdin

# at D-TACQ, the multimon web tool provides a hosts file for all live UUT's:
# ./make_addr_list.sh http://naboo:5000/hosts


fetch_hosts() {
	case $1 in
	http*)
		curl -s $1;;
	*)
		if [ -f $1 ]; then
			cat $1
		else
			echo error $1 not found
		fi;;
	esac	
}

ips=$(fetch_hosts $1 \
    | grep -v '^#' | grep -v 127.0.0.1 | grep -v :: | grep -v ^$ \
    | awk '{print $1}' \
    | tr '\n' ' ' \
    | sed 's/ $//' )

cat - >workspace.prefs <<EOF
#org.phoebus.pv/default=ca
#org.phoebus.pv.ca/addr_list=$ips
#org.phoebus.pv.ca/auto_addr_list=true
org.phoebus.pv/default=pva
org.phoebus.pv.pva/epics_pva_auto_addr_list=false
org.phoebus.pv.pva/epics_pva_addr_list=$ips
org.phoebus.pv.pva/epics_pva_name_servers=$ips
EOF


