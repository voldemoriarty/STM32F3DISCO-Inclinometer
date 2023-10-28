function data = read_data(device)
%READ_DATA Read data from serial into a struct
    data = struct();

    data.acc = device.read(3, 'int16')*0.0098;
    data.mag = device.read(3, 'int16');
    data.acc_temp = device.read(1, 'int16');
    data.gyro = device.read(3, 'int16');
    data.gyro_temp = device.read(1, 'int16');
    data.loop_time = device.read(1, 'uint32');
end

