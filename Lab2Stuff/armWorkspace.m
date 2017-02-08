botLink = -16:182;
topLink = -108:90;
botRange = length(botLink);
topRange = length(topLink);

x = zeros(39204,1);
y = zeros(39204,1);

for t1 = 1:botRange
    for t2 = 1:topRange
        t1*t2
        links = armModel(botLink(t1),topLink(t2),6,6);
        x(t1*t2) = links(3,1);
        y(t1*t2) = links(3,2);
    end
end

scatter(x,y);