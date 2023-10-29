function data = read_data_preamb(device)
%READ_DATA Read data from serial into a struct with preamble
    device.read(4, 'uint8');
    data = read_data(device);
end

