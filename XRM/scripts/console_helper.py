#!/usr/bin/env python3

import argparse
import socket

PORT = 4220
SOCKETS = {}

usage = """
    Usage:

    get.site 0 NCHAN
    > 32

    set.site 0 transient PRE=50000 POST=50000
    >
"""

def run_main(args):
    print(usage)
    while True:
        try: handle_input(args.uutname)
        except ValueError as e: print(e)

def handle_input(uutname):
    global SOCKETS
    print("> ", end="")
    cmd = input().split(maxsplit=2)
    if len(cmd) < 3: raise ValueError("Invalid command format. [get/set].site sitenum knob")
    _, site, knob = cmd
    if not site.isdigit(): raise ValueError("Invalid site [get/set].site site(int) knob")
    site_port = PORT + int(site)

    if site_port not in SOCKETS: 
        SOCKETS[site_port] = socket.socket()
        SOCKETS[site_port].connect((uutname, site_port))

    try:
        SOCKETS[site_port].sendall(f"{knob}\n".encode())
        response = SOCKETS[site_port].recv(4096).decode("latin-1")
        print(response)
    except (socket.error, BrokenPipeError, ConnectionResetError):
        SOCKETS[site_port].close()
        del SOCKETS[site_port]

def get_parser():
    parser = argparse.ArgumentParser(description='Connect ACQ400CSSP console to uut sockets')
    parser.add_argument('uutname', help="uutname")
    return parser

if __name__ == '__main__':
    run_main(get_parser().parse_args())