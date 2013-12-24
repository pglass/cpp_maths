import genDivideTestCases as gd 
import random

def genBigBigNum():
  switch = random.randint(1, 10)
  if switch % 2 == 0:
    return gd.genBigBigNum()
  else:
    return -gd.genBigBigNum()

def genSmallBigNum():
  switch = random.randint(1, 10)
  if switch % 2 == 0:
    return gd.genSmallBigNum()
  else:
    return -gd.genSmallBigNum()

def genSubtractPair():
  switch = random.randint(1, 16)
  if switch % 4 == 0:
    return genBigBigNum(), genBigBigNum()
  elif switch % 4 == 1:
    return genSmallBigNum(), genBigBigNum()
  elif switch % 4 == 2:
    return genBigBigNum(), genSmallBigNum()
  else:
    return genSmallBigNum(), genSmallBigNum()

def genSubtractTestCase():
  x, y = genSubtractPair()
  return x, y, x - y

def printSubtractTestCase():
  x, y, d = genSubtractTestCase()
  print 'CHECK(runSubtraction(Int("%s"),\n'\
        '                     Int("%s"),\n'\
        '                     Int("%s")));' % (x, y, d)

if __name__ == '__main__':
  for i in xrange(0, 10):
    printSubtractTestCase()
