function [ data_h, count ] = compute_feature_num( image )

[H W] = size(image);
count = 1;
num = 1;
for h=1 : H-2
    for w=1 : (W-2)/2
        for y=2 : H-h
            for x=2 : W-(w*2)
                data_h(count).x = x;
                data_h(count).y = y;
                data_h(count).h = h;
                data_h(count).w = w;
                data_h(count).num = num;
                count = count+1;
            end
        end
    end
end
disp('1 완료');
num = 2;
for h=1 : (H-2)/2
    for w=1 : W-2
        for y=2 : H-(2*h)
            for x=2 : W-w
                data_h(count).x = x;
                data_h(count).y = y;
                data_h(count).h = h;
                data_h(count).w = w;
                data_h(count).count = count;
                data_h(count).num = num;
                count = count+1;
            end
        end
    end
end
disp('2 완료');
num = 3;
for h=1 : H-2
    for w=1 : (W-2)/3
        for y=2 : H-h
            for x=2 : W-(w*3)
                data_h(count).x = x;
                data_h(count).y = y;
                data_h(count).h = h;
                data_h(count).w = w;
                data_h(count).count = count;
                data_h(count).num = num;
                count = count+1;
            end
        end
    end
end
disp('3 완료');
num = 4;
for h=1 : (H-2)/3
    for w=1 : W-2
        for y=2 : H-(h*3)
            for x=2 : W-w
                data_h(count).x = x;
                data_h(count).y = y;
                data_h(count).h = h;
                data_h(count).w = w;
                data_h(count).count = count;
                data_h(count).num = num;
                count = count+1;
            end
        end
    end
end
disp('4 완료');
num = 5;
for h=1 : (H-2)/2
    for w=1 : (W-2)/2
        for y=2 : H-(2*h)
            for x=2 : W-(2*w)
                data_h(count).x = x;
                data_h(count).y = y;
                data_h(count).h = h;
                data_h(count).w = w;
                data_h(count).count = count;
                data_h(count).num = num;
                count = count+1;
            end
        end
    end
end
disp('5 완료');

end

