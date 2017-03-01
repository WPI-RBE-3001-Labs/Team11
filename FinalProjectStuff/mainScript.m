fclose(instrfind);

s = serial('COM88');
set(s,'BaudRate',230400);
fopen(s);

state = 0; 

runTime = 30000;

theta1 = zeros(1,runTime);
theta2 = zeros(1,runTime);
% x = zeros(1,runTime);
% y = zeros(1,runTime);
% Qt0 = cell(1,runTime);
% Qt1 = cell(1,runTime);
% Dt = cell(1,runTime);
upperCurrentL = zeros(1,runTime);
lowerCurrentL = zeros(1,runTime);
setpoints0 = zeros(1,runTime);
setpoints1 = zeros(1,runTime);
firstIR = zeros(1,runTime);
secondIR = zeros(1,runTime);
sumE0 = 0;
lastE0 = 0;
[t2,t1] = inverseK([6,4]);
setpoint0 = t2(2);
sumE1 = 0;
lastE1 = 0;
setpoint1 = t1(2);
grabTime = runTime;
dist = 0;

fprintf(s,'a');

sendCMD(s,4,0);

for ii = 1:runTime
    out = fscanf(s);
    C = strsplit(out,',');
    str2double(C);
    if length(C)>=6
        degs = [str2double(C(1)) str2double(C(2))];
        links = armModel(degs(1),degs(2),6,6);
        firstIR(ii) = str2double(C(3));
        secondIR(ii) = str2double(C(4));
    end
    if mod(ii,2)==0
        %PID for top link
        [Q0,sumE0,lastE0] = PIDcalcs(setpoint0,degs(2),lastE0,sumE0);
        sendCMD(s,1,int16(Q0));
    end
    if mod(ii,2)==1
        %PID for bottom link
        [Q1,sumE1,lastE1] = PIDcalcs(setpoint1,degs(1),lastE1,sumE1);
        sendCMD(s,2,int16(Q1));
    end
     theta1(ii) = degs(1);
     theta2(ii) = degs(2);
%     x(ii) = links(3,1);
%     y(ii) = links(3,2);
    if mod(ii,10) == 0
        plot(links(:,1),links(:,2));
        axis([-12 12 -12 12]);
        drawnow;
    end
   switch state
       case 0
           [t2,t1] = inverseK([9.5,4]);
           setpoint0 = t2(2);
           setpoint1 = t1(2);
           if firstIR(ii)>300
                firstSight = ii;
                state = state + 1;
            end
       case 1
           if firstIR(ii)>300
                dist = mean(firstIR(ii-50:ii));
           end
           if secondIR(ii)>175
                timeDiff = ii-firstSight;
                grabTime = timeDiff*1.35 + ii;
           end
           if ii>grabTime
               state = state+1;
               currTime = ii;
           end
       case 2
           if(dist>250)
               [t2,t1] = inverseK([adc2In(dist),.25]);
           else
               [t2,t1] = inverseK([7,.25]);
               disp(['invalid dist ',num2str(ii)]);
           end
           setpoint0 = t2(2);
           setpoint1 = t1(2);
           if ii-currTime>25
               state = state + 1;
               currTime = ii;
               sendCMD(s,3,0);
           end
       case 3
           if ii-currTime>100
               state = state + 1;
               currTime = ii;
           end
       case 4
           [t2,t1] = inverseK([10,6]);
           setpoint0 = t2(2);
           setpoint1 = t1(2);
           if ii-currTime>200
               state = state + 1;
               currTime = ii;
           end
       case 5
           lowerCurrentL(ii) = str2double(C(5));
           upperCurrentL(ii) = str2double(C(6));
           if ii-currTime>200
               avgCurrent = mean(upperCurrentL(ii-250:ii).^2)
               if avgCurrent < 240000
                  state = 6; 
               else
                   state = 8;
               end
               currTime = ii;
           end
       case 6
           [t2,t1] = inverseK([9,1]);
           setpoint0 = t2(2);
           setpoint1 = t1(2);
           if ii-currTime>50
               state = state + 1;
               currTime = ii;
               sendCMD(s,3,0);
           end
       case 7
           if ii-currTime>100
               state = 0;
               grabTime = runTime;
               currTime = ii;
           end
        case 8
            [t2,t1] = inverseK([11.5,1.5]);
            setpoint0 = t2(2);
            setpoint1 = t1(2);
            if ii-currTime>50
               state = 7;
               currTime = ii;
               sendCMD(s,3,0);
            end
    end
    setpoints0(ii) = setpoint0;
    setpoints1(ii) = setpoint1;
end
sendCMD(s,4,0);
sendCMD(s,10,0);
% mean(upperCurrentL(2200:2400).^2);
% mean(upperCurrentL(800:1000).^2);

fclose(s);