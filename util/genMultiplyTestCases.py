import genSubtractTestCases as gs
import genExponentiationTestCases as ge
import random


def genMultiplyPair():
  return gs.genSubtractPair()

def genMultiplyTestCase():
  x, y = genMultiplyPair ()
  return x, y, x * y

def printMultiplyTestCase():
  x, y, p = genMultiplyTestCase()
  print 'CHECK(runMultiplication(Int("%s"),\n'\
        '                        Int("%s"),\n'\
        '                        Int("%s")));' % (x, y, p)

if __name__ == '__main__':
  for i in xrange(10):
    printMultiplyTestCase()
