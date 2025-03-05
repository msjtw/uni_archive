#zadanie 1
sin(2*pi)
cos(3/4)
tan(pi)
log10(100)
log(15)
log(1/7, base=7)
exp(3)
64^(1/3)

#zadanie 2
a = c(seq(1,10,by=1))
sum(a)

#zadanie 3
x = c(seq(2,20,by=2))
length(x)
y = rev(x)
print(x*x)
print(x^2)
print(sqrt(sum(x^2)))
print(t(x)%*%y)
print(x%*%t(y))      

#zadanie 4
v3 = c(seq(5,10,length=13))
print(v3)

#zadanie 5
z1 = rep(c(1,2),times=5)
z2 = rep(c(1,2),each=5)
print(z1 + 4)
z3 = z2[-c(10)]
c = z1+z3
z4 = z1[z1>1]

#zadanie 6
a = c(1,3,6,2,7,4)
min(a)
which.min(a)
which(a>=4)
sum(a)
sum(a*a)
length(a)
a[3]
b = a[-c(4)]

#zadanie 7
A = cbind(c(2,1,1),c(3,-1,1),c(0,2,-1))
A^2
A%*%A
b = A[3,]

#zadanie 8
x = round(runif(n = 10, min = 1, max = 10))
y = round(runif(n = 10, min = 1, max = 10))
plot(x,y)
df = data.frame(x,y)
plot(df)
rb = rbind(x,y)
plot(rb)
cb = cbind(x,y)
plot(cb)

#zadanie 9
x = seq(-3,4,0.1)
y = x^2 + 3*x - 5
plot(x, y)
