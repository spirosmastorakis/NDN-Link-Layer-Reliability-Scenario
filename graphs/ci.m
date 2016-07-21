
% function interval = ci (data)
% 	SEM = std(data')'/sqrt(size(data,2));               % Standard Error
% 	ts = tinv([0.05  0.95],size(data,2)-1);     		% T-Score
% 	% interval = repmat(mean(data,2),1,size(ts,2)) + (repmat(ts,size(SEM,1),1).*repmat(SEM,1,size(ts,2)));                   % Confidence Intervals
% 	interval = (repmat(ts,size(SEM,1),1).*repmat(SEM,1,2));
% 	interval (:,1) = -interval(:,1);
% end


function interval = ci (data)
	SEM = std(data')';               		% Standard Error
	ts = tinv([0.05  0.95],size(data,2)-1);     	% T-Score
	% interval = repmat(mean(data,2),1,size(ts,2)) + (repmat(ts,size(SEM,1),1).*repmat(SEM,1,size(ts,2)));                   % Confidence Intervals
	interval = (repmat(ts,size(SEM,1),1).*repmat(SEM,1,2));
	interval (:,1) = -interval(:,1);
end
