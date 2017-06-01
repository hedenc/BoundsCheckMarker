# For generating alpha.def for switch-labels for alphanumeric characters

for x in range(ord('a'), ord('z')+1):
    print("X(\'%c\')" % (chr(x)))

for x in range(ord('A'), ord('Z')+1):
    print("X(\'%c\')" % (chr(x)))

print("X('_')")
