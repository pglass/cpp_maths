import genDivideTestCases as gd
import random

BIN_LIMIT= 1000000000

def genQRPair():
  """ Return y, x such that 0 < x <= y < BIN_LIMIT * x """
  x = gd.genBigBigNum()
  y = random.randint(x, x * BIN_LIMIT - 1)
  return y, x

def genQRTestCase():
  y, x = genQRPair()
  return y, x, y / x, y % x

def printQRTestCase():
  y, x, q, r = genQRTestCase()
  print 'CHECK(runQuotientAndRemainder(Int("%s"),\n'\
        '                              Int("%s"),\n'\
        '                              %s,\n'\
        '                              Int("%s")));' % (y, x, q, r)

if __name__ == '__main__':
  for i in range(10):
    printQRTestCase()
