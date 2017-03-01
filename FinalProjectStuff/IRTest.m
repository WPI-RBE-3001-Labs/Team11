fclose(instrfind);

s = serial('COM88');
set(s,'BaudRate',230400);
fopen(s);


runTime = 750;

firstIR = zeros(1,runTime);

fprintf(s,'a');
sendCMD(s,4,0);

for ii = 1:runTime
    out = fscanf(s);
    C = strsplit(out,',');
    firstIR(ii) = str2double(C(3));
    if and(firstIR(ii)>300,ii>50)
        firstSight = ii;
        dist = mean(firstIR(ii-50:ii))
    end
end

sendCMD(s,4,0);

fclose(s);