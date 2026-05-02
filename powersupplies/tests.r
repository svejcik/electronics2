plot(c(0,3),c(0,6),'n',xlab='Pass Current (Amps)',ylab='Output Voltage (Volts)')
d<-read.csv('current-limiting-47-1000-7-50-3.csv',header=TRUE)
points(d$Ipass,d$Vout,pch=20,type='b')
d<-read.csv('current-limiting-470-1000-100-100-3.csv',header=TRUE)
points(d$Ipass,d$Vout,pch=21,type='b')
d<-read.csv('current-limiting-470-1000-10-100-3.csv',header=TRUE)
points(d$Ipass,d$Vout,pch=23,type='b')
d<-read.csv('current-limiting-470-1000-10-47-3.csv',header=TRUE)
points(d$Ipass,d$Vout,pch=25,type='b')
leg=c('R1=47,R2=1000,Rsc=3,Ra=6.8,Rb=50',
'R1=470,R2=1000,Rsc=3,Ra=100,Rb=100',
'R1=470,R2=1000,Rsc=3,Ra=10,Rb=100',
'R1=470,R2=1000,Rsc=3,Ra=10,Rb=47')
pchlist=c(20,21,23,25)
legend(1.7,6,leg,pch=pchlist)