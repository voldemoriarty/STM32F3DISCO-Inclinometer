function update_figure(cfg, t, data)
%update_figure plot data on figure
    window_size = 10;

    % if figure was closed, return
    if ~ishandle(cfg.acc_ax)
        return
    end

%% Accelerometer plots
    for i = 1:3
        y = data.acc(i);
        set(cfg.acc_m(i), 'XData', t, 'YData', y);
        addpoints(cfg.acc_al(i), t, y);
    end

    % lower limit is always >= 0
    % upper limit is always >= window_size
    set(cfg.acc_ax, 'XLim', [max(0, t-window_size), max(window_size, t)]);
    drawnow limitrate
end