import subprocess

p = subprocess.Popen(['adb', 'devices'], stdout=subprocess.PIPE)
out, err = p.communicate()
print(out)
print(out.decode('utf-8'))