clear all                               %Clear working space
close all

tic
infile = 'adc_data.txt';
data = load(infile, '-ascii');
t = data(:,1);
t = t'; 
x = data(:,2); % column 1 of the data text file is assigned the variable x
x = x'; 

MAX_X = max(abs(x)) ;
y = x;
y = circshift(y,1);

MAX_Y = max(abs(y)) ;

k1 = find(abs(x - MAX_X) < .000001);
time1 = t(1, k1);

k2 = find(abs(y - MAX_Y) < .000001);
time2 = t(1, k2);

expected = time2 - time1

xlength = length(x);
figure
plot(t,x, 'color', 'b');
hold on
plot(t,y, 'color', 'g');

Fs = find(abs(t - 1) < .000001); %encontrado en el eje de tiempo
Ts = 1/Fs;                             %Time per Sample
Fc = 40000;                             %Pinger Frequency
Fc1= Fc;
Fc2= Fc + 4000; 

%...................................................................
%..................FILTER PARAMETERS................................
%...................................................................
wl=(2 * Fc1)/(Fs);                              %Set normalized cut-off frequency
wh=(2 * Fc2)/(Fs); 
wm=[wl wh]; 
ordfil=floor((2*(0.0501*xlength)))+1;     %Set filter length

hBandpass = fir1(ordfil-1,wm,'Bandpass');      %Compute filter coefficie
x = conv(x, hBandpass);
x = x(1:376084);
y = conv(y, hBandpass);
y = y(1:376084);

X = fft(x);
Y = fft(y);
Z = X.*Y; 
z = ifft(Z);

%m2 es donde ocurre el máximo en el vector
[MAX, LAG] = max(abs(z)) ;

%LAG
zero = 106596;
LAG = LAG - zero; 
time = LAG/Fs

toc



