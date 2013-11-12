    function [ BW ] = RGBThreshold(I, RGB)
    [r,c,d] = size(I);
    BW = zeros(r, c);
    for y = 1:r
        for x = 1:c
           r = I(y,x,1);
           g = I(y,x,2);
           b = I(y,x,3);
           if( RGB(1,1) <= r && r < RGB(1,2))
               if(RGB(2,1) <= g && g < RGB(2,2))
                   if(RGB(3,1) <= b && b < RGB(3,2))
                       BW(y, x) = 255;
                   end
               end
           end
        end
    end
    
    
    
end

