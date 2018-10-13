#!/usr/bin/env python
import socket
from Sim import SimRunner
import os
from threading import Thread
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

# Some signal magic to work around a Gtk bug
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

socket_path = os.environ.get('LPC17XX_TESTING_SOCKET_NAME', './.lpc17_testing_socket')

def server_socket():
    if os.path.exists(socket_path):
        os.remove(socket_path)
    server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    server.settimeout(None)
    server.bind(socket_path)
    server.listen(1)
    print("Listening to socket `%s`..." % socket_path)
    return server


def accept_connections():
    runners = []
    server = server_socket()
    try:
        while True:
            try:
                connection, client_address = server.accept()
            except socket.timeout:
                continue
            print("New client connected")
            new_runner = SimRunner(connection, client_address)
            new_runner.start()
            runners.append(new_runner)
    except KeyboardInterrupt:
        pass
    print("Exit")
    server.close()
    Gtk.main_quit()
    print("Waiting for runners to stop...")
    for runner in runners:
        runner.stop()
    for runner in runners:
        runner.join()
    os.remove(socket_path)
    print("Done")

Thread(target=accept_connections).start()
Gtk.main()
