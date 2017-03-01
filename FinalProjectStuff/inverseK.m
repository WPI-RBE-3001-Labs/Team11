function [ theta2,theta1 ] = inverseK( pos )
%INVERSEK Summary of this function goes here
%   Detailed explanation goes here
l = [6,6];
theta2 = [atan2d(sqrt(1-(((pos(1)^2 + pos(2)^2 - l(1)^2 - l(2)^2)/(2*l(1)*l(2)))^2)),((pos(1)^2 + pos(2)^2 - l(1)^2 - l(2)^2)/(2*l(1)*l(2)))),...
        atan2d(-sqrt(1-(((pos(1)^2 + pos(2)^2 - l(1)^2 - l(2)^2)/(2*l(1)*l(2)))^2)),((pos(1)^2 + pos(2)^2 - l(1)^2 - l(2)^2)/(2*l(1)*l(2))))];
theta1 = atan2d(pos(2),pos(1))-atan2d(l(2)*sind(theta2),l(1)+l(2)*cosd(theta2));
end

