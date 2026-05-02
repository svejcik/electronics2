f=file('dac0-calibration-v0.0.0000.csv','w')
f.write(','.join(('time','count','Vout\n')))
for c in range(128):
  f.write(','+str(c)+',\n')
