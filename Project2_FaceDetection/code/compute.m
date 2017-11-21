function [ result ] = compute( integral_image,x,y,w,h )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
if y==2
    if x==2
        result = integral_image(y+h-1,x+w-1);
    else
        result = integral_image(y+h-1,x+w-1) - integral_image(y+h-1,x-1);
    end
else
    if x==2
        result = integral_image(y+h-1,x+w-1) - integral_image(y-1,x+w-1);
    else
        result = integral_image(y+h-1,x+w-1) - integral_image(y+h-1,x-1) - integral_image(y-1,x+w-1) + integral_image(y-1,x-1);
    end
end

end