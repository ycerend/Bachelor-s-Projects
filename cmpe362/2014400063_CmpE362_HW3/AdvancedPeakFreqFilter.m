clear;

data = csvread('exampleSignal.csv');
peakArray = zeros(1,5);
peakArray(1) = numel(findpeaks(data));

for freq = 1:4
    %designing lowpass filter
    d = fdesign.lowpass('N,Fc',90,1000*freq,22050);
    designmethods(d)
    Hd = design(d);
    %applying lowpass filter
    y = filter(Hd,data);

   peakArray(freq+1) = numel(findpeaks(y));

end
%plotting lowpass filter
x = 0:1000:4000;
plot(x,peakArray);