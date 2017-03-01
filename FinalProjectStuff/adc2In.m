function [ inches ] = adc2In( adcCnts )
%ADC2IN Summary of this function goes here
%   Detailed explanation goes here

inches = -.019*adcCnts + 16.2;
end

