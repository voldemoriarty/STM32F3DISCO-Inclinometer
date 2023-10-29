% x right
% y back
% z up

clearvars;
serial_port   = 'COM3';
baudrate      = 921600;
preamble      = uint8([0x69, 0x42, 0x69, 0x42]);
bufsize       = 5;
t             = zeros(1,bufsize);

h = figure(1);
cfg = init_figure(h);

% open serial port 
device = serialport(serial_port, baudrate);

% align data using preamble
info('Syncing with device');
sync_preamble(device, preamble);
info('Synced');

% read data 
idx = 1;
data(bufsize) = read_data(device);
data(1) = data(bufsize);
T0 = datetime;

while ishandle(h)
    if (idx == bufsize)
        % plot data
        update_figure(cfg, t, data);
        idx = 0;
    end
    
    % get new data from device
    idx = idx + 1;
    data(idx) = read_data_preamb(device);
    t(idx) = seconds(datetime - T0);
end

info('Figure closed');
clear device