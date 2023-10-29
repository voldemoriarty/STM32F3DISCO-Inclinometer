function update_figure(cfg, t, data)
%update_figure plot data on figure

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

    set(cfg.acc_ax, 'XLim', max(0,[t-10, t]));
    drawnow limitrate
end