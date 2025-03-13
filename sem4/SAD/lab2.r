#zadanie 1
#--------------------
loty = read.csv("/home/msjtw/Downloads/loty.csv",sep=";")
class(loty)
loty$X1955
loty[,1]

#srednia
rok1955 = loty$X1955
srednia = mean(rok1955)
# w roku 19555 średnia liczba pasażeróœ pewnej linii lotnicej wynosiła 284 osoby

#kwartyle

#1szy kwartyl
Q = quantile(rok1955)
Q1 = Q[2];
print(Q1)
# W roku 1955 w 3 miesiąscach liczba pasazerów była mniejsza  równa 261, a w pozostałych większa lub równa 261.

#mediana
Q2 = Q[3];
print(Q2)
# W roku 1955 w 6 miesiąscach liczba pasazerów była mniejsza równa 272, a w pozostałych większa lub równa 272.

#3ci kwartyl
Q3 = Q[4];
print(Q3)

#odchylenie stadardowe
sd = sd(rok1955)
print(sd)
# W roku 1955 liczba pasażerów odchylała się średnio o 42 osoby.

#współczynnik zmienności
v=sd/srednia*100
print(v)
# Wpółczynnik zmienności wynosił ok. 15%. To oznacza że liczba pasażerów była średnio zróżnicowana.

#histogramy liczebości
par(mfrow=c(2,3))
przedzialy = seq(200,650,length=7)
kolory = c("pink","yellow","orange","blue","magenta","green","purple")
for (i in 1:length(loty)) {
  hist(loty[,i],main=names(loty)[i],breaks = przedzialy ,col = kolory) 
}
par(mfrow=c(1,1))
boxplot(loty)

#zadanie 2
#--------------------
oceny = read.csv("/home/msjtw/Downloads/oceny.csv",dec=',',sep=";")
library(arm)
grupy = names(oceny)
par(mfrow=c(2,2))
for (i in 1:length(oceny)) {
  pie(table(na.omit(oceny[i])))
}
par(mfrow=c(2,2))

boxplot(oceny)


#zadanie 3
#--------------------
truskawki = read.csv("/home/msjtw/Downloads/truskawki.csv",dec=',',sep=";")
pie(table(cut(na.omit(truskawki[,1]), 5)))
pie(table(cut(na.omit(truskawki[,1]), 5)))
      
      