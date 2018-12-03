TAPS = 4096;
WINDOW=1024;
FS=48000;
FC=4000;
BLOCK_SIZE=512;

INPUT_LENGTH=TAPS+WINDOW-1;
ii=0:1:INPUT_LENGTH-1;

sine_input=sin(2*pi*ii*1000/FS)+sin(2*pi*ii*8000/FS); 
b = fir1(TAPS-1, 2*FC/FS);	% generate filter coefficients

a=1;
fir_output=filter(b,a,sine_input);

plot(sine_input);
figure;plot(fir_output);

fp = fopen('fir_input.dat','w');		% create input file

for i=1:INPUT_LENGTH-1,				
	fprintf(fp,'%2.20e,\n',sine_input(i));	
end
    fprintf(fp,'%2.20e\n',sine_input(INPUT_LENGTH));

fclose(fp);

fp = fopen('fir_coeffs.dat','w');		% create output file

for i=1:length(b)-1,				
	fprintf(fp,'%2.20e,\n',b(i));	% write each coefficient to output file
end
fprintf(fp,'%2.20e\n',b(TAPS));		% write last coefficient without comma

fclose(fp);							% close file

fp = fopen('fir_output.dat','w');		% create output file

for i=TAPS:1:INPUT_LENGTH-1,				
	fprintf(fp,'%2.20e,\n',fir_output(i));	% write each coefficient to output file
end

fprintf(fp,'%2.20e\n',fir_output(INPUT_LENGTH));	% write each coefficient to output file

fclose(fp);							% close file


disp('Finished!');


