function cfg = init_figure(h)
%init_figure Initialize a figure for plotting
%   Detailed explanation goes here

lw = 1.5;    % line width
ms = 7.5;    % marker size

set(h,'Name','Live Data');
clf(h, 'reset');

%% Accelerometer Plots
    acc_ax = subplot(1,2,1);
    hold(acc_ax, 'on');
    grid(acc_ax, 'on');
    title(acc_ax, '\textbf{Accelerometer Readings}', 'Interpreter', 'latex');
    ylabel(acc_ax, '$m/s^2$', 'Interpreter', 'latex');
    xlabel(acc_ax, '$t$ (s)', 'Interpreter', 'latex');
    acc_al(1) = animatedline(acc_ax, 'Color', 'b', 'LineWidth', lw, 'DisplayName', '$acc_x$');
    acc_al(2) = animatedline(acc_ax, 'Color', 'r', 'LineWidth', lw, 'DisplayName', '$acc_y$');
    acc_al(3) = animatedline(acc_ax, 'Color', 'g', 'LineWidth', lw, 'DisplayName', '$acc_z$');
    acc_m(1) = plot(acc_ax, 0, 0, 'o', 'MarkerFaceColor','b', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    acc_m(2) = plot(acc_ax, 0, 0, 'o', 'MarkerFaceColor','r', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    acc_m(3) = plot(acc_ax, 0, 0, 'o', 'MarkerFaceColor','g', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    
    legend(acc_ax, 'show', 'Interpreter', 'latex', 'FontSize', 14, 'location', 'sw');
    set(acc_ax, 'YLim', [-12, 12], 'YLimMode', 'manual');

%% Angle Plots
    ang_ax = subplot(1,2,2);
    hold(ang_ax, 'on');
    grid(ang_ax, 'on');
    title(ang_ax, '\textbf{Angle Estimates}', 'Interpreter', 'latex');
    ylabel(ang_ax, '$\circ$', 'Interpreter', 'latex');
    xlabel(ang_ax, '$t$ (s)', 'Interpreter', 'latex');
    ang_al(1) = animatedline(ang_ax, 'Color', 'b', 'LineWidth', lw, 'DisplayName', '$\theta$');
    ang_al(2) = animatedline(ang_ax, 'Color', 'r', 'LineWidth', lw, 'DisplayName', '$\phi$');
    ang_m(1) = plot(ang_ax, 0, 0, 'o', 'MarkerFaceColor','b', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    ang_m(2) = plot(ang_ax, 0, 0, 'o', 'MarkerFaceColor','r', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    
    legend(ang_ax, 'show', 'Interpreter', 'latex', 'FontSize', 14, 'location', 'sw');
    % set(ang_ax, 'YLim', [-12, 12], 'YLimMode', 'manual');
%%  Output handles

    cfg = struct();
    cfg.acc_al = acc_al;
    cfg.ang_m = ang_m;
    cfg.ang_al = ang_al;
    cfg.acc_m = acc_m;
    cfg.acc_ax = acc_ax;
    cfg.ang_ax = ang_ax;
end