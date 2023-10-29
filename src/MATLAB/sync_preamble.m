function sync_preamble(device, preamble)
%sync_preamble Sync self with device by aligning with preamble
    preamblesize = length(preamble);
    device.flush();
    rdbuf = device.read(preamblesize, 'uint8');
    while any(preamble ~= rdbuf)
        rdbuf = [rdbuf(2:end), device.read(1, 'uint8')];
    end
end