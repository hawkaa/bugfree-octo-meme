function [ BW ] = RGBDistanceThreshold(I, Avg, Std, t)
    RGBD = RGBDistance(I, Avg, Std);
    [r,c] = size(RGBD);
    BW = zeros(r, c);
    for y = 1:r
        for x = 1:c
            if RGBD(y,x) < t
                BW(y,x) = 255;
            end
        end
    end
end

