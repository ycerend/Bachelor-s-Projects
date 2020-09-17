%-------- Problem 1 script ---------%
figure('Name', 'Question1');

subplot(4,2,1)
x = linspace(-100,100);
y1 = sin(x);
plot(x, y1)
title('Subplot 1: y1 = sin(x)')

subplot(4,2,2)
y2 = sin(50*x);
plot(x, y2)
title('Subplot 2: y2 = sin(50x)')

subplot(4,2,3)
y3 = 50*sin(x);
plot(x, y3)
title('Subplot 3: y3 = 50sin(x)')

subplot(4,2,4)
y4 = sin(x) + 50;
plot(x, y4)
title('Subplot 4: y4 = sinx+50')

subplot(4,2,5)
y5 = sin(x + 50);
plot(x, y5)
title('Subplot 5: y5 = sin(x+50)')

subplot(4,2,6)
y6 = 50*sin(50*x);
plot(x, y6)
title('Subplot 6: y6 = 50sin50x')

subplot(4,2,7)
y7 = x.*sin(x);
plot(x, y7)
title('Subplot 7: y7 =  x*sinx')

subplot(4,2,8)
y8 = sin(x)./x;
plot(x, y8)
title('Subplot 8: y8 = sinx/x')

%-------- Problem 2 script ---------%

figure('Name', 'Question 2');
x = linspace(-20,20);           %x is a vector of real numbers between -20 and 20.

subplot(5,2,1)                  
y1 = sin(x);
plot(x,y1)
title('Subplot 1: sin(x)')

subplot(5,2,2)                  
y2 = sin(50*x);
plot(x,y2)
title('Subplot 2: sin(50x)')

subplot(5,2,3)                 
y3 = 50*sin(x);
plot(x,y3)
title('Subplot 3: 50sin(x)')

subplot(5,2,4)                  
y4 = sin(x)+50;
plot(x,y4)
title('Subplot 4: sin(x)+50')

subplot(5,2,5)                
y5 = sin(x+50);
plot(x,y5)
title('Subplot 5: sin(x+50)')

subplot(5,2,6)                 
y6 = 50*sin(50*x);
plot(x,y6)
title('Subplot 6: 50sin(50x)')

subplot(5,2,7)                 
y7 = x.*sin(x);
plot(x,y7)
title('Subplot 7: xsin(x)')

subplot(5,2,8)                
y8 = sin(x)./x;
plot(x,y8)
title('Subplot 8: sin(x)/x')

subplot(5,2,9)                  
y9 = y1+y2+y3+y4+y5+y6+y7+y8;
plot(x,y9)
title('Subplot 9: y1+y2+y3+y4+y5+y6+y7+y8')

%-------- Problem 3 script ---------%

figure('Name', 'Question 3');

x = (-20:20);                  
z = randn(1,41);                %z is a vector of random numbers with length 41.

subplot(5,2,1)                
y10 = z;
plot(x,y10)
title('Subplot 1: z')

subplot(5,2,2)                 
y11 = z+x;
plot(x,y11)
title('Subplot 2: z+x')

subplot(5,2,3)                  
y12 = z+sin(x);
plot(x,y12)
title('Subplot 3: z+sin(x)')

subplot(5,2,4)                
y13 = z.*sin(x);
plot(x,y13)
title('Subplot 4: zsin(x)')

subplot(5,2,5)                  
y14 = x.*sin(z);
plot(x,y14)
title('Subplot 5: xsin(z)')

subplot(5,2,6)                  
y15 = sin(x+z);
plot(x,y15)
title('Subplot 6: sin(x+z)')

subplot(5,2,7)                 
y16 = z.*sin(50*x);
plot(x,y16)
title('Subplot 7: zsin(50x)')

subplot(5,2,8)                  
y17 = sin(x+50*z);
plot(x,y17)
title('Subplot 8: sin(x+50z)')

subplot(5,2,9)                  
y18 = sin(x)./z;
plot(x,y18)
title('Subplot 9: sin(x)/z')

subplot(5,2,10)                 
y19 = y11+y12+y13+y14+y15+y16+y17+y18;
plot(x,y19)
title('Subplot 10: y11+y12+y13+y14+y15+y16+y17+y18')


%-------- Problem 4 script ---------%
figure('Name', 'Question 4');

x = (-20:20);                   
z = rand(1,41);             

subplot(5,2,1)                
y20 = z;
plot(x,y20)
title('Subplot 1: z')

subplot(5,2,2)                
y21 = z+x;
plot(x,y21)
title('Subplot 2: z+x')

subplot(5,2,3)                
y22 = z+sin(x);
plot(x,y22)
title('Subplot 3: z+sin(x)')

subplot(5,2,4)                  
y23 = z.*sin(x);
plot(x,y23)
title('Subplot 4: zsin(x)')

subplot(5,2,5)                  
y24 = x.*sin(z);
plot(x,y24)
title('Subplot 5: xsin(z)')

subplot(5,2,6)                 
y25 = sin(x+z);
plot(x,y25)
title('Subplot 6: sin(x+z)')

subplot(5,2,7)                  
y26 = z.*sin(50*x);
plot(x,y26)
title('Subplot 7: zsin(50x)')

subplot(5,2,8)                  
y27 = sin(x+50*z);
plot(x,y27)
title('Subplot 8: sin(x+50z)')

subplot(5,2,9)                
y28 = sin(x)./z;
plot(x,y28)
title('Subplot 9: sin(x)/z')

subplot(5,2,10)               
y29 = y21+y22+y23+y24+y25+y26+y27+y28;
plot(x,y29)
title('Subplot 10: y21+y22+y23+y24+y25+y26+y27+y28')

%-------- Problem 5 script ---------%
figure('Name', 'Question 5');

z = randn(10000,1);

mean1 = 0;

mu1 = 1;
std1 = sqrt(mu1);
r1 = mean1 + z*std1;   
subplot(2,2,1)
hist(r1)
title('Histogram 1: z(0,1)')

mu2 = 4;
std2 = sqrt(mu2);
r2 = mean1 + z*std2;     %Generate 10000 random variables with mean 0, variance 4
subplot(2,2,2)
hist(r2)
title('Histogram 2: z(0,2)')

mu3 = 16;
std3 = sqrt(mu3);
r3 = mean1 + z*std3;     %Generate 10000 random variables with mean 0, variance 16
subplot(2,2,3)
hist(r3)
title('Histogram 3: z(0,4)')

mu4 = 256;
std4 = sqrt(mu4);
r4 = mean1 + z*std4;    %Generate 10000 random variables with mean 0, variance 256
subplot(2,2,4)
hist(r4)
title('Histogram 4: z(0,16)')

%-------- Problem 6 script ---------%

figure('Name', 'Question 6');
z = randn(10000,1);

mean1 = 10;
mu1 = 1;
std1 = sqrt(mu1);
r6 = mean1 + z*std1;   
subplot(2,2,1)
hist(r6)
title('Histogram 1: z(10,1)')

mean2 = 20;
mu2 = 4;
std2 = sqrt(mu2);
r7 = mean2 + z*std2;     %Generate 10000 random variables with mean 0, variance 4
subplot(2,2,2)
hist(r7)
title('Histogram 2: z(20,2)')

mean3 = -10;
mu3 = 1;
std3 = sqrt(mu3);
r8 = mean3 + z*std3;     %Generate 10000 random variables with mean 0, variance 16
subplot(2,2,3)
hist(r8)
title('Histogram 3: z(-10,1)')

mean4 = -20;
mu4 = 4;
std4 = sqrt(mu4);
r9 = mean4 + z*std4;    %Generate 10000 random variables with mean 0, variance 256
subplot(2,2,4)
hist(r9)
title('Histogram 4: z(-20,2)')

%-------- Problem 7 script ---------%

figure('Name', 'Question 7');

z = rand(10000,1);

mean = 0;

var1 = 1;
a = sqrt(3*var1) + mean;
b = mean - sqrt(3*var1);

r11 = a + (b-a)*z;     %Generate 10000 random variables with mean 0, variance 1
subplot(2,2,1)
hist(r11)
title('Histogram 1: u(0,1)')

var2 = 4;
a2 = sqrt(3*var2) + mean;
b2 = mean - sqrt(3*var2);

r21 = a2 + (b2-a2)*z;      %Generate 10000 random variables with mean 0, variance 4
subplot(2,2,2)
hist(r21)
title('Histogram 2: u(0,2)')

var3 = 16;
a3 = sqrt(3*var3) + mean;
b3 = mean - sqrt(3*var3);

r31 = a3 + (b3-a3)*z;     %Generate 10000 random variables with mean 0, variance 16
subplot(2,2,3)
hist(r31)
title('Histogram 3: u(0,4)')

var4 = 256;
a4 = sqrt(3*var4) + mean;
b4 = mean - sqrt(3*var4);

r41 = a4 + (b4-a4)*z;      %Generate 10000 random variables with mean 0, variance 256
subplot(2,2,4)
hist(r41)
title('Histogram 4: u(0,16)')

%-------- Problem 8 script ---------%

figure('Name', 'Question 8');

z = rand(10000,1);

mean1 = 10;
var1 = 1;
a1 = sqrt(3*var1) + mean1;
b1 = mean1 - sqrt(3*var1);    %Generate 10000 random variables with mean 10, variance 1
r61 = a1 + (b1-a1)*z;
subplot(2,2,1)
hist(r61)
title('Histogram 1: u(10,1)')

mean2 = 20;
var2 = 4;
a2 = sqrt(3*var2) + mean2;
b2 = mean2 - sqrt(3*var2);  
r71 = a2 + (b2-a2)*z;    %Generate 10000 random variables with mean 20, variance 4
subplot(2,2,2)
hist(r71)
title('Histogram 2: u(20,2)')

mean3 = -10;
var3 = 1;
a3 = sqrt(3*var3) + mean3;
b3 = mean3 - sqrt(3*var3);    
r81 = a3 + (b3-a3)*z;    %Generate 10000 random variables with mean -10, variance 1
subplot(2,2,3)
hist(r81)
title('Histogram 3: u(-10,1)')

mean4 = -20;
var4 = 4;
a4 = sqrt(3*var4) + mean4;
b4 = mean4 - sqrt(3*var4);   
r91 = a4 + (b4-a4)*z;    %Generate 10000 random variables with mean -20, variance 4
subplot(2,2,4)
hist(r91)
title('Histogram 4: u(-20,2)')
