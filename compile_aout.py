import sys
import subprocess

if len(sys.argv) != 2:
    print('usage: compile_aout.py <filename>')
    exit()

filename = sys.argv[1]

def run(cmd):
    print('--- ' + cmd)
    proc = subprocess.Popen(cmd, shell=True, stderr=subprocess.PIPE)
    err = proc.stderr.read().decode('UTF-8')
    if err != '':
        for line in err.split('\n'):
            print(line)
        

run('i686-elf-gcc -c -fno-ident ' + filename)
objname = filename[:-1] + 'o'
run('gobjcopy -O a.out-i386-freebsd -R .eh_frame ' + objname)
data = subprocess.Popen('hexdump ' + objname, shell=True, stdout=subprocess.PIPE).stdout.read().decode('UTF-8')
processed = []
for entry in data.split():
    if len(entry) == 2:
        processed.append('0x' + entry)
cnt = 0
len = 14
curr = 0
result = ''
for entry in processed:
    result += entry + ', '
    cnt += 1
    curr += 1
    if curr == len:
        curr = 0
        result += '\n'
out_filename = 'bytes.txt'
out = open(out_filename, 'w')
result = result[:-3] if result[-1] == '\n' else result[:-2]
out.write(result + '\n')
out.close()
print('bytes in hex written to %s, total %d bytes' % (out_filename, cnt))
subprocess.Popen('rm ' + objname, shell=True)