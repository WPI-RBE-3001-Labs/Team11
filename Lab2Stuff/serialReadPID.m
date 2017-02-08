s = serial('COM69');
set(s,'BaudRate',230400);
fopen(s);
fprintf(s,'a');

runTime = 500; %in deciseconds

driveVal = zeros(1,runTime);
setPoint = zeros(1,runTime);
pos = zeros(1,runTime);
current = zeros(1,runTime);
kP = zeros(1,runTime);
kI = zeros(1,runTime);
kD = zeros(1,runTime);

for ii = 1:runTime
    out = fscanf(s);
    C = strsplit(out,',');
    driveVal(ii) = str2double(C(1));
    setPoint(ii) = str2double(C(2));
    pos(ii) = str2double(C(3));
    current(ii) = str2double(C(4));
    kP(ii) = str2double(C(5));
    kI(ii) = str2double(C(6));
    kD(ii) = str2double(C(7));
end

fclose(s);
delete(s);
clear s;

plot(setPoint,'DisplayName','Setpoint');
hold on;
plot(pos,'DisplayName','Position');
hold off;
