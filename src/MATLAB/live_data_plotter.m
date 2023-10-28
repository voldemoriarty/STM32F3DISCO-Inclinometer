clearvars

device = serialport('COM3', 921600, 'ByteOrder', 'little-endian');

bufsize = 4;
preamblesize = 4;

buffer   = uint8(zeros(1,bufsize));
preamble = uint8([0x69, 0x42, 0x69, 0x42]);

device.flush();

rdbuf = device.read(4, 'uint8');

% align data using preamble

while any(preamble ~= rdbuf(1:preamblesize))
    buffer = [buffer(2:end), device.read(1, 'uint8')];
end

data = read_data(device)