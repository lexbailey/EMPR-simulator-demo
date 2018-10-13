from enum import Enum

class SubPeripheral():
    pass


class I2CSubPeripheral(SubPeripheral):
    def __init__(self, addr, i2cbus):
        self.i2cbus = i2cbus
        self.addr = addr
        self.i2cbus.add_recv_hook(self.addr, self.recv)

    def recv(self, data):
        return bytes(0)


class LCD(I2CSubPeripheral):

    class Inst(Enum):
        Extended = 0
        Standard = 1

    def __init__(self, addr, i2cbus):
        super(LCD, self).__init__(addr, i2cbus)
        self.power_on = False
        self.cursor_on = False
        self.cursor_blink = False
        self.buffer = [" "] * 32
        self.pos = 0
        self.write_step = 1
        self.instructions = LCD.Inst.Standard

    def decode_byte(self, b):
        if any([
            b >= 160 and b <= 191,
            b >= 193 and b <= 218,
            b >= 225 and b <= 250
            ]):
            return chr(b - 128)
        return '\uFFFD'

    def get_text(self):
        if not self.power_on:
            return (" " * 32) + "\n" + (" " * 32)
        lined = "".join(self.buffer[0:16]) + "\n" + "".join(self.buffer[16:32])
        return lined

    def write_byte(self, d):
        if self.pos in range(0,16):
            self.buffer[self.pos] = d
        if self.pos in range(0x40,0x40+16):
            self.buffer[(self.pos-0x40) + 16] = d
        self.pos += self.write_step

    def recv(self, data):
        mode = data[0]
        if mode == 0x40:
            for d in data[1:]:
                self.write_byte(self.decode_byte(d))
        elif mode == 0:
            for cmd in data[1:]:
                # ignore extended mode functions
                if self.instructions == LCD.Inst.Extended:
                    if not any([(cmd & 0xe0) == 0x20, cmd == 0]):
                        continue
                if cmd == 0:
                    return # NOP
                elif cmd < 2: # Clear
                    self.buffer = [">"] * 32
                elif cmd < 4: # Return home
                    self.pos = 0
                elif cmd < 8: # inc/dec mode and shift mode
                    if cmd & 1 != 0:
                        raise Exception("LCD Shift mode not implemented")
                    if cmd & 2 == 0:
                        self.write_step = -1
                    else:
                        self.write_step = 1
                elif cmd < 16:
                    self.power_on = (cmd & 4) > 0
                    self.cursor_on = (cmd & 2) > 0
                    self.cursor_blink = (cmd & 1) > 0
                elif cmd < 32: # cursor modes
                    if cmd & 8 != 0:
                        raise Exception("LCD shift mode not implemented")
                elif cmd < 64: # function set
                    if cmd & 1 == 0:
                        self.instructions = LCD.Inst.Standard
                    else:
                        self.instructions = LCD.Inst.Extended
                    if cmd & 2 != 0:
                        raise Exception("LCD multiplex mode incorrect")
                    if cmd & 4 == 0:
                        raise Exception("LCD must be used in 2x16 mode")
                    if cmd & 16 == 0:
                        raise Exception("LCD must be used in 8 bit mode")
                elif cmd < 128: # cgram set
                    raise Exception("LCD CGRam not implemented")
                elif cmd < 256: # dram set
                    self.pos = cmd - 128
        return bytes(0)
        


class Keypad(I2CSubPeripheral):

    KEYS = [
        #['1','2','3','A'],
        #['4','5','6','B'],
        #['7','8','9','C'],
        #['*','0','#','D']

        ['1','4','7','*'],
        ['2','5','8','0'],
        ['3','6','9','#'],
        ['A','B','C','D']

    ]

    def __init__(self, addr, i2cbus, key_query):
        super(Keypad, self).__init__(addr, i2cbus)
        self.key_query = key_query
 
    def recv(self, data):
        if len(data) > 0:
            self.outputs = data[0]
        held_key = self.key_query()
        if held_key is None:
            return bytes([0x0f | self.outputs])
        else:
            lines = [l == "0" for l in format(self.outputs >> 4, "04b")]
            check_keys = [row for row, do_check in zip(Keypad.KEYS, lines) if do_check]
            for check in check_keys:
                for i, key in enumerate(check):
                    if key == held_key:
                        return bytes([(self.outputs & 0xf0) | (~(1<<(3-i))) & 0x0f])

        # This shouldn't be reached normally
        return bytes([0x0f | self.outputs])
