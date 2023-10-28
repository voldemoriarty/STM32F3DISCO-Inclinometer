clearvars

device = serialport('COM3', 115200, 'ByteOrder', 'little-endian');

preamblesize = 4;
preamble = uint8([0x69, 0x42, 0x69, 0x42]);

device.flush();

rdbuf = device.read(4, 'uint8');

% align data using preamble
disp('Syncing with device');
while any(preamble ~= rdbuf(1:preamblesize))
    rdbuf = [rdbuf(2:end), device.read(1, 'uint8')];
end
disp('Synced');

data = read_data(device)