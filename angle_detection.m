clear all                               %Clear working space
close all

infile = 'adc_data_9_ at_315_deg.txt';
data = load(infile, '-ascii');
t = data(:,1);
t = t'; 
Channel_1 = data(:,2); % column 1 of the data text file is assigned the variable x
Channel_1 = Channel_1'; 
Channel_2 = data(:,3); % column 1 of the data text file is assigned the variable x
Channel_2 = Channel_2';

dot_product = dot(Channel_1,Channel_2);
norm_product = (norm(Channel_1)*norm(Channel_2));
phase_shift_in_radians = acos(dot_product/norm_product)


MAX_CH1 = 1.4969;
MIN_CH1 = 1.4934; 

MAX_CH2 = 1.5009;
MIN_CH2 = 1.4965;


Start_CH1 = find(abs(Channel_1 - MAX_CH1) < .0001);
Start_CH2 = find(abs(Channel_2 - MAX_CH2) < .00001);

% size_ch1 = length(Start_CH1); 
% size_ch2 = length(Start_CH2);
 
% sum_ch1 = sum(Start_CH1); 
% sum_ch2 = sum(Start_CH2); 

sum_ch1 = sum(Start_CH1);   
sum_ch2 = sum(Start_CH2); 

% Start_CH1 = Start_CH1(ceil(size_ch1/2)); 
% Start_CH2 = Start_CH2(ceil(size_ch2/2)); 




if Start_CH1 < Start_CH2
    fprintf('Signal is coming from the RIGHT\n'); 
    FLAG = 1;
elseif Start_CH1 > Start_CH2
    fprintf('Signal is coming from the LEFT\n');
    FLAG = 2;
else
    fprintf('Signal is IN FRONT DEVICE\n');
    FLAG = 3;
end
    
if FLAG == 1
    if phase_shift_in_radians < 0.0232
        fprintf('Your signal is within the following range: [315° - 360°] \n');
    else
        fprintf('Your signal is within the following range: [270° - 315°] \n');
    end    
else 
    if phase_shift_in_radians < 0.0246
        fprintf('Your signal is within the following range: [0° - 45°] \n');
    else
        fprintf('Your signal is within the following range: [45° - 90°] \n');
    end 
end
        



