function [ FPR, TPR ] = cascade_roc( rocList, y, theta, total )
% FPR과 TPR을 구하는 roc함수

Tp = 0; Tn = 0; Fp = 0; Fn = 0;
% count = 0;

for i=1 : total
    if sum(rocList(i)) >= theta
        classify(i) = 1;
    else
        classify(i) = 0;
    end
end
for i=1 : total
    if classify(i) == 1
        if y(i) == 1
            Tp = Tp+1;
        else
            Fp = Fp+1;
%             count = count+1;
%             false_detection_num(count) = i;
        end
    else
        if y(i) == 0
            Tn = Tn+1;
        else
            Fn = Fn+1;
        end
    end
end

TPR = Tp/(Tp+Fn);
% if Tn+Fp == 0
%     FPR = 0;
% else
%     FPR = 1-(Tn/(Tn+Fp));
FPR = Fp/(Fp+Tn);
% end

end

