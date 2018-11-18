clear all                               %Clear working space
close all

Fs = 1024;                            %Sampling Frequency
Ts = 1/Fs;                              %Time per Sample
Fc1 = 100;
Fc2 = 300; 
Fc3 = 500;

t1 = 0 : Ts : .333 ;
t2 = .333 : Ts : .666 ;                 %Assuming we recived signal at 1s   
t3 = .666: Ts : .999 ;                 %Pinger only 3lasts 4ms 
t = [t1 t2 t3];                      %Signal will be captured at 2 seconds
x1 = (sin(2*pi * Fc1 * t1 )); 
x2 = (sin(2*pi * Fc2 * t2 ));     %Sine signal at pinger frequency
x3 = (sin(2*pi * Fc3 * t3 ));
x = [x1 x2 x3] ;
x = x' ;
save('x_FILTERS.txt','x','-ascii')
x = x' ;
xlength = length(x); 


%Signal A
F4p1=fft(x);                   %Take the FFT of input signal
F5p1=fftshift(F4p1);
F6p1=abs(F5p1);
Fry1=(1/(xlength*Ts));                 %Set output frequency resolution         
fydwave1=-(Fs/2):Fry1:(Fs/2)-Fry1;       %Set frequency axis of output signal
plot (t, x); 
xlength = length(x); 
ordfil=floor((2*(0.0501*xlength)))+1;     %Set filter length
W_low = (2*Fc1)/Fs;



wl=(2 * Fc2)/(Fs);                              %Set normalized cut-off frequency
wh=(2 * Fc3)/(Fs); 
wm=[wl wh]; 

low_coef = fir1(ordfil-1,W_low,'low');      %Compute filter coefficients
low_coef = low_coef' ;
save('LOW_COEF.txt','low_coef','-ascii')
low_coef = low_coef' ;
high_coef = fir1(ordfil-1,W_low,'high');      %Compute filter coefficients
high_coef = high_coef' ;
save('HIGH_COEF.txt','high_coef','-ascii')
high_coef = high_coef' ;
bandpass_coef = fir1(ordfil-1,wm,'bandpass');      %Compute filter coefficients
bandpass_coef = bandpass_coef' ;
save('BAND_COEF.txt','bandpass_coef','-ascii')
bandpass_coef = bandpass_coef' ;



x_filtered_low = conv(x, low_coef);
x_filtered_low = x_filtered_low' ;
x_filtered_low = x_filtered_low(1:1024);
save('x_filtered_low.txt','x_filtered_low','-ascii')
x_filtered_low = x_filtered_low' ;
x_filtered_high = conv(x, high_coef);
x_filtered_high = x_filtered_high' ;
x_filtered_high = x_filtered_high(1:1024);
save('x_filtered_high.txt','x_filtered_high','-ascii')
xx_filtered_high = x_filtered_high' ;
x_filtered_bandpass = conv(x, bandpass_coef);
x_filtered_bandpass = x_filtered_bandpass' ;
x_filtered_bandpass = x_filtered_bandpass(1:1024);
save('x_filtered_bandpass.txt','x_filtered_bandpass','-ascii')
x_filtered_bandpass = x_filtered_bandpass' ;

xlength = length(x_filtered_bandpass); 

F4p=fft(x_filtered_bandpass);                   %Take the FFT of input signal
F5p=fftshift(F4p);
F6p=abs(F5p);

F4p=fft(x_filtered_low);                   %Take the FFT of input signal
F5p=fftshift(F4p);
F6p2=abs(F5p);

F4p=fft(x_filtered_high);                   %Take the FFT of input signal
F5p=fftshift(F4p);
F6p3=abs(F5p);

%...................................................................
%............PLOTTING SPECTRUM OF FILTERED SIGNAL..................
%...................................................................
Fry=(1/(xlength*Ts));                 %Set output frequency resolution         
fydwave=-(Fs/2):Fry:(Fs/2)-Fry;       %Set frequency axis of output signal
figure
plot(fydwave1,F6p1, 'color', 'b'); 
title('Spectrum of signal x')
xlabel('Frequency in Hertz (Hz)')
figure
plot(fydwave,F6p, 'color', 'g'); 
title('FILTERED - BANDPASS - CUTOFF [300-500]')
xlabel('Frequency in Hertz (Hz)')
figure
plot(fydwave,F6p2, 'color', 'y'); 
title('FILTERED - LOW - CUTOFF [100]')
xlabel('Frequency in Hertz (Hz)')
figure
plot(fydwave,F6p3, 'color', 'r'); 
title('FILTERED - HIGH - CUTOFF [100]')
xlabel('Frequency in Hertz (Hz)')