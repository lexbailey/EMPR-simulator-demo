#!/usr/bin/env python
import socket
import select
import os
import time
from threading import Thread
from Sim.gui import SimWindow
from Sim import peripherals
from Sim import sub_peripherals
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GLib

class SimRunner(Thread):
    def __init__(self, connection, client_address):
        super(SimRunner, self).__init__()
        self.connection = connection
        self.client_address = client_address

        self._int_socket_s, self._int_socket_c = socket.socketpair()
        self._shutdown = False
        self._data_buffer = []

        self._gui_started = False


        self.pinsel = peripherals.Pinsel()
        self.i2c = peripherals.I2C()
        self._peripherals = [
            self.pinsel,
            self.i2c
        ]

        self._peripheral_map = {
            peripheral.get_command_base_name(): peripheral for peripheral in self._peripherals
        }


        self.lcd_i2c_bus = self.i2c.get_bus("i2c1")
        self.keypad_i2c_bus = self.i2c.get_bus("i2c1")

        self._sub_peripherals = []

    def _update_gui(self):
        self._window.set_pinsel_content(self.pinsel.describe_pins())
        self._window.set_comm_content(self.i2c.describe_state())
        self._window.set_lcd_content(self.lcd.get_text())
        self._window.set_lcd_cursor(self.lcd.cursor_on, self.lcd.cursor_blink, self.lcd.pos)

    def _window_closed(self, window, event):
        print("Window closed")
        self.stop()

    def _create_window(self):
        self._window = SimWindow()
        self._window.connect("delete-event", self._window_closed)
        self._window.show_all()
        self.lcd = sub_peripherals.LCD(0x3B, self.lcd_i2c_bus)
        self.keypad = sub_peripherals.Keypad(0x21, self.keypad_i2c_bus, self._window.get_key)
        self._sub_peripherals = [
            self.lcd,
            self.keypad
        ]

    def _do_command(self, command):
        if command == "start":
            GLib.idle_add(self._create_window)
            self._gui_started = True
            return
        if not self._gui_started:
            print("Error: First command was not a 'start' command")
        reply = ""
        try:
            base, pcommand = command.split(".", maxsplit=1)
        except ValueError:
            print("Error: malformed command: %s" % (command))
            return
        try:
            periph = self._peripheral_map[base]
        except KeyError:
            print("Error: no such peripheral: %s" % (base))
            return
        reply = periph.do_command(pcommand)
        GLib.idle_add(self._update_gui)
        return reply


    def _teardown(self):
        print("End client session")
        try:
            self.connection.close()
        except OSError:
            pass
        self._window.teardown()
        GLib.idle_add(self._window.destroy)

    def run(self):
        while not self._shutdown:
            # Wait for the interrupt socket or the data socket
            (read_socks, _, _) = select.select([self.connection, self._int_socket_c], [], [])
            # If the data socket didn't get data, it must have been the interrupt socket
            # so just let the loop iterate again.
            if self.connection not in read_socks:
                continue
            # Otherwise try and read some data
            try:
                data = self.connection.recv(128)
            except socket.timeout:
                continue
            except ConnectionResetError:
                break
            if not data:
                print("End of data from client")
                break
            else:
                d_str = data.decode('ascii')
                chunks = d_str.split('\n')
                while len(chunks) > 1:
                    next_chunk, *chunks = chunks
                    self._data_buffer.append(next_chunk)
                    command = "".join(self._data_buffer)
                    self._data_buffer = []
                    if command != "":
                        reply = self._do_command(command.lower())
                        if reply is None:
                            reply = ""
                        try:
                            if isinstance(reply, bytes):
                                reply = "".join(["%02x" % (byte) for byte in reply])
                            reply_bytes = (reply+'\n').encode('ascii')
                            self.connection.send(reply_bytes)
                        except BrokenPipeError:
                            self._shutdown = True
                        time.sleep(0.01)
                self._data_buffer.extend(chunks)
        self._teardown()

    def stop(self):
        """
            Stop the runner, this does not join the runer, it
            just puts it in a state where it will join promptly if required
        """
        self._shutdown = True
        # Interrupt the select call
        self._int_socket_s.send("X".encode('ascii'))


