import random

LOWER_BOUND = int(2)
UPPER_BOUND = int(500)

WRAP_WIDTH = 80


def genExpBase():
  return random.randint(LOWER_BOUND, UPPER_BOUND)

def genExpExp():
  return random.randint(LOWER_BOUND, UPPER_BOUND)

def genExpPair():
  x, y = genExpBase(), genExpExp()
  switch = random.randint(1,10)
  if switch % 2 == 0:
    return x, y
  elif switch % 2 == 1:
    return -x, y

def genExpTestCase():
  x, y = genExpPair()
  return x, y, x ** y

def wrapString(s, indent_width, wrap_width):
  indent = " " * indent_width
  result = ""
  i = 0
  while i < len(s):
    result += '%s"%s"\n' % (indent, s[i:i+wrap_width])
    i += wrap_width
  return result.strip()

def printExpTestCase():
  x, y, e = genExpTestCase()
  print 'CHECK(runExponentiation(Int("%s"),\n'\
        '                        Int("%s"),\n'\
        '                        Int(%s)));' % (x, y, wrapString(str(e), 28, WRAP_WIDTH))

if __name__ == '__main__':
  for i in range(10):
    printExpTestCase()
#  s = ("abcde" * 30) + "X"
#  print s
#  print wrapString(s, 5, 51)
