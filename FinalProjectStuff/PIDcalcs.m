function [ output,newSum,newLast] = PIDcalcs( setpoint,value,lastE,sumE )
%PIDCALCS Summary of this function goes here
%   Detailed explanation goes here
kP = 120;
kI = .17;
kD = 10;

error = value-setpoint;
P = error * kP;
newSum = sumE + error;
I = newSum * kI;
newLast = error;
D = (error-lastE) * kD;
output = P + I + D;
end

