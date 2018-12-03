clear all                               %Clear working space
close all

Fs = 192000;                            %Sampling Frequency
Ts = 1/Fs;                              %Time per Sample
Fc1 = 24000;
Fc2 = 42000; 


infile = 'adc_data_9inches_45deg.txt';
data = load(infile, '-ascii');
t = data(:,1);
t = t'; 
Channel_1 = data(:,2); % column 1 of the data text file is assigned the variable x
save('45_CH1.txt', 'Channel_1','-ascii'); 
Channel_1 = Channel_1';
Channel_2 = data(:,3); % column 1 of the data text file is assigned the variable x
save('45_CH2.txt', 'Channel_2','-ascii'); 
Channel_2 = Channel_2';

figure
plot(t, Channel_1);
hold on
plot(t, Channel_2);

MAX_CH1 = 1.505;

Start_CH1 = find(abs(Channel_1 - MAX_CH1) < .001);
Start_CH2 = find(abs(Channel_2 - MAX_CH1) < .001);

size_Start_CH1 = length(Start_CH1);
size_Start_CH2 = length(Start_CH2);
location_ch1 = 1;
location_ch2 = 1;

if((Channel_1(1) < 1.505) && (Channel_2(1) < 1.505))
    if (Start_CH1(location_ch1) == Start_CH1(location_ch2))
        for i = (2 : size_Start_CH1)
            dif = Start_CH1(i) - Start_CH1(i-1);
            if (dif > 5)
                location_ch1 = i;
                break;
            end
        end
    end 
    Start_CH1_F = Start_CH1(location_ch1); 
    Channel_1 = Channel_1(Start_CH1_F-2047 : Start_CH1_F +2048); 
    Channel_2 = Channel_2(Start_CH1_F-2047 : Start_CH1_F +2048);
else
     Channel_1 = Channel_1(1:4096);
     Channel_2 = Channel_2(1:4096); 
end

% Channel_1 = Channel_1'; 
% save('75_4096_CH1.txt','Channel_1','-ascii');
% Channel_1 = Channel_1';
% 
% Channel_2 = Channel_2'; 
% save('75_4096_CH2.txt','Channel_2','-ascii');
% Channel_2 = Channel_2';
% 
% figure
% plot(Channel_1, 'color', 'g');
% hold on
% plot(Channel_2, 'color', 'b');
% 
% xlength = length(Channel_1); 
% 
% %ordfil=floor((2*(0.0501*xlength)))+1;     %Set filter length
% ordfil = 3073;
% W_BP1 = (2*Fc1)/Fs;
% W_BP2 = (2*Fc2)/Fs;
% W_BP = [W_BP1 W_BP2] ;
% 
% high_coef = fir1(ordfil-1,W_BP1,'high');      %Compute filter coefficients
% high_coef = high_coef'; 
% save('HIGH_4096.txt','high_coef','-ascii');
% high_coef = high_coef';
% 
% Channel_1 = conv(Channel_1, high_coef);
% Channel_1 = Channel_1'; 
% save('75_4096_CH1_F.txt','Channel_1','-ascii');
% Channel_1 = Channel_1';
% 
% Channel_2 = conv(Channel_2, high_coef);
% Channel_2 = Channel_2'; 
% save('75_4096_CH2_F.txt','Channel_2','-ascii');
% Channel_2 = Channel_2';
% 
% figure
% plot(Channel_1, 'color', 'g');
% hold on
% plot(Channel_2, 'color', 'b');
% 
% for i = 1:1024
%     Channel_1(i) = 0;
%     Channel_2(i) = 0; 
% end 
% 
% 
% figure
% plot(Channel_1, 'color', 'g');
% hold on
% plot(Channel_2, 'color', 'b');
% 
MAX_CH = 0.3;
MIN_CH = -0.3;

Start_CH1_MAX = find(abs(Channel_1 - MAX_CH) < .01);
Start_CH1_MIN = find(abs(Channel_1 - MIN_CH) < .01);
Start_CH2_MAX = find(abs(Channel_2 - MAX_CH) < .01); 
Start_CH2_MIN = find(abs(Channel_2 - MIN_CH) < .01);

if (Start_CH1_MIN(1) < Start_CH1_MAX(1))
    Start_CH1_F = Start_CH1_MIN(1);
else
    Start_CH1_F = Start_CH1_MAX(1);
end

if (Start_CH2_MIN(1) < Start_CH2_MAX(1))
    Start_CH2_F = Start_CH2_MIN(1);
else
    Start_CH2_F = Start_CH2_MAX(1);
end

if Start_CH1_F < Start_CH2_F
    fprintf('Signal is coming from the RIGHT\n'); 
    FLAG = 1;
elseif Start_CH1_F > Start_CH2_F
    fprintf('Signal is coming from the LEFT\n');
    FLAG = 2;
else
    fprintf('Signal is IN FRONT DEVICE\n');
    FLAG = 3;
end


