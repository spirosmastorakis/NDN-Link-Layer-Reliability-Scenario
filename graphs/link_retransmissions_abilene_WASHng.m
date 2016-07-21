
%link retransmissions for abilene topology for WASHng at WASHng-ATLAng link

rt_WASHng_maxRet3 = [150  154  169  147  144  179  153  143  155  137  151  139  172  137  150  163  171  181  151  155;
					307  292  321  310  309  318  310  273  319  296  307  303  315  279  300  312  314  328  290  319;
					615  604  598  624  610  621  610  578  608  574  626  640  628  606  596  596  662  658  565  640;
					929  893  889  931  915  944  930  865  910  908  925  964  939  930  908  923  984  979  863  957;
					1251  1214  1203  1277  1238  1283  1280  1229  1290  1220  1284  1273  1243  1227  1210  1253  1315  1299  1188  1281;
					1614  1599  1528  1589  1542  1633  1603  1546  1549  1569  1588  1591  1586  1563  1554  1612  1677  1646  1540  1628;
					]/300;

rt_WASHng_maxRet2 = [150  154  169  147  144  179  153  143  155  137  151  138  172  137  150  163  171  181  151  155;
					310  291  323  310  313  318  310  273  319  296  307  303  315  279  300  312  314  327  289  318;
					615  604  599  616  612  619  598  575  610  583  617  639  628  607  602  603  644  649  559  645;
					942  909  885  925  917  953  931  857  922  909  935  950  940  924  910  929  970  979  858  959;
					1265  1209  1188  1266  1223  1273  1256  1205  1247  1235  1266  1263  1236  1241  1228  1239  1301  1295  1186  1268;
					1612  1566  1523  1595  1571  1591  1622  1565  1533  1556  1596  1607  1571  1577  1545  1599  1668  1620  1560  1630;
					]/300;

rt_WASHng_maxRet1 = [150  155  166  149  143  175  153  144  152  137  150  137  171  135  153  161  170  178  149  155;
					310  296  322  303  314  316  312  270  309  294  305  300  308  277  302  300  300  321  287  315;
					612  606  586  613  599  615  586  573  594  567  603  640  617  596  593  596  645  634  555  620;
					910  881  859  896  868  913  892  853  899  877  885  942  901  903  890  895  939  960  950  923;
					1226  1181  1161  1233  1185  1236  1224  1173  1210  1181  1239  1230  1197  1167  1207  1184  1260  1250  1142  1232;
					1545  1504  1458  1500  1510  1534  1529  1483  1490  1490  1497  1538  1507  1496  1509  1565  1598  1557  1485  1548;
					]/300;


probabilities = [0.005; 0.01; 0.02; 0.03; 0.04; 0.05;];

h = plot	(probabilities, mean(rt_WASHng_maxRet3,2), 'm.',
			 probabilities, mean(rt_WASHng_maxRet2,2), 'b.',
			 probabilities, mean(rt_WASHng_maxRet1,2), 'g.'
			);

h = errorbar(probabilities, mean(rt_WASHng_maxRet3,2), ci(rt_WASHng_maxRet3)(:,1), ci(rt_WASHng_maxRet3)(:,2), 'm.',
			 probabilities, mean(rt_WASHng_maxRet2,2), ci(rt_WASHng_maxRet2)(:,1), ci(rt_WASHng_maxRet2)(:,2), 'b.',
			 probabilities, mean(rt_WASHng_maxRet1,2), ci(rt_WASHng_maxRet1)(:,1), ci(rt_WASHng_maxRet1)(:,2), 'g.'
			);

set(h,'markersize',20);
axis([0 0.05 0 6]);

ax = gca();
set(ax, 'fontsize', 22);
set(ax,'YGrid','on');

b=legend('Max Retransmissions = 3', 'Max Retransmissions = 2', 'Max Retransmissions = 1');
set(b, 'Location', 'northwest');

title('Link layer retransmissions for WASHng at WASHng-ATLAng link');
xlabel('Error probability of links');
ylabel('Percent of link layer retransmissions');