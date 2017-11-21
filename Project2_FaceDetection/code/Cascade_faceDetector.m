clc; clear all; close all;

load('Cascade_Parameter7.mat');
image = imread('3.jpg');
gray_image = rgb2gray(image);
[H, W] = size(gray_image);
[h, w] = size(gray_image);

count = 1;
while(1)
    if h*0.8<19 || w*0.8<19
        break;
    end
    h=h*0.8;
    w=w*0.8;
    count = count+1;
end
[h, w] = size(gray_image);

img{1} = gray_image;
for i=2 : count
    img{i} = imresize(img{i-1}, [h*0.8,w*0.8]);
    h = h*0.8;
    w = w*0.8;
end
featureNumArr = compute_feature_num(gray_image(1:19,1:19));
disp('세팅 완료');

%% 얼굴 검출 시작
faceNum_sum = 0;
result = 0;
for windowSize=1 : count
    faceNum(windowSize) = 0;
    table_h = H*(0.8^(windowSize-1));
    table_w = W*(0.8^(windowSize-1));
    for window_y=1 : table_h-19
        for window_x=1 : table_w-19
            img_ready = img{windowSize}(window_y:(window_y+18), window_x:(window_x+18));
            integral_image = cumsum(cumsum(double(img_ready),2));
            for i=1 : size(n,2)
                for k=1 : n(i)
                    address_H{i}(k) = featureNumArr(featureNum{i}(k));
                    x = address_H{i}(k).x;
                    y = address_H{i}(k).y;
                    w = address_H{i}(k).w;
                    h = address_H{i}(k).h;
                    number = address_H{i}(k).num;
                    if number == 1
                        weakClassifier(i) = -compute( integral_image, x+w,y, w,h ) + compute( integral_image, x,y, w,h );
                    elseif number == 2
                        weakClassifier(i) = -compute( integral_image, x,y+h, w,h) + compute( integral_image, x,y, w,h);
                    elseif number == 3
                        weakClassifier(i) = compute( integral_image, x,y, w,h) - compute( integral_image, x+w,y, w,h) + compute( integral_image, x+(2*w),y, w,h);
                    elseif number == 4
                        weakClassifier(i) = compute( integral_image, x,y, w,h) - compute( integral_image, x,y+h, w,h) + compute( integral_image, x,y+(2*h), w,h);
                    elseif number == 5
                        weakClassifier(i) = compute( integral_image, x,y, w,h) - compute( integral_image, x+w, y, w,h) - compute( integral_image, x,y+h, w,h) + compute( integral_image, x+w,y+h, w,h);
                    end
                    classifier_h{i}(k) = Hj( weakClassifier(i), theta{i}(k), parity{i}(featureNum{i}(k)));
                end
                
                if sum(alpha{i}.*classifier_h{i}) >= sum(alpha{i})/2+2
                    strongClassifier(i) = 1;
                    %                     result = result + sum(alpha{i}.*classifier_h{i});
                else
                    strongClassifier = 0;
                    result = 0;
                    break;
                end
            end
            if strongClassifier == 1
                faceNum(windowSize) = faceNum(windowSize)+1;
                face(windowSize, faceNum(windowSize)).x = window_x;
                face(windowSize, faceNum(windowSize)).y = window_y;
                face(windowSize, faceNum(windowSize)).w = table_w;
                face(windowSize, faceNum(windowSize)).h = table_h;
                %                 face(windowSize, faceNum(windowSize)).confidence = result;
                face(windowSize, faceNum(windowSize)).confidence = sum(alpha{i}.*classifier_h{i});
                faceNum_sum = faceNum_sum+1;
                result = 0;
            end
        end
    end
    fprintf('%d번째 이미지 완료\n',windowSize);
end
disp('strongClassifier 생성 성공');

n = 0;
pass = 0;
for windowSize=1 : count
    if faceNum_sum > 0
        if faceNum(windowSize) >0
            for i=1 : faceNum(windowSize)
                if n>0
                    for m=1 : n
%                         if round(face(windowSize, i).y*(1.25^(windowSize-1))) <= FaceList{m}(1)+(1.25^(FaceList{m}(6)-1))*19*0.7 && round(face(windowSize, i).x*(1.25^(windowSize-1))) <= FaceList{m}(2)+(1.25^(FaceList{m}(6)-1))*19*0.7 && round((face(windowSize, i).y+19)*(1.25^(windowSize-1))) >= FaceList{m}(3)-(1.25^(FaceList{m}(6)-1))*19*0.7 && round((face(windowSize, i).x+19)*(1.25^(windowSize-1))) >= FaceList{m}(4)-(1.25^(FaceList{m}(6)-1))*19*0.7
if round(face(windowSize, i).y*(1.25^(windowSize-1))) <= FaceList{m}(1)+(1.25^(FaceList{m}(6)))*19*0.9 && round(face(windowSize, i).x*(1.25^(windowSize-1))) <= FaceList{m}(2)+(1.25^(FaceList{m}(6)))*19*0.9 && round((face(windowSize, i).y+19)*(1.25^(windowSize-1))) >= FaceList{m}(3)-(1.25^(FaceList{m}(6)))*19*0.9 && round((face(windowSize, i).x+19)*(1.25^(windowSize-1))) >= FaceList{m}(4)-(1.25^(FaceList{m}(6)))*19*0.9
                            if face(windowSize, i).confidence <= FaceList{m}(5)
                                pass = pass+1;
                            else
                                FaceList{m}(1) = round(face(windowSize, i).y*(1.25^(windowSize-1)));
                                FaceList{m}(2) = round(face(windowSize, i).x*(1.25^(windowSize-1)));
                                FaceList{m}(3) = round((face(windowSize, i).y+19)*(1.25^(windowSize-1)));
                                FaceList{m}(4) = round((face(windowSize, i).x+19)*(1.25^(windowSize-1)));
                                FaceList{m}(5) = face(windowSize, i).confidence;
                                FaceList{m}(6) = windowSize;
                                pass = pass+1;
                            end
                        end
                    end
                end
                
                if pass == 0
                    n = n+1;
                    FaceList{n}(1) = round(face(windowSize, i).y*(1.25^(windowSize-1)));
                    FaceList{n}(2) = round(face(windowSize, i).x*(1.25^(windowSize-1)));
                    FaceList{n}(3) = round((face(windowSize, i).y+19)*(1.25^(windowSize-1)));
                    FaceList{n}(4) = round((face(windowSize, i).x+19)*(1.25^(windowSize-1)));
                    FaceList{n}(5) = face(windowSize, i).confidence;
                    FaceList{n}(6) = windowSize;
                end
                pass = 0;
            end
        end
    end
end
disp('얼굴 검출 완료');

for i=1 : n
    image(FaceList{i}(1), FaceList{i}(2) : FaceList{i}(4), 1 ) = 255;
    image(FaceList{i}(1), FaceList{i}(2) : FaceList{i}(4), 2:3 ) = 0;
    
    image(FaceList{i}(3), FaceList{i}(2) : FaceList{i}(4), 1 ) = 255;
    image(FaceList{i}(3), FaceList{i}(2) : FaceList{i}(4), 2:3 ) = 0;
    
    image(FaceList{i}(1) : FaceList{i}(3), FaceList{i}(2), 1 ) = 255;
    image(FaceList{i}(1) : FaceList{i}(3), FaceList{i}(2), 2:3 ) = 0;
    
    image(FaceList{i}(1) : FaceList{i}(3), FaceList{i}(4), 1 ) = 255;
    image(FaceList{i}(1) : FaceList{i}(3), FaceList{i}(4), 2:3 ) = 0;
end
figure,imshow(image);