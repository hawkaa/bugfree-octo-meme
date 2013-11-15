Yellow = [
    178 134 13;
    177 140 32;
    156 110 5;
    213 172 11;
    211 161 5;
    174 127 1;
    187 148 22;
    162 118 13;
    ];

Green = [
    46 71 46;
    58 83 59;
    35 66 31;
    44 80 41;
    46 70 47;
    24 47 21;
    62 88 65;
    24 44 23;
    ];

Red = [
    190 21 18;
    191 18 15;
    142 25 27;
    140 19 20;
    156 52 55;
    158 42 45;
    100 4 4;
    107 11 9;
    96 4 3;
    107 8 6;
];
M = mean(Red, 1);
S = std(Red,1);


%K = [
%    M(1)-2*S(1) M(1)+2*S(1);
%    M(2)-2*S(2) M(2)+2*S(2);
%    M(3)-2*S(3) M(3)+2*S(3);
%    ];
RGBVectorDistance([190 21 18], M, S);

%Dist = RGBDistance(A01, M, S);


A01 = imread('sweetsA01.png');
A02 = imread('sweetsA02.png');
A03 = imread('sweetsA03.png');

BW01 = imfill(RGBDistanceThreshold(A03, M, S, 0.12));
BW02 = imfill(RGBDistanceThreshold(A01, M, S, 0.12));
BW03 = imfill(RGBDistanceThreshold(A01, M, S, 0.12));

SE = strel('disk',3);
SE2 = strel('disk',3);

BW01_1 = imerode(BW01, SE);
BW01_2 = imdilate(BW01_1, SE);
BW01_3 = imdilate(BW01_2, SE2);
BW01_4 = imerode(BW01_3, SE2);

imshow([BW01_1 BW01_2 BW01_3 BW01_4]);

%imshow([RGBThreshold(A01, K); RGBThreshold(A02, K); RGBThreshold(A03, K)])

%imshow(imfill(RGBDistanceThreshold(A01, M, S, 0.12)));
%figure, imshow(imfill(RGBDistanceThreshold(A02, M, S, 0.12)));
%figure, imshow(imfill(RGBDistanceThreshold(A03, M, S, 0.12)));
    