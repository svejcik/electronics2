data<-read.csv('foldback-3000-470-1-100-1.csv')
plot(data$I,data$EP-data$BP)
data3000_470<-read.csv('foldback-3000-470-1-100-1.csv')
data3000_100<-read.csv('foldback-3000-100-1-100-1.csv',sep="")
data1000_100<-read.csv('foldback-1000-100-1-100-1.csv',sep="")
data470_47<-read.csv('foldback-470-47-1-100-1.csv',sep="")

# Load current vs Vout.
plot(data3000_470$IL,data3000_470$V0,pch=17,ylim=c(0.,6.))
points(data1000_100$IL,data1000_100$V0,pch=20)
points(data470_47$IL,data470_47$V0,pch=21)
points(data3000_100$IL,data3000_100$V0,pch=22)

# Pass current vs.Vbe of Qswitch.
# These show how the pass transistor implicitly cuts off
# as its collector current raises Vbe of Qswitch. The cutoff
# is directly related to the magnitude of Vbe. 
plot(data3000_470$IPASS,data3000_470$BS-data3000_470$V0,pch=17,ylim=c(0.,2.))
points(data3000_100$IPASS,data3000_100$BS-data3000_100$V0,pch=20)
points(data1000_100$IPASS,data1000_100$BS-data1000_100$V0,pch=21)
points(data470_47$IPASS,data470_47$BS-data470_47$V0,pch=22)

# Turn on of regulator: these show the pass transistor current
# limiting as Qswitch turns on. Turn on values are generally 
# for Qswitch collector current at around 2-4 mA.
plot(data470_47$IC,data470_47$IPASS,pch=17,xlim=c(0,15))
plot(data3000_100$IC,data3000_100$IPASS,pch=17,xlim=c(0,15))
plot(data1000_100$IC,data1000_100$IPASS,pch=17,xlim=c(0,15))
plot(data3000_470$IC,data3000_470$IPASS,pch=17)

# these show that the current through Ra is tiny, << Qswitch 
# collector current. Thus, Qswitch collector current is dominated 
#by the base current of Q1 which is typicall 10-70 mA.
# To keep Qswitch out of saturation, choose 70mA x Rb<10V,
# or Rb<148 Ohms.
plot(data3000_470$IC,(data3000_470$EP-data3000_470$B1)/3000.)
plot(data470_47$IC,(data470_47$EP-data470_47$B1)/470.)
plot(data1000_100$IC,(data1000_100$EP-data1000_100$B1)/1000.)
plot(data3000_100$IC,(data3000_100$EP-data3000_100$B1)/1000.)


