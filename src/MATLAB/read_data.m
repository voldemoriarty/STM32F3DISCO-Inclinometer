function data = read_data(device)
%READ_DATA Read data from serial into a struct
    data = struct();
    
    % data from device
    data.acc = device.read(3, 'int16')*1e-3;
    data.mag = device.read(3, 'int16');
    data.acc_temp = device.read(1, 'int16');
    data.gyro = device.read(3, 'int16')*1e-3;
    data.gyro_temp = device.read(1, 'int16');
    data.loop_time = device.read(1, 'uint32');

    % post process
    acc = data.acc;
    [az, el, ~] = cart2sph(acc(3), acc(2), acc(1));
    data.theta = rad2deg(az);
    data.phi = rad2deg(el);
end

