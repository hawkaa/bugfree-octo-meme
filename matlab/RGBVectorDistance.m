function [ out ] = RGBVectorDistance( Sample, Avg, Std )
    Var = Std.*Std;
    Diff = abs(double(Sample)-double(Avg));
    NumStd = Diff./Var;
    
    out = sum(NumStd);
end

