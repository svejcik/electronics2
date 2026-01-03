import numpy
import sys
import time
HEARTBEAT = 1000
# parse ADC 7822 serial read from file created by xxd conversion of cat /dev/ttyS0
# example of running from command line 
# cat /dev/ttyS0 | xxd | python reverse.py 2> ADCerrors 1> ADCoutput
def adcparse(datum):
  out = None
  #print '==========================',datum
  for iframe in [0,1,2,3]:
    #print datum[iframe*2:iframe*2 + 2]
    x = int(datum[iframe*2:iframe*2+2], 16)
    if iframe == 0:
      xrev = 0
      for i in range(8):
        xrev <<= 1
        xrev |=(x&1)
        x>>=1
      frame0bits = str.zfill(bin(xrev)[2:],8)
      #print iframe, x, frame0bits
    elif iframe == 1:
      b1 = x & 0x01
      if b1 != x & 0x04>>2:
        print 'datum 0x%x frame 0x%x b10x%x 0x%x'%(x, iframe, b1, x & 0x04)
      else:
        pass
      frame1bits = str.zfill(bin(x)[2:],8)
      #print iframe, x, frame1bits
    elif iframe == 2:
      frame2bits = str.zfill(bin(x)[2:],8)
      #print iframe, x, frame2bits
    elif iframe == 3:
      frame3bits = str.zfill(bin(x)[2:],8)
      #print iframe, x, frame3bits
      #print '======================'
    else:
      pass
  if frame0bits[0] != frame2bits[5]:
    sys.stderr.write(' mismatch bit 11 %s %s\n'%(frame0bits, frame2bits))
  elif frame0bits[1] != frame2bits[6]:
    sys.stderr.write( 'mismatch bit 10 %s %s\n'%(frame0bits, frame2bits))
  elif frame0bits[2] != frame2bits[7]:
    sys.stderr.write( 'mismatch bit  9 %s %s\n'%(frame0bits, frame2bits))
  elif frame0bits[5] != frame1bits[0]:
    sys.stderr.write( 'mismatch bit  6 %s %s\n'%(frame0bits, frame1bits))
  elif frame0bits[6] != frame1bits[1]:
    sys.stderr.write( 'mismatch bit  5 %s %s\n'%(frame0bits, frame1bits))
  elif frame0bits[7] != frame1bits[2]:
    sys.stderr.write( 'mismatch bit  4 %s %s\n'%(frame0bits, frame1bits))
  elif frame1bits[5] != frame1bits[7]:
    sys.stderr.write( 'mismatch bit  1 %s %s\n'%(frame1bits, frame1bits))
  else:
    out = (2**11)*int(frame0bits[0]) + \
          (2**10)*int(frame0bits[1]) + \
          (2**9 )*int(frame0bits[2]) + \
          (2**8 )*int(frame0bits[3]) + \
          (2**7 )*int(frame0bits[4]) + \
          (2**6 )*int(frame0bits[5]) + \
          (2**5 )*int(frame0bits[6]) + \
          (2**4 )*int(frame0bits[7]) + \
          (2**3 )*int(frame1bits[3]) + \
          (2**2 )*int(frame1bits[4]) + \
          (2**1 )*int(frame1bits[5]) + \
          (2**0 )*int(frame1bits[6])
  return out      

def main(datafile='dump.txt', repeat = None):
  VREF = 5.0
  fmat = 'default'
  droppedword = 0
  carry = ''
  if isinstance(datafile, str):
    a = file(datafile)
  else:
    a = datafile
  reclength = 8   
  nBad = 0 
  vouts = []
  rec = a.readline()
  
  nread = 0
  while rec != '':
    if not (nread % HEARTBEAT):
      sys.stderr.write('heartbeat...%i\n'%(nread/HEARTBEAT))
      sys.stderr.write('%s\n'%rec)
    words = rec.split()[1:-1]
    try:
      # default: concatenate 0+1, 2+3, 4+5, 6+7 as 32 bits at a time: 4 8-bit reads.
      if fmat=='default':
        dataread = [words[2*i] + words[2*i+1] for i in range(reclength/2)]
      elif fmat=='dropword31':
        dataread = [carry + words[0] + words[1][:2],
                    words[1][2:] + words[2] + words[3][:2],
                    words[3][2:] + words[4] + words[5][:2],
                    words[5][2:] + words[6] + words[7][:2]]
        carry = words[7][2:]
      elif fmat=='dropword32':
        dataread = [carry + words[0],
                    words[1] + words[2],
                    words[3] + words[4],
                    words[5] + words[6]]
        carry = words[7]
      elif fmat=='dropword33':
        dataread = [carry + words[0][:2],
                    words[0][2:] + words[1] + words[2][:2],
                    words[2][2:] + words[3] + words[4][:2],
                    words[4][2:] + words[5] + words[6][:2]]
        carry = words[6][2:] + words[7]
      for datum in dataread:
        adcout = adcparse(datum)
        if adcout is not None:
          vout = VREF * float(adcout)/(2**12)
          try:
            vouts.append(vout)
          except:
            vouts = [vout]
        else:
          # currently assuming an error is solely due to dropping last word of the 4 reads as this is
          # the most likely cause. Probably want more intelligent check of this, e.g. look for the
          # tag value of word 4 (I think this is always xFE, but not sure, maybe xFF).
          droppedword +=1
          if droppedword >= 4:
            if fmat=='default':
              fmat='dropword31'
              carry = words[7][2:]
            elif fmat=='dropword31':
              fmat='dropword32'
              carry = words[7]
            elif fmat=='dropword32':
              fmat='dropword33'
              carry = words[6][2:] + words[7]
            else:
              fmat='default'
            droppedword = 0
          sys.stderr.write('Parsing datum %s\n'%datum)
          sys.stderr.write('Full record: %s \n'%rec)
          nBad +=1    
    except:
      sys.stderr.write('Error!')
    if repeat is not None:
      if nBad + len(vouts) >= repeat:
        vouts = numpy.asarray(vouts)
        sys.stderr.write('Finished cycle at %s\n'%time.ctime())
        sys.stdout.write('%s\n'%time.ctime())
        sys.stdout.write('%f V +/- %f V\n'%(vouts.mean(), vouts.std()))
        sys.stdout.write('Total words%i Bad %i\n'%(len(vouts), nBad))
        sys.stdout.flush()
        vouts = []
        nBad = 0
      if nBad + len(vouts) > repeat:
        sys.stderr.write('Too many reads!\n')
    rec = a.readline()
    nread += 1
  vouts = numpy.asarray(vouts)
  print('%f V +/- %f V\n')%(vouts.mean(), vouts.std())
  print('Grand Total words%i Bad %i\n'%(len(vouts), nBad))

if __name__=="__main__":
  if len(sys.argv)==2:
    try:
      repeat = int(sys.argv[1])
    except:
      sys.stderr.write('Second argument must be an integer, number of repeat cycles\n')
  else:
    repeat = 1500000
  main(sys.stdin, repeat = repeat)
  #main('test.dat', repeat = repeat)

