d1<-read.table('current-limiting-1.csv',header=TRUE,sep=",")
plot(d1$Vout~d1$Iload)
plot(d1$Vout~d1$Ipass)
plot(d1$Ipass~d1$Iload)

#plot(d1$Vout~d1$Iload)
#par(new=FALSE)
#points(d1$Vout,d1$Iload,pch=11)
                 