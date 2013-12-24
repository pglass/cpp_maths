import genDivideTestCases as gd
import random

def genModuloPair():
  x, y = gd.genDividePair()
  switch = random.randint(1, 40)
  if switch % 4 == 0:
    return x, y
  elif switch % 4 == 1:
    return -x, y
  elif switch % 4 == 2:
    return x, -y
  else:
    return -x, -y

def genModuloTestCase():
  x, y = genModuloPair()
  return x, y, x % y

def printModuloTestCase():
  x, y, m = genModuloTestCase()
  print 'CHECK(runModulus(Int("%s"),\n'\
        '                 Int("%s"),\n'\
        '                 Int("%s")));' % (x, y, m)

if __name__ == '__main__':
  for i in xrange(10):
    printModuloTestCase()
