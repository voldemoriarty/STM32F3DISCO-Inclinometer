function update_figure(cfg, t, data)
%update_figure plot data on figure
    window_size = 30;

    % if figure was closed, return
    if ~ishandle(cfg.acc_ax)
        return
    end

%% Accelerometer plots
    
    T = t(end);
    y = reshape([data.acc]',3,[]);
    for i = 1:3
        set(cfg.acc_m(i), 'XData', T, 'YData', y(i,end));
        addpoints(cfg.acc_al(i), t, y(i,:));
    end

    % lower limit is always >= 0
    % upper limit is always >= window_size
    set(cfg.acc_ax, 'XLim', [max(0, T-window_size), max(window_size, T)]);

%% Angle plots
    
    theta = [data.theta];
    phi = [data.phi];
    set(cfg.ang_m(1), 'XData', T, 'YData', theta(end));
    addpoints(cfg.ang_al(1), t, theta);
    set(cfg.ang_m(2), 'XData', T, 'YData', phi(end));
    addpoints(cfg.ang_al(2), t, phi);

    % lower limit is always >= 0
    % upper limit is always >= window_size
    set(cfg.ang_ax, 'XLim', [max(0, T-window_size), max(window_size, T)]);
    drawnow limitrate
end