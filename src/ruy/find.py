import os
from glob import iglob


statics = []
for name in iglob('/usr/local/lib/*.a'):
    name = os.path.basename(name).removesuffix('.a')
    # if 'libruy_' in name:
    name = name.replace('libruy_', '-lruy_')
    name = name.replace('libg', '-lg')
    statics.append(name)

print(' '.join(statics))
