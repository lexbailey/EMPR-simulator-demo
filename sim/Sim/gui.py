import time
from threading import Thread
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Pango, GLib

class SimWindow(Gtk.Window):

    def __init__(self):

        self.last_key = None
        self.last_key_time = None

        self.cursor_on = False
        self.cursor_blink = False
        self.cursor_pos = 0
        self.cursor_state = True
        self.cursor_stop = False

        Gtk.Window.__init__(self, title="LPC Simulator")
        self.set_default_size(1000, 700)
        root_box = Gtk.HBox()
        peripheral_box = Gtk.VBox()
        serial_box = Gtk.VBox()
        output = Gtk.TextView()

        self.lcd_label = Gtk.Label()
        self.lcd_label.modify_font(Pango.FontDescription("mono 32"))
        self.set_lcd_content("")

        self.pinsel_label = Gtk.Label()
        self.set_pinsel_content("")

        self.comm_label = Gtk.Label()
        self.set_comm_content("")

        keypad_grid = Gtk.Grid()
        gridchars = [
            ['1','2','3','A'],
            ['4','5','6','B'],
            ['7','8','9','C'],
            ['*','0','#','D']
        ]
        x, y = 0, 0
        for row in gridchars:
            for char in row:
                button = Gtk.Button(label=char)
                button.connect("pressed", self.keypad_press)
                button.connect("released", self.keypad_release)
                keypad_grid.attach(button, x, y, 1, 1)
                x += 1
            x = 0
            y += 1


        keypad_grid.props.halign = Gtk.Align.CENTER

        root_box.pack_start(serial_box, True, True, 0)
        root_box.pack_start(peripheral_box, False, True, 0)
        serial_box.pack_start(Gtk.Label(label="Serial"), False, False, 0)
        serial_box.pack_start(output, True, True, 0)
        peripheral_box.pack_start(Gtk.Label(label="LCD"), False, False, 0)
        peripheral_box.pack_start(self.lcd_label, False, False, 0)
        peripheral_box.pack_start(Gtk.Separator(), False, False, 0)
        peripheral_box.pack_start(Gtk.Label(label="Keypad"), False, False, 0)
        peripheral_box.pack_start(keypad_grid, False, True, 0)
        peripheral_box.pack_start(Gtk.Separator(), False, False, 0)
        peripheral_box.pack_start(Gtk.Label(label="Comms"), False, False, 0)
        peripheral_box.pack_start(self.comm_label, False, False, 0)
        peripheral_box.pack_start(Gtk.Separator(), False, False, 0)
        peripheral_box.pack_start(Gtk.Label(label="Pin Config"), False, False, 0)
        peripheral_box.pack_start(self.pinsel_label, False, False, 0)

        self.add(root_box)

        self.cursor_thread = Thread(target=self.cursor_timer)
        self.cursor_thread.start()

    def teardown(self):
        self.cursor_stop = True
        self.cursor_thread.join()

    def set_lcd_cursor(self, cursor_on, cursor_blink, cursor_pos):
        self.cursor_on = cursor_on
        self.cursor_blink = cursor_blink
        if cursor_pos >= 0x40:
            cursor_pos -= 0x40
            cursor_pos += 16
        self.cursor_pos = cursor_pos

    def keypad_press(self, button):
        self.last_key = button.get_label()
        self.last_key_time = None

    def keypad_release(self, button):
        self.last_key_time = time.time()

    def get_key(self):
        if self.last_key_time is not None:
            if self.last_key_time + 0.1 < time.time():
                self.last_key = None
                self.last_key_time = None
        if self.last_key is None:
            return None
        return self.last_key

    def render_lcd(self):
        header = "\u2554" + ("\u2550" * 16) + "\u2557"
        footer = "\u255A" + ("\u2550" * 16) + "\u255D"
        lines = "\u2551%s\u2551"
        line_num = self.cursor_pos // 16
        col_num = self.cursor_pos % 16
        base_lines = self.lcd_base_content
        if line_num <= 1 and self.cursor_on:
            if not self.cursor_blink or self.cursor_state:
                l = base_lines[line_num]
                base_lines[line_num] = "%s<span foreground='#ffffff' background='#000000'>%s</span>%s" % (l[0:col_num], l[col_num], l[col_num+1:])
        lcd_text = [
            header,
            *(lines % (line) for line in base_lines),
            footer
        ]
        self.lcd_label.set_markup("\n".join(lcd_text))

    def set_lcd_content(self, content):
        if '\n' not in content:
            content += '\n'
        def to_length(s, l=16):
            sl = len(s)
            if sl > l:
                return s[0:l]
            return s + (" " * (l-sl))
        self.lcd_base_content = list(to_length(line) for line in content.split("\n"))
        self.render_lcd()

    def update_cursor(self):
        if self.cursor_on:
            if self.cursor_blink:
                self.cursor_state = not self.cursor_state
            else:
                self.cursor_state = True
        self.render_lcd()

    def cursor_timer(self):
        while not self.cursor_stop:
            GLib.idle_add(self.update_cursor)
            time.sleep(0.5)

    def set_pinsel_content(self, content):
        self.pinsel_content = content
        self.pinsel_label.set_text(self.pinsel_content)

    def set_comm_content(self, content):
        self.comm_content = content
        self.comm_label.set_text(self.comm_content)
