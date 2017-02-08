fclose(instrfind);

s = serial('COM77');
set(s,'BaudRate',230400);
fopen(s);
fprintf(s,'a');

runTime = 2500; %in centiseconds
theta1 = zeros(runTime);
theta2 = zeros(runTime);
x = zeros(runTime);
y = zeros(runTime);

for ii = 1:runTime
    out = fscanf(s);
    C = strsplit(out,',');
    degs = [str2double(C(1)) str2double(C(2))];
    links = armModel(degs(1),degs(2),6,6);
    str2double(C)
    theta1(ii) = str2double(C(1));
    theta2(ii) = str2double(C(2));
    x(ii) = links(3,1);
    y(ii) = links(3,2);
    if mod(ii,5) == 0
        plot(links(:,1),links(:,2));
        axis([-12 12 -12 12]);
        drawnow;
    end
end

fclose(s);
delete(s);
clear s;
