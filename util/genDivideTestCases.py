import random

LOWER_BOUND = int(1e12)
MID_BOUND   = int(1e50)
UPPER_BOUND = int(1e100)

def genSmallBigNum():
  return random.randint(LOWER_BOUND, MID_BOUND)

def genBigBigNum():
  return random.randint(LOWER_BOUND, UPPER_BOUND)

def genDividePair():
  """ Return (x, y) such that x > y """
  x = genBigBigNum()
  for i in xrange(0, 200):
    y = genSmallBigNum()
    if x > y:
      return (x, y)
  return genDividePair()

def genDivideTestCase():
  """ Return (x, y, q) such that x > y and x / y = q (integer division """
  x, y = genDividePair()
  q = x / y
  return x, y, q

def printDivideTestCase():
  x, y, q = genDivideTestCase()
  print 'CHECK(runDivision(Int("%s"),\n'\
        '                  Int("%s"),\n'\
        '                  Int("%s")));' % (x, y, q)
  print 'CHECK(runDivision(Int("-%s"),\n'\
        '                  Int("%s"),\n'\
        '                  Int("-%s")));' % (x, y, q)
  print 'CHECK(runDivision(Int("%s"),\n'\
        '                  Int("-%s"),\n'\
        '                  Int("-%s")));' % (x, y, q)
  print 'CHECK(runDivision(Int("-%s"),\n'\
        '                  Int("-%s"),\n'\
        '                  Int("%s")));' % (x, y, q)


if __name__ == '__main__':
  for i in range(10):
    printDivideTestCase()




  
