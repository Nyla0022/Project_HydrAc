clear all                               %Clear working space
close all

Fs = 192000;                            %Sampling Frequency
Ts = 1/Fs;                              %Time per Sample
Fc1 = 24000;


infile = 'adc_data_9inches_45deg.txt';
data = load(infile, '-ascii');
t = data(:,1);
t = t'; 
Channel_1 = data(:,2); % column 1 of the data text file is assigned the variable x
Channel_1 = Channel_1'; 
Channel_2 = data(:,3); % column 1 of the data text file is assigned the variable x
Channel_2 = Channel_2';

dot_product = dot(Channel_1,Channel_2);
norm_product = (norm(Channel_1)*norm(Channel_2));
phase_shift_in_radians = acos(dot_product/norm_product);


figure
plot(t, Channel_1);
hold on
plot(t, Channel_2);
xlength = length(Channel_1); 


ordfil=floor((2*(0.0501*xlength)))+1;     %Set filter length
W_HIGH = (2*Fc1)/Fs;

high_coef = fir1(ordfil-1,W_HIGH,'high');      %Compute filter coefficients
high_coef = high_coef';
save('hug.txt','high_coef','-ascii');
high_coef = high_coef';

Channel_1 = conv(Channel_1, high_coef);
Channel_2 = conv(Channel_2, high_coef);

MAX_CH1 = 0.0025;

Start_CH1 = find(abs(Channel_1 - MAX_CH1) < .001);
Start_CH2 = find(abs(Channel_2 - MAX_CH1) < .001);

size_Start_CH1 = length(Start_CH1);
size_Start_CH2 = length(Start_CH2);
location_ch1 = 1;
location_ch2 = 1;

if (Start_CH1(location_ch1) == Start_CH1(location_ch2))
    for i = (2 : size_Start_CH1)
        dif = Start_CH1(i) - Start_CH1(i-1);
        if (dif > 500)
            location_ch1 = i;
            break;
        end
    end

    for i = (2 : size_Start_CH2)
        dif = Start_CH2(i) - Start_CH2(i-1);
        if (dif > 500)
            location_ch2 = i;
            break;
        end
    end
end 


Start_CH1_F = Start_CH1(location_ch1); 
Start_CH2_F = Start_CH2(location_ch2);

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
     
if FLAG == 1
    if phase_shift_in_radians < .0232
        fprintf('Your signal is within the following range: [315° - 360°] \n');
    else
        fprintf('Your signal is within the following range: [270° - 315°] \n');
    end    
else 
    if phase_shift_in_radians < .0312
        fprintf('Your signal is within the following range: [0° - 45°] \n');
    else
        fprintf('Your signal is within the following range: [45° - 90°] \n');
    end 
end

xlength = length(Channel_1); 
F4p=fft(Channel_1);                   %Take the FFT of input signal
F5p=fftshift(F4p);
F6p=abs(F5p);
F4p=fft(Channel_2);                   %Take the FFT of input signal
F5p=fftshift(F4p);
F6p2=abs(F5p);
%...................................................................
%............PLOTTING SPECTRUM OF FILTERED SIGNAL..................
%...................................................................
Fry=(1/(xlength*Ts));                 %Set output frequency resolution         
fydwave=-(Fs/2):Fry:(Fs/2)-Fry;       %Set frequency axis of output signal
figure
plot(fydwave,F6p, 'color', 'g'); 
xlabel('Frequency in Hertz (Hz)')
figure
plot(fydwave,F6p2, 'color', 'y'); 
xlabel('Frequency in Hertz (Hz)')
figure
plot(t, Channel_1(1:384000));
hold on
plot(t, Channel_2(1:384000));
