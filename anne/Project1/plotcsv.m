close all;
clc;

nn1a=csvread('W:\SchoolProjects\anne\data\nn1a.csv');
nn1b=csvread('W:\SchoolProjects\anne\data\nn1b.csv');
nn2a=csvread('W:\SchoolProjects\anne\data\nn2a.csv');
nn2b=csvread('W:\SchoolProjects\anne\data\nn2b.csv');
testnn1a=csvread('W:\SchoolProjects\anne\data\testnn1a.csv');
testnn1b=csvread('W:\SchoolProjects\anne\data\testnn1b.csv');
testnn2a=csvread('W:\SchoolProjects\anne\data\testnn2a.csv');
testnn2b=csvread('W:\SchoolProjects\anne\data\testnn2b.csv');

figure
x = nn1a(:, 1);
y = nn1a(:, 2);
plot(x, y,'gx');
hold on
y = sin(x) + atan(x);
plot(x,y,'bx');
y = testnn1a(:,2);
plot(x,y,'yx');
hold off

figure
x = nn1b(:, 1);
y = nn1b(:, 2);
plot(x, y,'gx');
hold on
y = sin(x) + atan(x);
plot(x,y,'x');
y = testnn1b(:,2);
plot(x,y,'yx');
hold off

figure
x = nn2a(:, 1);
y = nn2a(:, 2);
z = nn2a(:, 3);
plot3(x, y, z,'gx');
hold on
z = sin(y) + atan(x);
plot3(x,y,z,'x');
z = testnn2a(:,3);
plot3(x,y,z,'yx');
hold off

figure
x = nn2b(:, 1);
y = nn2b(:, 2);
z = nn2b(:, 3);
plot3(x, y, z,'gx');
hold on
z = sin(y) + atan(x);
plot3(x,y,z,'x');
z = testnn2b(:,3);
plot3(x,y,z,'yx');
hold off
