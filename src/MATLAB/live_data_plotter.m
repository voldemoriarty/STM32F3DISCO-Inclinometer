% x right
% y back
% z up

clearvars;
serial_port  = 'COM3';
baudrate     = 115200;
preamble     = uint8([0x69, 0x42, 0x69, 0x42]);

h = figure(1);
cfg = init_figure(h);

% open serial port 
device = serialport(serial_port, baudrate);

% align data using preamble
info('Syncing with device');
sync_preamble(device, preamble);
info('Synced');

% read data 
data = read_data(device);
T0 = datetime;
t = 0;

while ishandle(h)
    % plot data
    update_figure(cfg, t, data);

    % get new data from device
    data = read_data_preamb(device);
    t = seconds(datetime - T0);
end

info('Figure closed');