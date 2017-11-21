function [ result ] = Hj( feature, theta, p )
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here
% if feature > 0
    if p*feature < p*theta
        result = 1;
    else
        result = 0;
    end;
% else
%     if feature > theta
%         result = 1;
%     else
%         result = 0;
%     end;
% end;

end

