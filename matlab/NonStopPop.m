A01 = imread('sweetsA01.png');

colors{1} = {[255 255 0], [50 255;50 255;0 50]}; % Yellow
colors{2} = {[0 255 0], [0 50;50 255;0 50]}; % Green
colors{3} = {[0 0 255], [100 200;140 255;100 200]}; % Blue
colors{4} = {[255 0 0], [50 255;0 50;0 50]}; % Red

SE = strel('disk', 5);


PopTheNonStops(A01, 'test.txt', colors, SE, 'c');
