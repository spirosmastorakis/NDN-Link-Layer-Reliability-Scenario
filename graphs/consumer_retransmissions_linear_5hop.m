
%linear topology of hop length 5
figure;
pause;
rt_6node_maxRet3 = [234  221  195  194  205  210  216  187  232  233  210  195  217  229  210  233  227  233  211  230;
259  268  263  265  239  254  230  306  246  250  258  264  283  281  244  248  235  250  260  232;
150  153  166  203  176  157  165  136  144  171  141  131  151  129  141  131  159  169  131  153;
80  76  75  81  80  73  85  79  81  96  71  62  66  68  82  76  52  75  76  90;
30  29  34  51  41  39  39  30  35  43  35  30  24  42  47  57  45  46  30  28;
27  17  19  18  17  16  22  17  8  11  13  28  18  17  17  27  26  15  29  19;
];


rt_6node_maxRet2 = [234  221  195  194  205  210  216  187  232  233  210  195  217  229  210  233  227  233  211  230;
259  268  263  265  239  254  230  306  246  250  258  264  283  281  244  248  235  250  224  232;
150  155  150  151  181  144  165  163  167  171  136  177  176  141  135  145  175  182  131  160;
89  74  72  76  83  96  78  76  83  63  65  84  57  76  72  73  83  62  87  90;
62  43  43  45  71  66  50  41  45  51  57  47  52  52  53  60  48  39  60  38;
50  51  40  41  40  37  55  47  50  47  48  40  39  50  50  48  51  42  57  38;
];


rt_6node_maxRet1 = [217  221  210  231  237  204  195  241  223  215  212  201  230  238  213  231  231  261  236  232;
253  274  233  257  270  278  248  260  259  295  266  277  245  295  228  262  271  258  254  295;
242  201  215  214  211  230  200  235  213  237  219  218  212  214  215  204  239  237  241  202;
272  249  258  249  226  250  246  260  239  259  266  242  233  242  244  276  233  225  241  242;
377  353  339  355  344  381  351  345  374  366  382  360  329  352  375  356  347  347  357  357;
517  541  486  499  499  509  515  518  546  489  528  500  512  539  537  554  509  521  522  512;
];

rt_6node_noRe = [1037  1082  1082  975  1008  1071  1048  1047  946  1048  1056  1032  1000  1024  1011  983  1029  1063  1036  992;
2095  2142  2152  1984  2041  2119  2063  2129  2004  2186  2145  2071  2079  2070  2113  2083  2048  2153  2073  2046;
4317  4354  4333  4310  4285  4430  4268  4384  4328  4472  4355  4305  4324  4235  4336  4412  4245  4418  4290  4379;
6850  6852  6661  6760  6663  6879  6684  6812  6768  6874  6744  6737  6801  6580  6716  6840  6714  6785  6707  6758;
9550  9471  9355  9357  9276  9526  9351  9443  9400  9499  9417  9483  9473  9186  9412  9495  9312  9371  9284  9475;
12360  14264  12192  12264  12360  12264  12173  12288  12301  12244  12283  12360  12240  11967  12150  12240  13240  12337  12173  10150;
];

probabilities = [0.005; 0.01; 0.02; 0.03; 0.04; 0.05;];

h = plot	(probabilities, mean(rt_6node_maxRet3,2), 'b.',
			 probabilities, mean(rt_6node_maxRet2,2), 'r.',
			 probabilities, mean(rt_6node_maxRet1,2), 'g.',
			 probabilities, mean(rt_6node_noRe,2), 'r.'
			);

h = errorbar(probabilities, mean(rt_6node_maxRet3,2), ci(rt_6node_maxRet3)(:,1), ci(rt_6node_maxRet3)(:,2), 'm.',
			 probabilities, mean(rt_6node_maxRet2,2), ci(rt_6node_maxRet2)(:,1), ci(rt_6node_maxRet2)(:,2), 'b.',
			 probabilities, mean(rt_6node_maxRet1,2), ci(rt_6node_maxRet1)(:,1), ci(rt_6node_maxRet1)(:,2), 'g.',
			 probabilities, mean(rt_6node_noRe,2), ci(rt_6node_noRe)(:,1), ci(rt_6node_noRe)(:,2), 'r.'
			);

set(h,'markersize',20);
axis([0 0.05 0 13000]);

ax = gca();
set(ax, 'fontsize', 22);
set(ax,'YGrid','on');

b=legend('Max Retransmissions = 3', 'Max Retransmissions = 2', 'Max Retransmissions = 1', 'Only end-to-end reliability');
set(b, 'Location', 'east');

title('Consumer Retransmissions');
xlabel('Error probability of links');
ylabel('Consumer Retransmissions');

p = get(gca, 'Position');
ax2 = axes('Position',[p(1)+0.05 p(2)+0.4 p(3)-0.42 p(4)-0.42]);

h2 = plot(ax2, probabilities, mean(rt_6node_maxRet3,2), 'm.',
		 probabilities, mean(rt_6node_maxRet2,2), 'b.',
		 probabilities, mean(rt_6node_maxRet1,2), 'g.');

h2 = errorbar(ax2, probabilities, mean(rt_6node_maxRet3,2), ci(rt_6node_maxRet3)(:,1), ci(rt_6node_maxRet3)(:,2), 'm.',
			 probabilities, mean(rt_6node_maxRet2,2), ci(rt_6node_maxRet2)(:,1), ci(rt_6node_maxRet2)(:,2), 'b.',
			 probabilities, mean(rt_6node_maxRet1,2), ci(rt_6node_maxRet1)(:,1), ci(rt_6node_maxRet1)(:,2), 'g.');

set(h2,'markersize',15);
xlim(ax2,[0 0.05]);
