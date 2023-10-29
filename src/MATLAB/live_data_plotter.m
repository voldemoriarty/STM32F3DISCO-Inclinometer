% x right
% y back
% z up

clearvars

device = serialport('COM3', 115200, 'ByteOrder', 'little-endian');

preamblesize = 4;
preamble = uint8([0x69, 0x42, 0x69, 0x42]);
h = figure(1);
ax = gca;
hold(ax, 'on')
grid(ax, 'on')

al(1) = animatedline(ax, 'Color', 'r', 'LineWidth', 1.5);
al(2) = animatedline(ax, 'Color', 'b', 'LineWidth', 1.5);
al(3) = animatedline(ax, 'Color', 'g', 'LineWidth', 1.5);
m(1) = plot(ax, 0, 0, 'o', 'MarkerFaceColor','r');
m(2) = plot(ax, 0, 0, 'o', 'MarkerFaceColor','b');
m(3) = plot(ax, 0, 0, 'o', 'MarkerFaceColor','g');

% align data using preamble
disp('Syncing with device');
device.flush();
rdbuf = device.read(4, 'uint8');
while any(preamble ~= rdbuf(1:preamblesize))
    rdbuf = [rdbuf(2:end), device.read(1, 'uint8')];
end
disp('Synced');

data = read_data(device);
T0 = datetime;

while true
    x = seconds(datetime - T0);
    y = data.acc;

    
    data = read_data_preamb(device);

    m(1).XData = x; m(1).YData = y(1);
    m(2).XData = x; m(2).YData = y(2);
    m(3).XData = x; m(3).YData = y(3);

    addpoints(al(1), x, y(1));
    addpoints(al(2), x, y(2));
    addpoints(al(3), x, y(3));

    xlim(ax, max(0,[x-10, x]));
    drawnow limitrate nocallbacks 
end