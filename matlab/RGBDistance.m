function [ out ] = RGBDistance( I, Avg, Std )
    [r,c,d] = size(I);
    out = zeros(r, c);
    for y = 1:r
        for x = 1:c
            out(y, x) = RGBVectorDistance([I(y,x,1) I(y,x,2) I(y,x,3)], Avg, Std);
        end
    end
end

