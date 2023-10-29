% x right
% y back
% z up

clearvars;
serial_port  = 'COM3';
baudrate     = 115200;
preamble     = uint8([0x69, 0x42, 0x69, 0x42]);
device       = serialport(serial_port, baudrate);

h = figure(1);
cfg = init_figure(h);

% align data using preamble
info('Syncing with device');
sync_preamble(device, preamble);
info('Synced');

data = read_data(device);
T0 = datetime;

while ishandle(h)
    t = seconds(datetime - T0);
    data = read_data_preamb(device);
    update_figure(cfg, t, data);
end

info('Figure closed');