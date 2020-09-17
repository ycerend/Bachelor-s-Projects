clear;
%reading data with specifying cells in csv file
data = dlmread('exampleSignal.csv',',','A4..A49502');
%peakArray will keep the peak results after the filter applied
peakArray = zeros(1,30);

%first result will be without applying the filter
peakArray(1) = numel(findpeaks(data));

%calculate the averages of windows at given number of windows
%moving_average_filter function will filter the data and return
%filtered data array

for window = 2:30
   filtered_data = moving_average_filter(data,window);
   peakArray(window) = numel(findpeaks(filtered_data));

end
%plotting the resulting peak numbers versus window size
x = 1:1:30;
plot(x,peakArray);


%filter function to take average at a given given windows size
function temp_array = moving_average_filter(data,window_size)
    array_length = size(data,1)-window_size+1;
    temp_array = zeros(1,array_length);
    for k=1:array_length
        temp = 0;
        for m=k:(k+window_size-1)
            temp = temp + data(m,1);
        end
        
        temp = temp./ window_size;
        temp_array(1,k) = temp;
    end

end
