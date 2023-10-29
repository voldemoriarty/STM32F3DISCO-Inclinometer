function cfg = init_figure(h)
%init_figure Initialize a figure for plotting
%   Detailed explanation goes here

lw = 1.5;    % line width
ms = 7.5;    % marker size

set(h,'Name','Live Data');
clf(h, 'reset');

%% Accelerometer Plots
    acc_ax = axes;
    hold(acc_ax, 'on');
    grid(acc_ax, 'on');
    title(acc_ax, '\textbf{Accelerometer Readings}', 'Interpreter', 'latex');
    ylabel(acc_ax, '$m/s^2$', 'Interpreter', 'latex');
    xlabel(acc_ax, '$t$ (s)', 'Interpreter', 'latex');
    al(1) = animatedline(acc_ax, 'Color', 'b', 'LineWidth', lw, 'DisplayName', '$acc_x$');
    al(2) = animatedline(acc_ax, 'Color', 'r', 'LineWidth', lw, 'DisplayName', '$acc_y$');
    al(3) = animatedline(acc_ax, 'Color', 'g', 'LineWidth', lw, 'DisplayName', '$acc_z$');
    m(1) = plot(acc_ax, 0, 0, 'o', 'MarkerFaceColor','b', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    m(2) = plot(acc_ax, 0, 0, 'o', 'MarkerFaceColor','r', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    m(3) = plot(acc_ax, 0, 0, 'o', 'MarkerFaceColor','g', 'HandleVisibility','off', 'MarkerEdgeColor', 'none', 'MarkerSize', ms);
    
    legend(acc_ax, 'show', 'Interpreter', 'latex', 'FontSize', 14, 'location', 'sw');
    set(acc_ax, 'YLim', [-12, 12], 'YLimMode', 'manual');
%%  Output handles

    cfg = struct();
    cfg.acc_al = al;
    cfg.acc_m = m;
    cfg.acc_ax = acc_ax;
end