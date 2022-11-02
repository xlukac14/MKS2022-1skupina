close all;
clear all;

%% Load data
raw = csvread("ntc.csv");

x = raw(:,1);
y = raw(:,2);

% figure;
% plot(raw(:,1), raw(:,2))

%% Conversion
% ADC = Uin/Uref * 2^Nbit

Nbit = 10;

t = x;

ad = 1024.*(y./(y+10));

p = polyfit(ad, t, 10);

ad2=0:1023;
t2 = round(polyval(p,ad2),1);

hold on, plot(ad2, t2, 'r');

dlmwrite('data.dlm', t2*10, ',');
