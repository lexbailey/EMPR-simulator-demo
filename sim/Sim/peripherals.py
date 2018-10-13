from enum import Enum

class CommandWrapper:
    """
        Used by the 'command' decorator for command functions on 'Peripheral'
    """
    def __call__(self, funcself, string):
        args=string.split(" ")
        return self.function(funcself, *args)

    def __init__(self, name):
        self.name = name

    def get_name(self):
        return self.name

    def wrap(self, function):
        self.function = function
        return self

class Peripheral():
    """
        Base class for all peripherals
    """
    def __init__(self):
        self._commands = {}
        self._init_commands()

    @classmethod
    def command(cls, name):
        """
            Decorates a peripheral function, so that it is called by do_command
            when the specified command is received
        """
        return CommandWrapper(name).wrap

    def _init_commands(self):
        for func_name in dir(self):
            wrapper = getattr(self, func_name)
            if isinstance(wrapper, CommandWrapper):
                self._commands[wrapper.get_name()] = func_name

    def get_command_base_name(self):
        return "BasePeripheral"

    def do_command(self, command):
        name, args = command.split(" ", maxsplit=1)
        try:
            func_name = self._commands[name]
        except KeyError:
            print("Error: No such command '%s' for peripheral '%s'" % (command, self.get_command_base_name()))
            return
        try:
            reply = getattr(self, func_name)(self, args)
            return reply
        except:
            print("Unexpected error when calling command function for '%s'" % (command))
            raise

class PinMode(Enum):
    PULLUP = 0
    TRISTATE = 2
    PULLDOWN = 3

class Pin():
    def __init__(self, number):
        self.number = number
        self.functions = [None] * 4
        self.mode = PinMode.PULLUP
        self.func_num = 0
        self.open_drain = False
        self.is_configured = False
        self._name = "Unknown"

    def set_function(self, num, function):
        self.functions[num] = function

    def set_id(self, pin_id):
        self._pin_id = pin_id

    def set_name(self, name):
        self._name = name

    def set_func_num(self, func_num):
        if func_num not in range(0,4):
            print("Error: Set func num on pin %d to %d. Must be in range 0-3" % (self.number, func_num))
            return
        self.func_num = func_num
        self.is_configured = True

    def set_open_drain(self, open_drain):
        if open_drain not in range(0,2):
            print("Error: Set open drain on pin %d to %d. Must be in range 0-1" % (self.number, open_drain))
            return
        self.open_drain = open_drain == 1

    def set_mode(self, mode):
        try:
            self.mode = PinMode(mode)
        except ValueError:
            print("Error: Set mode on pin %d to %d. Not a valid mode value" % (self.number, mode))

    def get_is_configured(self):
        return self.is_configured

    def describe(self):
        return "%s: %s - Mode:%s%s" % (self._name, self.functions[self.func_num], self.mode.name, " OD" if self.open_drain else "")

class Pinsel(Peripheral):

    pin_map = {
        # P0
        "P0.0": 46,
        "P0.1": 47,
        "P0.2": 98,
        "P0.3": 99,
        "P0.4": 81,
        "P0.5": 80,
        "P0.6": 79,
        "P0.7": 78,
        "P0.8": 77,
        "P0.9": 76,
        "P0.10": 48,
        "P0.11": 49,
        # 12..14
        "P0.15": 62,
        "P0.16": 63,
        "P0.17": 61,
        "P0.18": 60,
        "P0.19": 59,
        "P0.20": 58,
        "P0.21": 57,
        "P0.22": 56,
        "P0.23": 9,
        "P0.24": 8,
        "P0.25": 7,
        "P0.26": 6,
        "P0.27": 25,
        "P0.28": 24,
        "P0.29": 29,
        "P0.30": 30,
        # 31

        # P1
        "P1.0": 95,
        "P1.1": 94,
        # 2..3
        "P1.4": 93,
        # 5..7
        "P1.8": 92,
        "P1.9": 91,
        "P1.10": 90,
        # 11..13
        "P1.14": 89,
        "P1.15": 88,
        "P1.16": 87,
        "P1.17": 86,
        "P1.18": 32,
        "P1.19": 33,
        "P1.20": 34,
        "P1.21": 35,
        "P1.22": 36,
        "P1.23": 37,
        "P1.24": 38,
        "P1.25": 39,
        "P1.26": 40,
        "P1.27": 43,
        "P1.28": 44,
        "P1.29": 45,
        "P1.30": 21,
        "P1.31": 20,

        # P2
        "P2.0": 75,
        "P2.1": 74,
        "P2.2": 73,
        "P2.3": 70,
        "P2.4": 69,
        "P2.5": 68,
        "P2.6": 67,
        "P2.7": 66,
        "P2.8": 65,
        "P2.9": 64,
        "P2.10": 53,
        "P2.11": 52,
        "P2.12": 51,
        "P2.13": 50,
        # 14..31

        # P3
        # 0..24
        "P3.25": 27,
        "P3.26": 26,
        # 27..31

        # P4
        # 0..27
        "P4.28": 82,
        # 29..31

    }

    serial_map = {
        "TXD0": [(98,1)],
        "RXD0": [(99,1)],
        "TXD1": [(75,2),(62,1)],
        "RXD1": [(74,2),(63,1)],

        "TXD2": [(48,1), (65,2)],
        "RXD2": [(49,1), (64,2)],

        "TXD3": [(46,2), (82,3)],
        "RXD3": [(47,2), (85,3)],
    }

    i2c_map = {
        "SDA0": [(25,1)],
        "SCL0": [(24,1)],
        "SDA1": [(59,3),(46,3)],
        "SCL1": [(58,2),(47,3)],
        "SDA2": [(48,2)],
        "SCL2": [(49,2)],
    }

    func_maps = [
        serial_map,
        i2c_map
    ]

    def __init__(self):
        super(Pinsel, self).__init__()
        self.pins = []
        for i in range(100):
            self.pins.append(Pin(i+1))
        for pin_name, pin_number in Pinsel.pin_map.items():
            pin_number -= 1
            self.pins[pin_number].set_name(pin_name)
            self.pins[pin_number].set_function(0, "GPIO")
        for f_map in Pinsel.func_maps:
            for function, pin_func_nums in f_map.items():
                for pin_num, func_num in pin_func_nums:
                    self.pins[pin_num-1].set_function(func_num, function)

    def get_command_base_name(self):
        return "pinsel"

    def describe_pins(self):
        output = []
        for pin in self.pins:
            if pin.get_is_configured():
                output.append(pin.describe())
        return "\n".join(output)
            

    @Peripheral.command("configpin")
    def _configpin(self, portnum, pinnum, pinmode, funcnum, open_drain):
        pinname = "P%d.%d" % (int(portnum), int(pinnum))
        try:
            pin_index = Pinsel.pin_map[pinname]-1
            self.pins[pin_index].set_func_num(int(funcnum))
            self.pins[pin_index].set_mode(int(pinmode))
            self.pins[pin_index].set_open_drain(int(open_drain))
        except KeyError:
            print("Error: No such pin: %s" % (pinname))

class NoSuchI2C(Exception):
    pass

class I2CBus():
    def __init__(self):
        self.init = False
        self.enabled = False
        self._clock_rate = 0
        self.recv_hooks = {}

    def describe(self):
        if not self.init:
            return None
        return "%s (%dhz)" % ("Enabled" if self.enabled else "disabled", self.clock_rate)

    @property
    def clock_rate(self):
        return self._clock_rate

    @clock_rate.setter
    def clock_rate(self, clock_rate):
        self._clock_rate = int(clock_rate)

    def add_recv_hook(self, addr, callback):
        self.recv_hooks[addr] = callback

    def recv(self, addr, data):
        if not all([self.init, self.enabled]):
            print("Warning: send to uninitialised or disabled I2C device")
            return
        try:
            reply = self.recv_hooks[addr](data)
            return reply
        except KeyError:
            pass

class I2C(Peripheral):

    def __init__(self):
        super(I2C, self).__init__()
        self.buses = {}
        for i in range(3):
            self.buses["i2c%d" % (i)] = I2CBus()

    def get_bus(self, name):
        return self.buses[name]

    def get_command_base_name(self):
        return "i2c"

    def check_bus(self, i2c_dev):
        if i2c_dev not in self.buses:
            raise NoSuchI2C()

    def do_command(self, command):
        try:
            return super(I2C, self).do_command(command)
        except NoSuchI2C:
            print("Error: no such i2c bus: %s" % (i2c_dev))

    def describe_state(self):
        return "\n".join(name + ": " + bus.describe() for name, bus in self.buses.items() if bus.describe())

    @Peripheral.command("init")
    def _i2c_init(self, i2c_dev, clock_rate):
        self.check_bus(i2c_dev)
        self.buses[i2c_dev].init = True
        self.buses[i2c_dev].clock_rate = clock_rate


    @Peripheral.command("cmd")
    def _i2c_cmd(self, i2c_dev, enabled):
        self.check_bus(i2c_dev)
        self.buses[i2c_dev].enabled = enabled == "enable"
            

    @Peripheral.command("mastertransferdata")
    def _i2c_m_trans(self, i2c_dev, addr, data):
        def chunks(l, n):
            """Yield successive n-sized chunks from l."""
            for i in range(0, len(l), n):
                yield l[i:i + n]
        self.check_bus(i2c_dev)
        bus = self.buses[i2c_dev]
        if not all([bus.init, bus.enabled]):
            print("Error: attempted to transfer data using i2c bus '%s', without init and enable" % (i2c_dev))
        else:
            reply = bus.recv(int(addr, 16), bytearray(int(chunk, 16) for chunk in chunks(data, 2)))
            return reply
