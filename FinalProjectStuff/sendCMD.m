function sendCMD(s,cmd,data)
%SENDCMD Summary of this function goes here
%   Detailed explanation goes here

fwrite(s,'B');
fwrite(s,cmd);

dataBytes =typecast(data,'uint8');
for x = dataBytes
    fwrite(s,x);
end
fwrite(s,'S');