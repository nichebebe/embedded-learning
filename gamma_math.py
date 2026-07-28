import math

gamma = 2.2

print("const unsigned char gammaTable[256] = {" )

for i in range(256):
    value = round(pow(i / 255.0, gamma) * 255)
    end = "," if i != 255 else ""
    print(f"{value}{end}")
print("};")