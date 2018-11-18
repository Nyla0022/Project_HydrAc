clear all                               %Clear working space
close all

tic
Fs = 160000;                            %Sampling Frequency
Ts = 1/Fs;                              %Time per Sample
Fc = 40000;                             %Pinger Frequency
Fc1= Fc;
Fc2= Fc + 4000; 
%...................................................................
%.........CREATING SIGNAL RECIEVED FROM HYDROPHONE A ...............
%...................................................................
t1 = 0 : Ts : 1 ;
t2 = 1 : Ts : 1.004 ;                 %Assuming we recived signal at 1s   
t3 = 1.004 : Ts : 2 ;                 %Pinger only 3lasts 4ms 
t = [t1 t2 t3];                      %Signal will be captured at 2 seconds
x1 = zeros(size(t1)); 
x2 = 0.5 * (sin(2* pi * Fc * t2 ) + 1);  %Sine signal at pinger frequency
x3 = zeros(size(t3));
x = [x1 x2 x3] ;                        %Signal A

k1 = find(x,1);
time1 = t(1, k1); 

x = x(160000:161023); 
t = t(160000:161023); 


...................................................................
.........CREATING SIGNAL RECIEVED FROM HYDROPHONE B ...............
...................................................................
t4 = 0 : Ts : 1.0004 ;
t5 = 1.0004 : Ts : 1.0044 ;          
t6 = 1.0044 : Ts : 2 ;
t7 = [t4 t5 t6]; 
y1 = zeros(size(t4)); 
y2 = (0.5) * (sin(2 * pi * Fc * t5 ) + 1);  %Sine signal at pinger frequency
y3 = zeros(size(t6));
%y = [y1 y2 y3] ;                        %Signal B
y=x;
 
k2 = find(y,1); 
time2 = t7(1, k2);

%y = y(160000:161023); 
t7 = t7(160000:161023); 

xlength = length(x); 

%...................................................................
%.........PLOTTING CLEAN CREATED SIGNALS ...........................
%...................................................................
figure
subplot(2,2,1);
plot (t, x, 'color', 'b');
title('Input from Hydrophone A')
xlabel('Time in Second (s)')
subplot(2,2,2);
plot (t, y, 'color', 'g'); 
title('Input from Hydrophone B')
xlabel('Time in Second (s)')
subplot(2,2,[3,4]);
plot (t, x, 'color', 'b');
hold on
plot (t, y, 'color', 'g');
title('Inputs from Hydrophone A and B')
xlabel('Time in Second (s)')


%...................................................................
%.........ADDING GAUSSIAN NOISE ....................................
%...................................................................
x = awgn(x,35);
y = awgn(y,35);
x = x' ;
y = y' ; 
save('x_1024_Q1.txt','x','-ascii')
save('y_1024_Q1.txt','y','-ascii')
x = x' ;
y = y' ;
%...................................................................
%.........PLOTTING NOISY CREATED SIGNALS ...........................
%...................................................................
figure
subplot(2,2,1);
plot (t, x, 'color', 'b');
title('Input from Hydrophone A')
xlabel('Time in Second (s)')
subplot(2,2,2);
plot (t, y, 'color', 'g'); 
title('Input from Hydrophone B')
xlabel('Time in Second (s)')
subplot(2,2,[3,4]);
plot (t, x, 'color', 'b');
hold on
plot (t, y, 'color', 'g');
title('Inputs from Hydrophone A and B')
xlabel('Time in Second (s)')



%...................................................................
%..................FILTER PARAMETERS................................
%...................................................................
wl=(2 * Fc1)/(Fs);                              %Set normalized cut-off frequency
wh=(2 * Fc2)/(Fs); 
wm=[wl wh]; 
ordfil=floor((2*(0.0501*xlength)))+1;     %Set filter length

xlengthp = xlength; 
prevx = x;
prevy = y;
%...................................................................
%.........FINITE IMPULSE RESPONSE FILTERING.........................
%...................................................................
hBandpass = fir1(ordfil-1,wm,'Bandpass');      %Compute filter coefficients
hBandpass = hBandpass' ;
save('COEF_1024_Q1.txt','hBandpass','-ascii')
hBandpass = hBandpass' ;
x = conv(x, hBandpass);
x = x(1:1024);
% x = x' ;
% save('x_1024_filtered.txt','x','-ascii')
% x = x' ;
y = conv(y, hBandpass);
y = y(1:1024); 
% y = y' ;
% save('y_1024_filtered.txt','y','-ascii')
% y = y' ;
xlength = length(x); 

% [MAXA,EJEA] = max(abs(x)); 
% [MAXB,EJEB] = max(abs(y)); 

%...................................................................
%............CAPTURING SPECTRUM OF FILTERED SIGNAL..................
%...................................................................
F1=fft(x);                   %Take the FFT of input signal
F2=fftshift(F1);
F3=abs(F2);

F4=fft(y);                   %Take the FFT of input signal
F5=fftshift(F4);
F6=abs(F5);

F1p=fft(prevx);                   %Take the FFT of input signal
F2p=fftshift(F1p);
F3p=abs(F2p);

F4p=fft(prevy);                   %Take the FFT of input signal
F5p=fftshift(F4p);
F6p=abs(F5p);

%...................................................................
%............PLOTTING SPECTRUM OF FILTERED SIGNAL..................
%...................................................................
Fry=(1/(xlength*Ts));                 %Set output frequency resolution         
fydwave=-(Fs/2):Fry:(Fs/2)-Fry;       %Set frequency axis of output signal
Fryp=(1/(xlengthp*Ts));                 %Set output frequency resolution         
fydwavep=-(Fs/2):Fryp:(Fs/2)-Fryp;       %Set frequency axis of output signal

figure
subplot(2,1,1);
plot(fydwavep,F3p, 'color', 'b'); 
title('Spectrum of signal A')
xlabel('Frequency in Hertz (Hz)')
subplot(2,1,2);
plot(fydwave,F3, 'color', 'b'); 
title('Spectrum of filtered signal A')
xlabel('Frequency in Hertz (Hz)')


figure
subplot(2,1,1);
plot(fydwavep,F6p, 'color', 'g'); 
title('Spectrum of signal B')
xlabel('Frequency in Hertz (Hz)')
subplot(2,1,2);
plot(fydwave,F6, 'color', 'g'); 
title('Spectrum of filtered signal B')
xlabel('Frequency in Hertz (Hz)')


X = fft(x);
% X = X' ;
% save('X_F.txt','X','-ascii')
% X = X' ;

Y = fft(y);
% Y = Y' ;
% save('Y_F.txt','Y','-ascii')
% Y = Y' ;

Z = X.*Y; 
z = ifft(Z);

%m2 es donde ocurre el máximo en el vector
[MAX, LAG] = max(abs(z)) ;

LAG
%zero = 747; 
%LAG = LAG - zero; 
time = LAG/Fs

expected = time2 - time1


toc
