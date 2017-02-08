function a = armModel( theta1,theta2,len1,len2 )
%ARMMODEL Summary of this function goes here
%   Detailed explanation goes here

p0 = [0,0];
p1 = [len1*cosd(theta1),len1*sind(theta1)];
p2 = [p1(1) + len2*cosd(theta1+theta2),p1(2) + len2*sind(theta1 +theta2)];
links = [p0;p1;p2];

a = links;

end

