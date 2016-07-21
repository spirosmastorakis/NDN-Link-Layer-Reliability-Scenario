
data_noRe = csvread('results_linear_5hop_noRe/delays');
data_noTim3 = csvread('results_linear_5hop_3/delays');
%data_noTim2 = csvread('results_linear_5hop_2/delays');
%data_noTim1 = csvread('results_linear_5hop_1/delays');

x = [0:0.1:25 25:0.2:30 30:0.3:700];
cdf_noRe = empirical_cdf(x,data_noRe);
cdf_noTim3 = empirical_cdf(x,data_noTim3);
%cdf_noTim2 = empirical_cdf(x,data_noTim2);
%cdf_noTim1 = empirical_cdf(x,data_noTim1);

cdf_noTim3 *= 100;
%cdf_noTim2 *= 100;
%cdf_noTim1 *= 100;
cdf_noRe *= 100;

h = plot(x,cdf_noTim3,'g',
	     x,cdf_noRe,'r'
	);

set(h, "linewidth", 4);

ax = gca();
set(ax, 'fontsize', 22);

axis([50 700 0 100]);
h=legend('Max Retransmissions = 3', 'Only end-to-end reliability');
set(h, 'Location', 'southeast');

title('CDF of % of interest-data exchanges vs data retrieval time');
ylabel("Percentage of interest-data exchanges");
xlabel("Data retrieval time (in ms)");

grid on;

p = get(gca, 'Position');
ax2 = axes('Position',[p(1)+0.35 p(2)+0.2 p(3)-0.35 p(4)-0.35]);

h2 = plot(x,cdf_noTim3,'g',
	     x,cdf_noRe,'r'
	);
set(h2, "linewidth", 4);
axis(ax2,[40 160 0 100]);
