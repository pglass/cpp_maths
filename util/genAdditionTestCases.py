import genSubtractTestCases as gs

def genAddPair():
  return gs.genSubtractPair()

def genAddTestCase():
  x, y = genAddPair()
  return x, y, x + y

def printAddTestCase():
  x, y, s = genAddTestCase()
  print 'CHECK(runAddition(Int("%s"),\n'\
        '                  Int("%s"),\n'\
        '                  Int("%s")));' % (x, y, s)

if __name__ == '__main__':
  for i in xrange(0, 10):
    printAddTestCase()
