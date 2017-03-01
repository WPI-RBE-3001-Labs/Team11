fclose(instrfind);

s = serial('COM21');
set(s,'BaudRate',230400);
fopen(s);
fprintf(s,'a');

runTime = 1000; %in centiseconds
% encoder0 = zeros(1,runTime);

for ii = 1:runTime
    out = fscanf(s);
    C = strsplit(out,',');
    str2double(C(1))
%     encoder0(ii) = str2double(C(1));
end

fclose(s);
delete(s);
clear s;