clear;
RGB = imread('Hubble-Massive-Panorama.png');
gray = rgb2gray(RGB);

signal_array = zeros(1024,900);
fs=900 ; % sampling frequency

%investigating each pixels with 2 for-loops
for Columns=1:1024
    for Rows= 1:900
        %if the value of the pixel higher than 40, it means pixel is not
        %black
        if RGB(Rows,Columns)>40
            %get amplitude of the pixel from the function
            Amplitude = getAmplitude(Rows) ;
            %make this pixel's amplitude different from zero to produce
            %signal at the and

            signal_array(Columns,Rows) = Amplitude;
        end

        
    end
        %turn respected column to time domain from freq domain 
        %even if the column is not turned into freq domain before
        %actually it is in the freq domain because it doesn't have any
        %imaginary part
        signal_array(Columns,:) = abs(ifft(signal_array(Columns,:)));
end
%reshaping array as 1 column to produce audio
rSound = reshape(signal_array',1,[]);
%plotting the audio as a signal
plot(rSound)
%creating the audio signal with 900 sampling rate
audiowrite('SonifiedDeepSpace_sound.wav',rSound,fs);

%getAmplitude function returns the amplitude with respect to the decription
%it divides 900 rows into 10 groups and gives specified amplitude after
%examining it's row number 
function amp = getAmplitude(row_number)
    if row_number>=0 && row_number<90
        amp=10;
    elseif row_number>=90 && row_number<180
        amp=9;        
    elseif row_number>=180 && row_number<270
        amp=8;
    elseif row_number>=270 && row_number<360
        amp=7;
    elseif row_number>=360 && row_number<450
        amp=6; 
    elseif row_number>=450 && row_number<540
        amp=5;
    elseif row_number>=540 && row_number<630
        amp=4;
    elseif row_number>=630 && row_number<720
        amp=3;
    elseif row_number>=720 && row_number<810
        amp=2; 
    elseif row_number>=810 && row_number<=900
        amp=1;
    else
        disp('Value is above maximum value.')
    end

end